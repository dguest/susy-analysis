#!/usr/bin/env python2.7

from ROOT import TFile, TTree
from itertools import islice

ro_file = TFile('output_skim.root')
tree = ro_file.Get('evt_tree')

print 'dan {evt:>10} {njet:>10} {pt:>10} {eta:>10} {phi:>10} {met:>10}'.format(
    evt='evt n', 
    njet='n jets', 
    pt='j1 pt', 
    eta='j1 eta', 
    phi='j1 phi', 
    met='met')

bits = ( 
    1 << 2 | # grl
    1 << 3 | # trig
    1 << 4 | # lar error
    1 << 5 | # core
    1 << 6 | # clean
    1 << 7 | # good vx
    1 << 8 | # leading jet
    1 << 9 | # met
    # 1 << 10| # n jet
    0
    )

forstr = 'dan {evt:10} {njet:10} {pt:10.3f} {eta:10.3f} {phi:10.3f} {met:10.3f}'

for entry in islice(tree, 100000): 
    if not ( (entry.pass_bits & bits) == bits): continue
    print forstr.format(
        evt=entry.event_number, 
        njet=entry.n_good_jets, 
        pt=entry.jet1_pt / 1e3, 
        eta=entry.jet1_eta, 
        phi=entry.jet1_phi, 
        met=entry.met / 1e3)
