#!/usr/bin/env bash

# _______________________________________________________________________
# usage and parsing

usage() {
    echo "${0##*/} [-ht] [-o <output_dir>] <fit inputs>" >&2
}

OUTDIR=fit_figs_and_tables
SC_CLSFILE=stop-to-charm-cls.yml

doc() {
    usage
    cat <<EOF

Wrapper to run fit limits.

Options:
 -o <out_dir>  set output dir, default $OUTDIR
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

# __________________________________________________________________________
# define the main functions used

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

function makelim() {
    # first arg: yaml fit input file
    # second arg: subdir of OUTDIR where outputs go
    # third arg: additional stuff to pass to susy-fit-workspace
    if ! check_for_files $2 ; then return $?; fi
    local WSDIR=$2/workspaces
    if [[ ! -d $WSDIR ]]
	then
	echo making limits for $2
	susy-fit-workspace.py $1 -o $WSDIR -c $2/configuration.yml $3 $ee
    fi
    mkdir -p $OUTDIR/$2
    local CLSFILE=$OUTDIR/$2/cls.yml
    local SC_CLSPATH=$2/$SC_CLSFILE
    if [[ ! -f $CLSFILE ]]
	then
	susy-fit-runfit.py $WSDIR -o $CLSFILE $ee
	if [[ -f $SC_CLSPATH ]]
	    then
	    cat $SC_CLSPATH >> $CLSFILE
	fi
    fi
    echo drawing $CLSFILE
    susy-fit-draw-exclusion.py $CLSFILE -o $OUTDIR/$2/exclusion_overlay.pdf
    susy-fit-draw-exclusion.py $CLSFILE --best-regions \
	-o $OUTDIR/$2/exclusion_best.pdf
    echo done limits for $2
}
function makebg() {
    if ! check_for_files $2 ; then return $?; fi
    echo making bg fit for $2
    susy-fit-workspace.py $1 -o $2/workspaces -c $2/configuration.yml -fb $ee
    echo done bg fit for $2
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

    local WSTAIL=combined_meas_model_afterFit.root
    for fit in $1/workspaces/**/*${WSHEAD}*_${WSTAIL}
    do
	odir=$OUTDIR/$1/$(dirname ${fit#*/workspaces/})
	if [[ $2 ]] ; then regs='-r '$2 ; fi
	if [[ $3 ]]
	then
	    odir=$odir/$3
	fi
	echo "making systables in $odir"
	mkdir -p $odir
	susy-fit-systable.sh $fit -o $odir $regs $ee
	pars=$odir/fit-parameters.yml
	susy-fit-results.py $fit | tee $pars | susy-fit-draw-parameters.py \
	    -o $odir $ee $DRAWARGS
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

# __________________________________________________________________________
# run the actual routines here

# run full fit (pass -f to make fit results for all workspaces)
DEFREGIONS=signal_mct150,cr_w,cr_z,cr_t
BGREGIONS=cr_w,cr_z,cr_t
VREGIONS=vr_mct,vr_mcc
if ! makelim $input full_exclusion -f ; then exit 1 ; fi
if ! makepars full_exclusion $BGREGIONS bg_fit ; then exit 1 ; fi
if ! makepars full_exclusion $DEFREGIONS 400_200 400-200 ; then exit 1 ; fi
if ! makepars full_exclusion $DEFREGIONS 550_50 550-50 ; then exit 1 ; fi
if ! makepars full_exclusion $DEFREGIONS 250_50 250-50 ; then exit 1 ; fi


# run systematics comparison
if ! makelim $input compare_systematics ; then exit 1 ; fi
if ! makebg $input compare_systematics ; then exit 1 ; fi
if ! makepars compare_systematics ; then exit 1 ; fi

# run crw comparison
if ! makelim $input compare_crw ; then exit 1 ; fi
if ! makebg $input compare_crw ; then exit 1 ; fi
if ! makepars compare_crw $VREGIONS vr_fit ; then exit 1 ; fi
if ! makepars compare_crw $BGREGIONS bg_fit ; then exit 1 ; fi

# run validation / sr plotting stuff
SIGREGIONS=signal_mct150,signal_mct200,signal_mct250
if ! makebg $input vrsr ; then exit 1 ; fi
if ! makepars vrsr $VREGIONS vr_fit ; then exit 1 ; fi
if ! makepars vrsr $SIGREGIONS sr_fit ; then exit 1 ; fi
if ! makepars vrsr signal_mct150 onesr_fit ; then exit 1 ; fi

# zip up result
if [[ $ZIP ]]
then
    echo zipping
    tar czf ${OUTDIR}.tgz $OUTDIR
fi