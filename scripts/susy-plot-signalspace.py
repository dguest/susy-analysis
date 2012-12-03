#!/usr/bin/env python2.7
"""
reads in an optimum_cuts.pkl, produces signal significance vs sample plots
"""

import argparse, sys, cPickle
from os.path import join, split, isdir
import os
from stop.optimize.opttools import OptimaCache
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.ticker import LogFormatter, LogLocator
from matplotlib.colors import LogNorm, Normalize
import matplotlib as mpl

def run(): 
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('opt_cuts')
    parser.add_argument(
        'plot_dir', nargs='?', 
        default='parameter_space_plots', 
        help='default: %(default)s')
    args = parser.parse_args(sys.argv[1:])
    
    optima = OptimaCache(args.opt_cuts)

    if not isdir(args.plot_dir): 
        os.mkdir(args.plot_dir)

    # --- to shit with it, I'm going to do it the easy way 
    mlsp_vs_mstop_signif(optima, args.plot_dir)

    mstop, mlsp, opt = np.array(
        zip(*(get_mstop_mlsp_opt(*it) for it in optima.items())))
    diff = mstop - mlsp 
    diff_vs_lsp = DiffVsLsp()
    # diff_vs_lsp.do_log = False
    sig = [o.significance for o in opt]
    diff_vs_lsp.plot(mstop, diff, sig, 'opt-diff.pdf')

    a_signal = optima.values()[0]
    for cut in a_signal.cuts: 
            
        diff_vs_lsp.do_log = False
        diff_vs_lsp.cb_label = cut
        z_vals = [o.cuts[cut] for o in opt]
        # for zinx in xrange(len(z_vals)): 
        #     if z_vals[zinx] == float('-inf'): 
        #         z_vals[zinx] = mpl.cm.get_cmap()(1.0)
            
        if cut in ['mttop','leadingPt','met']: 
            z_vals = [z / 1000.0 for z in z_vals]
            diff_vs_lsp.cb_label += ' [GeV]'
        diff_vs_lsp.plot(mstop, diff, z_vals, '{}.pdf'.format(cut))
    

class DiffVsLsp(object): 
    x_label = r'$m_{\chi}$ [GeV]'
    y_label = r'$m_{\tilde{t}} - m_{\chi}$ [GeV]'
    def __init__(self, cb_label=r'$\frac{s}{\sqrt{s + b + (\sigma b)^2} }$', 
                 plot_dir='parameter_space_plots'):
        self.cb_label = cb_label
        self.plot_dir = plot_dir
        self.do_log = True
    def plot(self, x, y, z, save_name): 
        fig = plt.figure(figsize=(6,4.5))
        ax = fig.add_subplot(1,1,1)
        norm = LogNorm() if self.do_log else Normalize()

        # FIXME: I can't get the -inf points to draw... 
        sc = ax.scatter(x, y, c=z, norm=norm, s=80)#, cmap=cm)
        # ...using this hack instead
        neginfs = np.isneginf(z)
        if np.any(neginfs): 
            ax.scatter(x[neginfs], y[neginfs], c='w', s=80)
        infs = np.isposinf(z)
        if np.any(infs): 
            ax.scatter(x[infs], y[infs], c='magenta', s=80)
        
        if self.do_log: 
            log_locate = LogLocator(10, np.arange(0.1,1,0.1))
            log_format = LogFormatter(10)
            cb = plt.colorbar(sc, format=log_format, ticks=log_locate)
        else: 
            cb = plt.colorbar(sc)

        ax.set_xlabel(self.x_label, x=0.98, ha='right')
        ax.set_ylabel(self.y_label, y=0.98, va='top')
        cb.set_label(self.cb_label, y=0.98, va='top')
        fig.savefig(join(self.plot_dir,save_name),bbox_inches='tight')
        plt.close(fig)
        

def mlsp_vs_mstop_signif(optima, plot_dir): 
    stop, lsp, z = zip(*(get_mstop_mlsp_sig(*it) for it in optima.items()))

    fig = plt.figure(figsize=(6,4.5))
    ax = fig.add_subplot(1,1,1)
    sc = ax.scatter(lsp, stop, c=z, norm=LogNorm())

    print lsp, stop, z

    log_locate = LogLocator(10, np.arange(0.1,1,0.1))
    log_format = LogFormatter(10)
    cb = plt.colorbar(sc, format=log_format, ticks=log_locate)

    ax.set_xlabel(r'$m_{\chi}$ [GeV]', x=0.98, ha='right')
    ax.set_ylabel(r'$m_{\tilde{t}}$ [GeV]', y=0.98, va='top')
    cb.set_label(r'$\frac{s}{\sqrt{s + b + (\sigma b)^2} }$', y=0.98, va='top')
    fig.savefig(join(plot_dir,'optimum.pdf'),bbox_inches='tight')
    plt.close(fig)


def get_mstop_mlsp_sig(name, optimum): 
    stop_mass, lsp_mass = [float(x) for x in name.split('-')]
    return stop_mass, lsp_mass, optimum.significance

def get_mstop_mlsp_opt(name, optimum): 
    stop_mass, lsp_mass = [float(x) for x in name.split('-')]
    return stop_mass, lsp_mass, optimum

def get_bullshit(optima): 
    """
    this function may need to be revisited if scatter doesn't work 
    for some asshole
    """
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


if __name__ == '__main__': 
    run()
