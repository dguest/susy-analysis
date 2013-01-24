#!/usr/bin/env python2.7

from pyAMI.client import AMIClient
from pyAMI.auth import AMI_CONFIG, create_auth_config
from pyAMI.query import get_dataset_info
import os

client = AMIClient()
if not os.path.exists(AMI_CONFIG):
   create_auth_config()
client.read_config(AMI_CONFIG)

print AMI_CONFIG

dataset = 'mc12_8TeV.157872.MadGraphPythia_AUET2BCTEQ6L1_SM_TT_directCC_175_170.merge.NTUP_SUSY.e1355_a159_a171_r3549_p1032'
#'mc11_7TeV.125206.PowHegPythia_VBFH130_tautauhh.evgen.EVNT.e893'
info = get_dataset_info(client, dataset)
nfiles = info.info['nFiles']
total_events = info.info['totalEvents']
try: 
   filt_eff = info.extra['GenFiltEff_mean']
except KeyError: 
   filt_eff = info.extra['approx_GenFiltEff']


try: 
   xsec = info.extra['crossSection_mean']
except KeyError:
   xsec = info.extra['approx_crossSection']

print 'files: {}, evt: {}, filteff: {}, xsec: {}'.format(
   nfiles, total_events, filt_eff, xsec)
                                                         
print info 
