#!/usr/bin/env python2.7 

from scharm.stack import hfw
import os, sys
from scharm import bits

# limit_dict = {
#     'min_pt': 100*1e3, 
#     'max_pt': 300*1e3, 
#     'hard_cu': -0.5, 
#     }

# hyperstack.hypersusy(
#     sys.argv[1], [('all',0)], 'hyper.h5', flags='v', limit_dict=limit_dict)

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
        'type': 'CONTROL', 
        'jet_tag_requirements': ['JFC_MEDIUM','JFC_MEDIUM'], 
        'tagger':'JFC', 
        'systematic':'NONE', 
        'jet_tag_assignment':'PT_ORDERED', 
        }, 
    ]

hfw.stacksusy(sys.argv[1], all_cuts, flags='v')
