#!/usr/bin/env python2.7 

from scharm.stack import hfw
import os, sys
from scharm import bits

all_cuts = [('all',0)] + bits.bits

every_cut = 0
for cut, bit in all_cuts: 
    every_cut |= bit
all_cuts.append( ('every',every_cut) )
short_cuts = [('every',every_cut)]

all_cuts = [
    { 
        'name': 'pt-order', 
        'output_name': 'histmill.h5', 
        'selection':'QUALITY_EVENT', 
        'type': 'CONTROL', 
        'jet_tag_requirements': ['JFC_MEDIUM','JFC_MEDIUM'], 
        'tagger':'JFC', 
        'systematic':'NONE', 
        'jet_tag_assignment':'PT_ORDERED', 
        }, 
    ]

hfw.stacksusy(sys.argv[1], all_cuts, flags='v')
