#!/usr/bin/env python3.3

from scharm.stack import hfw
import os, sys
try:
    from scharm.stack.regions import sbottom_regions as sbr
    from scharm.stack.regions import Region, NMINUS, EVENT_LIST
    def nr(name, yaml_dict, evlst=False):
        hists = EVENT_LIST if evlst else NMINUS
        reg_dict = Region(yaml_dict).get_config_dict(hists=hists)
        reg_dict['name'] = name
        reg_dict['output_name'] = '{}.h5'.format(hists.lower())
        reg_dict['stream'] = 'FULLSIM'
        reg_dict['save_wt2'] = True
        reg_dict['hists'] = 'EVENT_LIST'
        return reg_dict
    all_cuts = [nr(x,d) for x,d in sbr().items()]
    bonus = [nr(x, d, True) for x,d, in sbr().items() if d['type'] == 'signal']
except ImportError:
    all_cuts = [
        {
            'name': 'sr',
            'output_name': 'nminus.h5',
            'hists':'NMINUS',
            'selection':'VR_MCT',
            'type': 'SIGNAL',
            'jet_tag_requirements': ['JFC_MEDIUM','JFC_MEDIUM'],
            'tagger':'JFC',
            'systematic':'NONE',
            'jet_tag_assignment':'PT_ORDERED',
            },
        ]

hfw.stacksusy(sys.argv[1], all_cuts + bonus, flags='v')
