#!/usr/bin/env bash

if [[ -z $1 ]]
then
    echo "please give a directory containing images and tables" >&2
    exit 1
fi

# sort tables / figures
figs=figures/limit_tree
for fig in $(find $1 -type f -name '*.pdf')
do
    tail=${fig#*$1}
    dest=$figs/$tail
    mkdir -p $(dirname $dest)
    cp $fig $dest
done

tables=tables/generated
for table in $(find $1 -type f -name '*.tex')
do
    tail=${table#*$1}
    dest=$tables/$tail
    mkdir -p $(dirname $dest)
    if [[ $table == */systable.tex || $table == */yieldtable.tex ]]
	then
	susy-fit-translate-defs.py $table >| $dest
    else
	cp $table $dest
    fi
done
