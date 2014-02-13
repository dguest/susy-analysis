#!/usr/bin/env python2.7

from ROOT import TFile, TTree
from itertools import islice

ro_file = TFile('py-output.root')
total = ro_file.Get('total_events').GetVal()
print "total {}".format(total)

