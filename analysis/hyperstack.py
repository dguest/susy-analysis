#!/usr/bin/env python3.3

from scharm.stack import hfw
import os, sys
try:
    from scharm.stack.regions import sbottom_regions as sbr
    from scharm.stack.regions import Region
    def named_region(name, yaml_dict):
        reg_dict = Region(yaml_dict).get_config_dict()
        reg_dict['name'] = name
        reg_dict['output_name'] = 'nminus.h5'
        reg_dict['stream'] = 'FULLSIM'
        reg_dict['save_wt2'] = True
        return reg_dict
    all_cuts = [named_region(x,d) for x,d in sbr().items()]
except ImportError:
    all_cuts = [
        {
            'name': 'sr',
            'output_name': 'nminus.h5',
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
