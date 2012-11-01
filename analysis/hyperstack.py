#!/usr/bin/env python2.7 

from stop import hyperstack
import os, sys

hyperstack.stacksusy(sys.argv[1], [('all',0)], 'bullshit.h5', flags='v')
