#!/usr/bin/env python2.7 

from stop import hyperstack
import os, sys

limit_dict = {
    'min_pt': 100*1e3, 
    'max_pt': 300*1e3, 
    }

hyperstack.hypersusy(
    sys.argv[1], [('all',0)], 'hyper.h5', flags='v', limit_dict=limit_dict)
hyperstack.stacksusy(sys.argv[1], [('all',0)], 'stupid.h5', flags='v')
