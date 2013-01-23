#!/usr/bin/env python2.7

from pyAMI.client import AMIClient
from pyAMI.auth import AMI_CONFIG, create_auth_config
from pyAMI.query import get_dataset_xsec_effic
import os

client = AMIClient()
if not os.path.exists(AMI_CONFIG):
   create_auth_config()
client.read_config(AMI_CONFIG)

print AMI_CONFIG

dataset = 'mc11_7TeV.125206.PowHegPythia_VBFH130_tautauhh.evgen.EVNT.e893'
xsec, effic = get_dataset_xsec_effic(client, dataset)
print xsec, effic
