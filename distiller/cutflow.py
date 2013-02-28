#!/usr/bin/env python2.7

from susy import cutflow
import sys

values = cutflow.cutflow(sys.argv[1:], flags='vbg', 
                         grl='~/btag-calibration/grl.xml', 
                         output_ntuple='py-output.root', 
                         btag_cal_file='~/btag-calibration/BTagCalibration.env', 
                         cal_dir='~/btag-calibration', 
                         systematic='NONE')
for key, value in values: 
    print '{:20}: {}'.format(key, value)
