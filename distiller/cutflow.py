#!/usr/bin/env python2.7

from susy import cutflow
import sys

values = cutflow.cutflow(sys.argv[1:], 180614, flags='vd', grl='grl.xml', 
                         output_ntuple='py-output.root')
for key, value in values: 
    print '{:20}: {}'.format(key, value)
