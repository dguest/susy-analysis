#!/usr/bin/env bash 

if [[ $ROOTCOREDIR ]]
    then 
    sed -n "s:^[ \t]*printf://&:g" ${ROOTCOREDIR}/../**/Root/*
    echo "RootCore should keep its damn mouth shut now (after you recompile)"
else 
    echo "Set ROOTCOREDIR for this this to work"
    false
fi