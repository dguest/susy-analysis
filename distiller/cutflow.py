#!/usr/bin/env python2.7

from susy import cutflow
import sys

files = sys.argv[1:]

flags = 'vbg'
if 'data' in files[0]: 
    flags += 'd'

values = cutflow.cutflow(files, flags=flags, 
                         grl='~/btag-calibration/grl.xml', 
                         output_ntuple='py-output.root', 
                         btag_cal_file='~/btag-calibration/BTagCalibration.env', 
                         cal_dir='~/btag-calibration', 
                         systematic='NONE')
for key, value in values: 
    print '{:20}: {}'.format(key, value)
