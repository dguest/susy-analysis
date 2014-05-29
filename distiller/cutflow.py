#!/usr/bin/env python2.7

from scharm.distiller import cutflow
import sys, glob, os

flags = 'vbgzfc'
# flags += 'u'                    # generate pu reweighting file
configs = ['SIGNAL']
files = []

# 400_200_MET180 info (183440)
xsec = 357.354819775
lumi = 20.3
filteff = 0.37804
wt_total = 50000
# # MET80
# filteff = 0.37855
# # MET300
# filteff = 0.09768
# wt_total = 20000

lumicalc = 'pu_lumicalc.root'

cf_multiple = xsec * lumi * filteff / wt_total
print cf_multiple

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
        # mumet_output_ntuple='py-output-mumet.root',
        # leptmet_output_ntuple='py-output-leptmet.root',
        btag_cal_file='~/calibration/BTagCalibration.env',
        cal_dir='~/calibration',
        systematic='NONE',
        # systematic='MSCALEDOWN',
        cutflow=config,
        pu_config='~/calibration/pu_config.prw.root',
        pu_lumicalc='~/calibration/' + lumicalc)
    for key, value in values:
        print '{:20}: {:.1f}'.format(key, value)
