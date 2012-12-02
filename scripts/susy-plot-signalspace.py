#!/usr/bin/env python2.7
"""
reads in an optimum_cuts.pkl, produces signal significance vs sample plots
"""

import argparse, sys, cPickle
from stop.optimize.opttools import OptimaCache
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.ticker import LogFormatter, LogLocator
from matplotlib.colors import LogNorm

def run(): 
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('opt_cuts')
    args = parser.parse_args(sys.argv[1:])
    
    optima = OptimaCache(args.opt_cuts)

    stop_masses = []
    lsp_masses = []

    for cut_name, optimum in optima.items(): 
        stop_mass, lsp_mass = [float(x) for x in cut_name.split('-')]
        stop_masses.append(stop_mass)
        lsp_masses.append(lsp_mass)
        
    sorted_stop = np.unique(stop_masses)
    stop_mass_range = (sorted_stop[0], sorted_stop[-1])
    stop_inc = np.diff(sorted_stop).min()
    
    sorted_lsp = np.unique(lsp_masses)
    lsp_mass_range = (sorted_lsp[0], sorted_lsp[-1])
    lsp_inc = np.diff(sorted_lsp).min()
    # print stop_mass_range, stop_inc 
    # print lsp_mass_range, lsp_inc

    plot_lsp_range = (lsp_mass_range[0] - lsp_inc / 2, 
                      lsp_mass_range[1] + lsp_inc / 2)
    plot_stop_range = (stop_mass_range[0] - stop_inc / 2, 
                       stop_mass_range[1] + stop_inc / 2)

    # --- to shit with it, I'm going to do it the easy way 

    x, y, z = zip(*(get_xyz(*it) for it in optima.items()))

    fig = plt.figure(figsize=(6,4.5))
    ax = fig.add_subplot(1,1,1)
    sc = ax.scatter(x, y, c=z, norm=LogNorm())

    log_locate = LogLocator(10, np.arange(0.1,1,0.1))
    log_format = LogFormatter(10)#, labelOnlyBase=False)
    cb = plt.colorbar(sc, format=log_format, ticks=log_locate)


    ax.set_xlabel(r'$m_{\tilde{t}}$ [GeV]', x=0.98, ha='right')
    ax.set_ylabel(r'$m_{\chi}$ [GeV]', y=0.98, va='top')
    cb.set_label(r'$\frac{s}{\sqrt{s + b + (\sigma b)^2} }$', y=0.98, va='top')
    fig.savefig('testshit.pdf',bbox_inches='tight')

def get_xyz(name, optimum): 
    stop_mass, lsp_mass = [float(x) for x in name.split('-')]
    return stop_mass, lsp_mass, optimum.significance

if __name__ == '__main__': 
    run()
