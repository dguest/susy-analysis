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

dataset = 'mc11_7TeV.125206.PowHegPythia_VBFH130_tautauhh.evgen.EVNT.e893'
info = get_dataset_info(client, dataset)
nfiles = info.info['nFiles']
total_events = info.info['totalEvents']
filt_eff = info.extra['GenFiltEff_mean']
xsec = info.extra['crossSection_mean']

print 'files: {}, evt: {}, filteff: {}, xsec: {}'.format(
   nfiles, total_events, filt_eff, xsec)
                                                         
