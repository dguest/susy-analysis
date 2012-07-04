#!/usr/bin/env python2.7

from ROOT import TFile, gROOT, TH1D, TGraph, gPad
import random, sys, array

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

if __name__ == '__main__': 
    
    root_file_name = sys.argv[1]

    root_file = TFile(root_file_name)

   # hist_base_name = 'logCbNewTune'

    flavors = ['bottom','charm','light']

    discrim_hists = {}
    c_values = range(0,11)
    for c in c_values:

        for flav in flavors: 
        #hist_name = hist_base_name + '_' + flav
            hist_name = flav + 'cVal' + str(c)
            print hist_name
            discrim_hists[flav] = hist_getter(root_file, hist_name)

    
        x_array, y_array = plots_to_xy(discrim_hists['charm'], 
                                   discrim_hists['bottom'])
    the_graph = TGraph(len(y_array), x_array, y_array)
    the_graph.Draw('ap')
    gPad.SetLogy()

    raw_input('press enter')

