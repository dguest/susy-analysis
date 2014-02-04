#!/usr/bin/env python2.7

from ROOT import TFile

f = TFile('skim-output.root')
t = f.Get('susy')
for entry in t: 
    for iii in xrange(t.mu_staco_n): 
        print t.mu_staco_pt[iii]
