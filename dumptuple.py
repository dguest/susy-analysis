#!/usr/bin/env python2.7

from ROOT import TFile, TTree, TH1D
from math import log, exp

root_file = TFile('v16_jfcWithSv1.root')

root_tree = root_file.Get('SVTree')


likelihood_hist = TH1D('likelihood_c','',100,0,1)
out_file = TFile('output.root','recreate')



def discriminator(entry, c): 
    """
    this returns giacinto's discriminator
    """
    c = c/10.0
    w_c = entry.Likelihood_c
    w_b = entry.Likelihood_b
    w_l = entry.Likelihood_u
    return w_c / (c * w_l + (1-c) * w_b)

discriminator_hists = {}
c_values = [ x for x in range(0,11)]
for c in c_values: 
    flavor_hists = {}
    for flavor in ['bottom','charm','light']: 
        hist_name = flavor + '_cVal' + str(c)
        flavor_hists[flavor] = TH1D(hist_name,'',200,-5,5)
    
    discriminator_hists[c] = flavor_hists


for n, entry in enumerate(root_tree): 
    
    likelihood_hist.Fill(entry.Likelihood_bNewTune)
        
    for c in c_values:
         if entry.light == 1:
             flavor = 'light'
         if entry.charm == 1:
             flavor = 'charm'
         if entry.bottom == 1:
             flavor = 'bottom'
       
         discriminator_hists[c][flavor].Fill(discriminator(entry,c))




for c_hists in discriminator_hists.values():
    for flavor_hist in c_hists.values():
        
        out_file.WriteTObject(flavor_hist)

