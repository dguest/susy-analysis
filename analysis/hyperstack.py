#!/usr/bin/env python3.3

from scharm.stack import hfw
import os, sys

all_cuts = [
    { 
        'name': 'sr', 
        'output_name': 'histmill.h5', 
        'hists':'NMINUS',
        'selection':'SIGNAL', 
        'type': 'SIGNAL', 
        'jet_tag_requirements': ['JFC_MEDIUM','JFC_MEDIUM'], 
        'tagger':'JFC', 
        'systematic':'NONE', 
        'jet_tag_assignment':'PT_ORDERED', 
        }, 
    ]

hfw.stacksusy(sys.argv[1], all_cuts, flags='v')
