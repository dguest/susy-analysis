#!/usr/bin/env bash

OUTDIR=fit_figs_and_tables

if [[ -z $1 ]]
then
    echo need file 1>&2
    exit 1
fi

# passed to python routines (for example to test them with -h)
# ee=-h

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
    if ! check_for_files $2 ; then return $?; fi
    if [[ ! -d $2/workspaces ]]
	then
	echo making limits for $2
	susy-fit-workspace.py $1 -o $2/workspaces -c $2/configuration.yml $ee
    fi
    mkdir -p $OUTDIR/$2
    local CLSFILE=$OUTDIR/$2/cls.yml
    if [[ ! -f $CLSFILE ]]
	then
	susy-fit-runfit.py $2/workspaces -o $CLSFILE $ee
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
    echo making parameters for $2

    for bgfit in $2/workspaces/**/background_*_afterFit.root
    do
	odir=$OUTDIR/$2/$(dirname ${bgfit#*/workspaces/})
	if [[ $3 ]] ; then regs='-r '$3 ; fi
	if [[ $4 ]]
	then
	    odir=$odir/$4
	fi
	echo "making systables in $odir"
	mkdir -p $odir
	susy-fit-systable.sh $bgfit -o $odir $regs $ee
	pars=$odir/fit-parameters.yml
	susy-fit-results.py $bgfit | tee $pars | susy-fit-draw-parameters.py \
	    -o $odir $ee
    done
    echo done making parameters for $2
}

# run full fit
if ! makelim $1 full_exclusion ; then exit 1 ; fi
if ! makebg $1 full_exclusion ; then exit 1 ; fi
if ! makepars $1 full_exclusion ; then exit 1 ; fi

# run systematics comparison
if ! makelim $1 compare_systematics ; then exit 1 ; fi
if ! makebg $1 compare_systematics ; then exit 1 ; fi
if ! makepars $1 compare_systematics ; then exit 1 ; fi

# run crw comparison
if ! makelim $1 compare_systematics ; then exit 1 ; fi
if ! makebg $1 compare_systematics ; then exit 1 ; fi
if ! makepars $1 compare_systematics ; then exit 1 ; fi

# run validation / sr plotting stuff
SIGREGIONS=signal_mct150,signal_mct200,signal_mct250
if ! makebg $1 vrsr ; then exit 1 ; fi
if ! makepars $1 vrsr vr_mct,vr_mcc vr_fit ; then exit 1 ; fi
if ! makepars $1 vrsr $SIGREGIONS sr_fit ; then exit 1 ; fi

