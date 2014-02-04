#!/usr/bin/env python2.7

from ROOT import TFile

f = TFile('skim-output.root')
t = f.Get('susy')


for entry in t: 
    n_el = t.el_n
    def check(name): 
        n_things = len(getattr(entry,name))
        if n_el != n_things:
            print '{} != {} {}'.format(n_el, n_things, name)
    check('el_trackz0pv')
    check('el_cl_E')
    check('el_cl_phi')
    check('el_tightPP')
