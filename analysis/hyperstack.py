#!/usr/bin/env python2.7 

from stop import hyperstack
import os, sys

cuts_dict = {
    'min_pt': 100*1e3, 
    'max_pt': 300*1e3, 
    }

hyperstack.hypersusy(
    sys.argv[1], [('all',0)], 'hyper.h5', flags='v', cuts_dict=cuts_dict)
hyperstack.stacksusy(sys.argv[1], [('all',0)], 'stupid.h5', flags='v')
