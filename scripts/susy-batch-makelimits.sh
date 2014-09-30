#!/usr/bin/env bash

# _______________________________________________________________________
# usage and parsing

usage() {
    echo "${0##*/} [-t] [-h] [-l] [-z] [-o <output_dir>] <fit inputs>" >&2
}

OUTDIR=fit_figs_and_tables
SC_CLSFILE=stop-to-charm-cls.yml
DATASET_META=dataset-meta.yml # need this to get the cross sections

doc() {
    usage
    cat <<EOF

Wrapper to run fit limits.

Options:
 -o <out_dir>  set output dir, default $OUTDIR
 -l            also run upper limits (slow)
 -t            run test
 -h, --help    print help
 -z            tar and zip when finished
EOF
}

while (( $# ))
do
    case $1 in
	--help) doc; exit 1;;
	-h) doc; exit 1;;
	-o) shift; OUTDIR=$1; shift;;
	-t) ee=-h; shift;;
	-z) ZIP=1; shift;;
	-l) DO_UL=1; shift;;
	*)
	    if [[ -n $input ]]
		then
		usage
		echo 'too many inputs' >&2
		exit 2
	    else
		input=$1
	    fi
	    shift;;
    esac
done

if [[ -z $input ]]
then
    usage
    echo need file 1>&2
    exit 1
fi

# ____________________________________________________________________________
# common utility functions (mostly checking for files)

function check_for_files() {
    if [[ ! -d $1 ]]
    then
	echo can\'t find $1 >&2
	return 2
    fi
    local config=$1/configuration.yml
    if [[ ! -f $config ]]
    then
	echo cant find $config >&2
	return 2
    fi
    return 0
}

function matches_in() {
    # takes a directory, followed by a pattern
    if [[ ! -d $1 ]]
	then
	return 1
    fi
    local NWS=$(find $1 -type f -name $2 | wc -w)
    if (( $NWS > 0 ))
	then
	return 0
    fi
    return 1
}

function check() {
    if ! eval $@ ; then exit 1 ; fi
}

# __________________________________________________________________________
# define the main functions used

function makelim() {
    # first arg: yaml fit input file
    # second arg: subdir of OUTDIR where outputs go
    # third arg: additional stuff to pass to susy-fit-workspace
    if ! check_for_files $2 ; then return $?; fi
    local WSDIR=$2/workspaces
    if ! matches_in $WSDIR '*nominal*'
	then
	echo making limits for $2
	local fitargs="-o $WSDIR -c $2/configuration.yml $3 $ee"
	if ! susy-fit-workspace.py $1 $fitargs; then return 2; fi
    fi
    mkdir -p $OUTDIR/$2
    local CLSFILE=$OUTDIR/$2/cls.yml
    local SC_CLSPATH=$2/$SC_CLSFILE
    if [[ ! -f $CLSFILE ]]
	then
	if ! susy-fit-runfit.py $WSDIR -o $CLSFILE $ee; then return 2; fi
	if [[ -f $SC_CLSPATH ]]
	    then
	    cat $SC_CLSPATH >> $CLSFILE
	fi
    fi
    echo done limits for $2
}
function drawlim() {
    local CLSFILE=$OUTDIR/$1/cls.yml
    if [[ ! -f $CLSFILE ]]; then
	echo no $CLSFILE >&2
	return 1
    fi
    local OVL=$OUTDIR/$1/exclusion_overlay.pdf
    local BST=$OUTDIR/$1/exclusion_best.pdf
    if [[ ! -f $OVL ]] ; then
	echo drawing $OVL
	susy-fit-draw-exclusion.py $CLSFILE -o $OVL
    fi
    if [[ ! -f $BST ]] ; then
	echo drawing $BST
	susy-fit-draw-exclusion.py $CLSFILE --best-regions -o $BST
    fi
}
function drawlimsubset() {
    # first arg: dir with cls
    # second arg: out file name (put in dir with cls)
    # all remaining: regions to draw
    local CLSFILE=$OUTDIR/$1/cls.yml
    if [[ ! -f $CLSFILE ]] ; then
	echo no file ${CLSFILE}! >&2
	return 1
    fi
    local CONFIGS="-r ${@:3}"
    local OUTNAME=$OUTDIR/$1/$2
    if [[ ! -f $OUTNAME ]] ; then
	echo drawing $2 from $CLSFILE
	susy-fit-draw-exclusion.py $CLSFILE -o $OUTNAME $CONFIGS
    fi
}

function makews_updown() {
    # first arg: yaml fit input file
    # second arg: subdir of OUTDIR where outputs go
    if ! check_for_files $2 ; then return $?; fi
    local WSDIR=$2/workspaces
    if ! matches_in $WSDIR '*1sigma*'
	then
	for dr in --down --up
	do
	    echo making ${dr#--} limits for $2
	    susy-fit-workspace.py $1 -o $WSDIR -c $2/configuration.yml \
		$ee $dr
	done
    fi
}

function makebg() {
    if ! check_for_files $2 ; then return $?; fi
    if matches_in $2/workspaces "background*afterFit.root"; then
	return 0
    fi
    echo making bg fit for $2
    susy-fit-workspace.py $1 -o $2/workspaces -c $2/configuration.yml -fb $ee
    echo done bg fit for $2
}

function make_upper_limits() {
    # first arg: dir where workspace and upper-limits.yml live
    local WS_DIR=$1/workspaces
    local UL_FILE=$1/upper-limits.yml
    if ! matches_in $WS_DIR "*nominal.root"; then
	echo "no pre-fit workspaces found in $WS_DIR" >&2
	return 1
    fi
    if [[ ! -f $UL_FILE ]] ; then
	echo "making $UL_FILE"
	susy-fit-runfit.py $WS_DIR -c ul -o $UL_FILE
    fi

    local UL_OUTDIR=$OUTDIR/$1
    mkdir -p $UL_OUTDIR
    local CLS_FILE=$UL_OUTDIR/cls.yml
    if [[ ! -f $CLS_FILE ]] ; then
	echo "no $CLS_FILE found" >&2
	return 1
    fi
    local COMBINED_OUT=$UL_OUTDIR/combined-ul-cls.yml
    if [[ ! -f $COMBINED_OUT ]] ; then
	susy-fit-add-xsec.py -i $UL_FILE $DATASET_META
	if ! susy-fit-merge-cls.py $UL_FILE $CLS_FILE > $COMBINED_OUT; then
	    exit 2
	fi
    fi

    local PLOTDIR=$UL_OUTDIR/upper_limits
    mkdir -p $PLOTDIR
    local ALL_CONFIG="mct150 mct200 mct250"
    local FARG="$COMBINED_OUT --ul "
    local COMB_OUT=$PLOTDIR/scharm_combined.pdf
    if [[ ! -f $COMB_OUT ]]; then
	echo "drawing $COMB_OUT"
	susy-fit-draw-exclusion.py $FARG -r $ALL_CONFIG -o $COMB_OUT
    fi
    local CONFIG
    for CONFIG in $ALL_CONFIG ; do
	local OUT_PLT=$PLOTDIR/${CONFIG}.pdf
	if [[ ! -f $OUT_PLT ]]; then
	    echo "drawing $OUT_PLT"
	    susy-fit-draw-exclusion.py $FARG -r $CONFIG -o $OUT_PLT
	fi
    done
}

function make_model_independent_ul () {
    # first arg: directory containing workspaces
    local DISCWS=discovery_nominal.root
    local WS_DIR=$1/workspaces
    if ! matches_in $WS_DIR $DISCWS; then
	echo "no discovery workspaces found in $WS_DIR" >&2
	exit 1
    fi
    local UL_DIR=$OUTDIR/$1/upper_limits
    local OUTFILE=$UL_DIR/model_independent_limit.tex
    if [[ -f $OUTFILE ]] ; then
	return 0
    fi
    mkdir -p $UL_DIR

    local SHIT=pile-o-shit
    mkdir -p $SHIT
    (
	if cd $SHIT ; then
	    rm *
	else
	    exit 1
	fi
	local WS=$(find ../$WS_DIR -name $DISCWS)
	echo "making ul table"
	UpperLimitTable.py $WS -o ultab.tex >| bullshit.log 2>&1
	local BULLSHIT=$(wc -l bullshit.log | cut -d ' ' -f 1)
	echo "made ul table with $BULLSHIT lines of bullshit"
    )
    cp $SHIT/ultab.tex $OUTFILE
}

function makepars() {
    # parameters:
    # 1: directory containing workspaces (also used to name output)
    # 2: regions to print when making tables
    # 3: subdirectory for outputs
    # 4: signal point used in workspace (defaults to background)

    echo making parameters for $1

    local WSHEAD=background
    if [[ $4 ]]
    then
	WSHEAD=$4
	local DRAWARGS=-f
    fi

    local WSTAIL=afterFit.root
    local WSMATCH="*${WSHEAD}*_${WSTAIL}"
    if ! matches_in $1/workspaces $WSMATCH
	then
	echo "no matches to $WSMATCH $1/workspaces"
	return 1
    fi

    local fit
    for fit in $1/workspaces/**/$WSMATCH
    do
	local odir=$OUTDIR/$1/$(dirname ${fit#*/workspaces/})
	if [[ $2 ]] ; then regs='-r '$2 ; fi
	if [[ $3 ]]
	then
	    odir=$odir/$3
	fi
	if ! matches_in $odir "*.tex"
	    then
	    echo "making systables in $odir"
	    mkdir -p $odir
	    susy-fit-systable.sh $fit -o $odir $regs $ee
	fi
	if ! matches_in $odir "*.pdf"
	    then
	    echo "drawing parameters in $odir"
	    local pars=$odir/fit-parameters.yml
	    local draw="susy-fit-draw-parameters.py -o $odir $ee $DRAWARGS"
	    susy-fit-results.py $fit | tee $pars | $draw
	fi
    done
    echo done making parameters for $1
}

# __________________________________________________________________________
# check for files

if [[ ! -f full_exclusion/$SC_CLSFILE ]]
then
    echo "can't find $SC_CLSFILE for full exclusion, quitting" >&2
    exit 1
fi
if [[ $DO_UL && ! -f $DATASET_META ]] ; then
    echo "can't find $DATASET_META for upper-limits, quitting" >&2
    exit 1
fi

# __________________________________________________________________________
# run the actual routines here

# run full fit (pass -f to make fit results for all workspaces)
DEFREGIONS=signal_mct150,cr_w,cr_z,cr_t
BGREGIONS=cr_w,cr_z,cr_t
VREGIONS=vr_mct,vr_mcc
SIGREGIONS=signal_mct150,signal_mct200,signal_mct250
check makews_updown $input full_exclusion
check makelim $input full_exclusion -f
check drawlim full_exclusion
check makepars full_exclusion $BGREGIONS bg_fit
check makepars full_exclusion $DEFREGIONS srcr srcr
check makepars full_exclusion $DEFREGIONS 400_200 400-200
check makepars full_exclusion $DEFREGIONS 550_50 550-50
check makepars full_exclusion $DEFREGIONS 250_50 250-50

# upper limit stuff
if [[ $DO_UL ]]
then
    check make_upper_limits full_exclusion
    check make_model_independent_ul full_exclusion
fi

# run systematics comparison
if ! makelim $input compare_systematics ; then exit 1 ; fi
if ! drawlim compare_systematics ; then exit 1; fi
if ! makebg $input compare_systematics ; then exit 1 ; fi
if ! makepars compare_systematics ; then exit 1 ; fi

# run crw comparison
if ! makelim $input compare_crw ; then exit 1 ; fi
if ! drawlim compare_crw ; then exit 1; fi
if ! makebg $input compare_crw ; then exit 1 ; fi
if ! makepars compare_crw $VREGIONS vr_fit ; then exit 1 ; fi
if ! makepars compare_crw $BGREGIONS bg_fit ; then exit 1 ; fi

# other fit checks
check makelim $input other_fits -f
check drawlimsubset other_fits single_t.pdf normal st_with_other
check drawlimsubset other_fits jes.pdf normal jes_breakdown
check makepars other_fits $DEFREGIONS bg_fit
check makepars other_fits $DEFREGIONS 400_200 400-200

# run validation / sr plotting stuff
check makebg $input vrsr
check makepars vrsr $VREGIONS vr_fit
check makepars vrsr $SIGREGIONS sr_fit
check makepars vrsr signal_mct150 onesr_fit

# zip up result
if [[ $ZIP ]]
then
    echo zipping
    tar czf ${OUTDIR}.tgz $OUTDIR
fi