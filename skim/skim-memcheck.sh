#!/usr/bin/env bash 

if [[ ! $1 ]]
then 
    echo 'need a file to run on' >&2 
    exit 1
fi

supp=--suppressions=${ROOTSYS}/etc/root/valgrind-root.supp
leakch=--leak-check=full
valgrind $supp $leakch ./run-skim --check $1 