#!/usr/bin/env python2.7

from scharm.distiller import cutflow
import sys, glob, os

flags = 'vbgzfc'
# flags += 'u'                    # generate pu reweighting file
configs = ['NOMINAL']
files = []
if len(sys.argv) > 1: 
    if sys.argv[1] == 'all': 
        files = sys.argv[2:]
        configs = ['NOMINAL','ELECTRON_CR','MUON_CR']
    elif sys.argv[1] == 'data': 
        files = sys.argv[2:]
        flags += 'd'
    elif sys.argv[1] == 'sherpa': 
        files = sys.argv[2:]
        flags += 'p'            # boson pt reweight
    else: 
        files = sys.argv[1:]
    
if not files: 
    files = glob.glob('stop-files/*.root*')

for config in configs:
    print config
    values = cutflow.cutflow(
        files, flags=flags, 
        grl='~/calibration/grl.xml', 
        output_ntuple='py-output.root', 
        mumet_output_ntuple='py-output-mumet.root', 
        leptmet_output_ntuple='py-output-leptmet.root', 
        btag_cal_file='~/calibration/BTagCalibration.env', 
        cal_dir='~/calibration', 
        systematic='NONE', 
        # systematic='METUP', 
        cutflow=config, 
        pu_config='~/calibration/pu_config.prw.root', 
        pu_lumicalc='~/calibration/pu_lumicalc.root')
    for key, value in values: 
        print '{:20}: {}'.format(key, value)
