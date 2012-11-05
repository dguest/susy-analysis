#!/usr/bin/env python2.7 

from stop import hyperstack
import os, sys

hyperstack.hypersusy(sys.argv[1], [('all',0)], 'hyper.h5', flags='v')
hyperstack.stacksusy(sys.argv[1], [('all',0)], 'stupid.h5', flags='v')
