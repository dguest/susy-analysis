#!/usr/bin/env bash

if (( $# < 1 ))
then
    echo "please give a directory containing images and tables" >&2
    exit 1
fi
set -e

function fixdir() {
    # workaround for bullshit with atlas not allowing svn dirs to
    # start with a digit
    sed -r 's:/([0-9]):/m\1:g' <<< $1
}

# sort tables / figures
figs=figures/limit_tree
for fig in $(find $1 -type f -name '*.pdf')
do
    tail=${fig#*$1}
    dest=$(fixdir $figs/$tail)
    mkdir -p $(dirname $dest)
    cp $fig $dest
done

tables=tables/generated
for table in $(find $1 -type f -name '*.tex')
do
    tail=${table#*$1}
    dest=$(fixdir $tables/$tail)
    mkdir -p $(dirname $dest)
    if [[ $table == */systable.tex || $table == */yieldtable.tex ]]
	then
	if ! susy-fit-translate-defs.py $table >| $dest ; then
	    echo problem translating $table
	    exit 1
	fi
    else
	cp $table $dest
    fi
done
