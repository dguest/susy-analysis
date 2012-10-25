#!/usr/bin/env python2.7

from ROOT import TFile, gROOT, TH1D, TGraph, gPad, TCanvas,TLegend,TAttFill
import random, sys, array
import numpy

# --- utility functions 
def rejection(signal, background, total_background): 
    if background == 0: 
        return None
    return total_background / background
rejection.string = 'rejection'

def bin_rev_iter(hist): 
    n_bins = hist.GetNbinsX()
    for bin_num in xrange(n_bins + 1,1, -1): 
        yield hist.GetBinContent(bin_num)

def bin_fwd_iter(hist): 
    n_bins = hist.GetNbinsX()
    for bin_num in xrange(1,n_bins + 1): 
        yield hist.GetBinContent(bin_num)

def plots_to_xy(signal, background, y_function = rejection, rev_itr = True): 
    """
    reads in root histograms for signal and background, returns
    (efficiency, rejection) tuple. 
    """
    if isinstance(background, list): 
        total_background = background[0].Clone(str(random.random()))
        for other_background in background[1:]: 
            total_background.Add(other_background)
        background = total_background
        
    sum_background = 0
    sum_signal = 0

    sig_array = array.array('d')
    bkg_array = array.array('d')

    if rev_itr == True: 
        itr_func = bin_rev_iter
    else: 
        itr_func = bin_fwd_iter

    bin_values = zip(itr_func(signal), itr_func(background))
    total_signal = sum(itr_func(signal))
    total_background = sum(itr_func(background))
    for sig_val, bkg_val in bin_values: 
        sum_signal += sig_val
        sum_background += bkg_val

      

        y_value = y_function(sum_signal, sum_background, total_background)
        if y_value is not None: 
            sig_array.append(sum_signal / total_signal)
            bkg_array.append(y_value)


    assert len(sig_array) == len(bkg_array)
    return sig_array, bkg_array

def hist_getter(root_file, hist_name): 
    """
    workaround for root's weird ownership rules
    """
    # print 'getting', hist_name
    gROOT.cd()   
    print hist_name
    the_hist = root_file.Get(hist_name).Clone(str(random.random()))
    return the_hist



def hist_plotter(eff,root_file_name):
    """
    plotting function, for various efficiencies

    """

    root_file = TFile(root_file_name)

   # hist_base_name = 'logCbNewTune'
    

    flavors = ['bottom','charm','light']

    rejB_array = array.array('d')
    rejL_array = array.array('d')
    c_array = array.array('d')

    discrim_hists = {}
    c_values = range(0,11)
    for c in c_values:

        for flav in flavors: 
        #hist_name = hist_base_name + '_' + flav
            hist_name = flav + '_cVal' + str(c)
            discrim_hists[flav] = hist_getter(root_file, hist_name)

    
        xB_array, yB_array = plots_to_xy(discrim_hists['charm'], 
                                   discrim_hists['bottom'])

        
        

        xL_array, yL_array = plots_to_xy(discrim_hists['charm'],discrim_hists['light'])

                             
        
        rejAtEff_Cb = numpy.interp([eff],xB_array,yB_array)
        print rejAtEff_Cb

        rejAtEff_Cl = numpy.interp([eff],xL_array,yL_array)
        print rejAtEff_Cl

        
        rejB_array.append(rejAtEff_Cb)
        rejL_array.append(rejAtEff_Cl)
        c_array.append(c)
        
    
    

    c1 =  TCanvas("c1","title",300,300,600,600)
        

    rejPlotB = TGraph(len(rejB_array),c_array,rejB_array)
    rejPlotL = TGraph(len(rejL_array),c_array,rejL_array)

    rejPlotB.SetLineColor(9)
    rejPlotB.SetLineWidth(2)

    rejPlotL.SetLineColor(6)
    rejPlotL.SetLineWidth(2)

    rejPlotB.GetXaxis().SetTitle("c(l) value")
    rejPlotB.GetYaxis().SetTitle("rejection")

   #  rejPlotL.GetXaxis().SetTitle("c(l) value")
   # rejPlotL.GetYaxis().SetTitle("L rejection")

    rejPlotL.Draw('al')
    rejPlotB.Draw('l')

    leg = TLegend(0.8,0.8,0.95,0.95)
    leg.AddEntry(rejPlotB, "b rejection","l")
    leg.AddEntry(rejPlotL, "l rejection","l")
    leg.Draw()

    gPad.SetLogy()

    c1.Print(str(eff) + ".pdf")


    

if __name__ == '__main__': 
    
    root_file_name = sys.argv[1]
    
    eff_values =  [.1,.2,.3,.4,.5,.6,.7,.8,.9]
    for eff in eff_values:
               
        hist_plotter(eff,root_file_name)


