#!/usr/bin/env python2.7

from ROOT import TFile, TTree, TH1D
from math import log, exp

root_file = TFile('v16_jfcWithSv1.root')

root_tree = root_file.Get('SVTree')

#Variable Definition
#comment

# the proper weight syntax should be in v16 root file
#cOverB = entry.weightC / entry.weightB
#cOverL = entry.weightC / entry.weightL

likelihood_hist = TH1D('likelihood_c','',100,0,1)
out_file = TFile('output.root','recreate')

#may need to change these hist names to match the ones already there
b_hist = TH1D('b_hist', '', 100, 0,1)
out_fileB = TFile('outputB.root','recreate')

#c_hist = TH1D('c_hist', '', 100, 0,1)
#out_fileC = TFile('outputC.root','recreate')

#l_hist = TH1D('l_hist', '', 100, 0,1)
#out_fileL = TFile('outputL.root','recreate')



for n, entry in enumerate(root_tree): 
    print entry.Likelihood_bNewTune
    likelihood_hist.Fill(entry.Likelihood_bNewTune)
    b_hist.Fill(entry.Likelihood_b)

 #   if entry.light == 1:
        #----- guess for the filling member
  #      l_hist.Fill(log(cOverL))

   # if entry.charm == 1:
    #    c_hist.Fill(entry.Likelihood_cNewTune)

    
   # if entry.bottom == 1:
    #    b_hist.Fill(entry.Likelihood_bNewTune)

    


    if n > 10: break 

out_file.WriteTObject(likelihood_hist)
out_fileB.WriteTObject(b_hist)
