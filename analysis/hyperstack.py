#!/usr/bin/env python2.7 

from stop.stack import hfw
import os, sys
from stop import bits

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
        'output_name': 'stupid.h5', 
        'type': 'CONTROL', 
        'jet_tag_requirements': ['JFC_LOOSE','NOTAG'], 
        'region_bits': 0x1, 
        'tagger':'JFC', 
        'systematic':'NONE', 
        'jet_tag_assignment':'PT_ORDERED', 
        }, 
    { 
        'name': 'tag-order', 
        'output_name': 'stupid.h5', 
        'type': 'CONTROL', 
        'jet_tag_requirements': ['JFC_LOOSE','NOTAG'], 
        'region_bits': 0x1, 
        'tagger':'JFC', 
        'systematic':'NONE', 
        'jet_tag_assignment':'TAG_ORDERED', 
        }, 
    { 
        'name': 'bob', 
        'output_name': 'stupider.h5', 
        'type': 'CONTROL', 
        'jet_tag_requirements': ['NOTAG','JFC_LOOSE'], 
        'hists': 'KINEMATIC_STAT', 
        'tagger':'JFC', 
        'systematic':'BUP'
        }, 
    { 
        'name': 'boball', 
        'output_name': 'stupider.h5', 
        'type': 'CONTROL', 
        'jet_tag_requirements': ['NOTAG','JFC_LOOSE'], 
        'hists': 'HISTMILL', 
        'tagger':'JFC', 
        'systematic':'BUP'
        }, 
    ]

GeV = 1e3

added_cuts = dict(
    leading_jet = 260*GeV, 
    met         = 180*GeV, 
    # j2_anti_b   = -0.2, 
    # j2_anti_u   =  2.5, 
    # j3_anti_b   = -0.2, 
    # j3_anti_u   = -0.5, 
    )

hfw.stacksusy(sys.argv[1], all_cuts, flags='v')
