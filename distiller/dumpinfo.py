#!/usr/bin/env python2.7

from ROOT import TFile, TTree
from itertools import islice

ro_file = TFile('egamma-skim.root')
total = ro_file.Get('total_events').GetVal()
skimmed = ro_file.Get('skimmed_events').GetVal()
print "total {}".format(total)
print "skimmed {}".format(skimmed)

