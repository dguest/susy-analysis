#!/usr/bin/env python2.7

from susy import cutflow
import sys

values = cutflow.cutflow(sys.argv[1:], 180614, flags='vb', grl='grl.xml', 
                         output_ntuple='py-output.root', 
                         btag_cal_file='~/btag-calibration/BTagCalibration.env', 
                         btag_cal_dir='~/btag-calibration', 
                         systematic='NONE')
for key, value in values: 
    print '{:20}: {}'.format(key, value)
