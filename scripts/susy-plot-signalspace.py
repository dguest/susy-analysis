#!/usr/bin/env python2.7
"""
reads in an optimum_cuts.pkl, produces signal significance vs sample plots
"""

import argparse, sys, cPickle
from os.path import join, split, isdir
import os
from collections import defaultdict
import ConfigParser
from stop.optimize.opttools import OptimaCache
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.ticker import LogFormatter, LogLocator
from matplotlib.colors import LogNorm, Normalize
import matplotlib as mpl

_rel_label = r'$\sigma_{\mathrm{SR} } / \sigma_{ \mathrm{opt} }$'

def run(): 
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('opt_cuts', nargs='+')
    parser.add_argument(
        '--plot_dir', nargs='?', 
        default='parameter_space_plots', 
        help='default: %(default)s')
    parser.add_argument('--baseline', nargs='?')
    parser.add_argument('-t', nargs='?', 
                        const='opt_cuts.cfg', 
                        default=None,
                        help='save textfile of cuts (default %(const)s)')
    args = parser.parse_args(sys.argv[1:])

    if len(args.opt_cuts) > 1: 
        run_multi(args)
    else:
        run_single(args)

def run_multi(args): 

    base_optima = None
    if args.baseline: 
        base_optima = OptimaCache(args.baseline)

    optima = OptimaCache(args.opt_cuts[0])

    if not isdir(args.plot_dir): 
        os.mkdir(args.plot_dir)

    # --- to shit with it, I'm going to do it the easy way 
    mlsp_vs_mstop_signif(optima, args.plot_dir)

    mstop, mlsp, opt = np.array(
        zip(*(get_mstop_mlsp_opt(*it) for it in optima.items())))
    diff = mstop - mlsp 
    diff_vs_stop = DiffVsStop()
    sig = np.array([o.significance for o in opt])
    if base_optima: 
        base_sig = np.array([o.significance for o in base_optima.values()])
        ratio = sig / base_sig
        diff_vs_stop.do_log = False
        diff_vs_stop.cb_label = _rel_label
        diff_vs_stop.plot(mstop, diff, ratio, 'opt-ratio.pdf')
    else: 
        diff_vs_stop.plot(mstop, diff, sig, 'opt-diff.pdf')

    a_signal = optima.values()[0]

    for cut in a_signal.cuts: 

        diff_vs_stop.do_log = False
        diff_vs_stop.cb_label = cut
        z_vals = [o.cuts[cut] for o in opt]
            
        if cut in ['mttop','leadingPt','met']: 
            z_vals = [z / 1000.0 for z in z_vals]
            diff_vs_stop.cb_label += ' [GeV]'
        diff_vs_stop.plot(mstop, diff, z_vals, '{}.pdf'.format(cut))

    if args.t: 
        cuts = ConfigParser.SafeConfigParser()
        cuts.optionxform = str
        for signame, opt in optima.iteritems(): 
            cuts.add_section(signame)
            for cut_name, cut_val in opt.cuts.iteritems():
                cuts.set(signame, cut_name, str(cut_val))
        with open(args.t,'w') as out_file: 
            cuts.write(out_file)

def run_single(args): 
    base_optima = None
    if args.baseline: 
        base_optima = OptimaCache(args.baseline)

    plotter = SRPlotter(args.opt_cuts)
    plotter.plot('opt-signal-regions.pdf')
    plotter.plot_sr('signal-regions.pdf')
    if args.baseline: 
        plotter.plot_ratio(args.baseline, 'signal-regions-vs-baseline.pdf')

class SRPlotter(object): 
    x_label = r'$m_{\tilde{t}}$ [GeV]'
    y_label = r'$m_{\tilde{t}} - m_{\chi}$ [GeV]'
    cb_label = r'$\frac{s}{\sqrt{s + b + (\sigma b)^2} }$'
    plot_dir = 'parameter_space_plots'
    def __init__(self, opt_cuts): 
        self.sr_dict = {f.split('.')[0]:OptimaCache(f) for f in opt_cuts}
        # hackish way to keep things ordered
        self.sr_order = [f.split('.')[0] for f in opt_cuts]

        self.best_plane = []
        if not isdir(self.plot_dir): 
            os.mkdir(self.plot_dir)
        self.sr_colors = {sr:color for sr, color in zip(self.sr_dict,'rgkw')}

        for signal_name in self.sr_dict.values()[0]:
            sig_opt = []
            for sr_name, optima in self.sr_dict.iteritems(): 
                sig_opt.append( (optima[signal_name].significance, sr_name) )
            sig_sort = sorted(sig_opt, reverse=True)
            best_sig, region = sig_sort[0]
            second_best, second_region = sig_sort[1]
            sig_dict = {name: sig for sig, name in sig_sort}
            
            self.best_plane.append( (signal_name, best_sig, region ) )


    def plot(self, save_name):
    
        points = []
        for sig_name, value, region in self.best_plane:
            stop_mass, lsp_mass = [float(x) for x in sig_name.split('-')]
            points.append((stop_mass, lsp_mass, value))

        mstop, mlsp, sig = np.array(zip(*points))
        mass_diff = mstop - mlsp

        fig = plt.figure(figsize=(6,4.5))
        ax = fig.add_subplot(1,1,1)

        sc = ax.scatter(mstop, mass_diff, 
                        c=sig, 
                        norm=LogNorm(), 
                        s=80)
        
        log_locate = LogLocator(10, np.arange(0.1,1,0.1))
        log_format = LogFormatter(10)
        cb = plt.colorbar(sc, format=log_format, ticks=log_locate)
    
        ax.set_xlabel(self.x_label, x=0.98, ha='right')
        ax.set_ylabel(self.y_label, y=0.98, va='top')
        cb.set_label(self.cb_label, y=0.98, va='top')

        fig.savefig(join(self.plot_dir,save_name),bbox_inches='tight')
        plt.close(fig)

    def plot_ratio(self, baseline_name, save_name):
    
        baseline = OptimaCache(baseline_name)

        points = []
        for sig_name, value, region in self.best_plane:
            stop_mass, lsp_mass = [float(x) for x in sig_name.split('-')]
            points.append((stop_mass, lsp_mass, value))

        base_points = []
        for sig_name, base_opt in baseline.iteritems(): 
            value = base_opt.significance
            stop_mass, lsp_mass = [float(x) for x in sig_name.split('-')]
            base_points.append((stop_mass, lsp_mass, value))

        mstop, mlsp, sig = np.array(zip(*points))
        mass_diff = mstop - mlsp

        base_mstop, base_lsp, base_sig = np.array(zip(*base_points))
        reldif =  ( 
            (np.sum(mstop - base_mstop)**2 + np.sum(mlsp - base_lsp)**2) / 
            (np.sum(mstop + base_mstop)**2 + np.sum(mlsp + base_lsp)**2) )
        if reldif > 1e-4: 
            raise StandardError('you fucked up: reldif {}'.format(reldif))

        rel_sig = sig / base_sig

        fig = plt.figure(figsize=(6,4.5))
        ax = fig.add_subplot(1,1,1)

        sc = ax.scatter(mstop, mass_diff, 
                        c=rel_sig, 
                        s=80)
        
        cb = plt.colorbar(sc)
    
        ax.set_xlabel(self.x_label, x=0.98, ha='right')
        ax.set_ylabel(self.y_label, y=0.98, va='top')
        cb.set_label(_rel_label, y=0.98, va='top')

        fig.savefig(join(self.plot_dir,save_name),bbox_inches='tight')
        plt.close(fig)

    def plot_sr(self, save_name):
    
        points_dict = {sr:[] for sr in self.sr_dict}
        for sig_name, value, region in self.best_plane:
            stop_mass, lsp_mass = [float(x) for x in sig_name.split('-')]
            points_dict[region].append( (stop_mass, lsp_mass, value))

        fig = plt.figure(figsize=(6,4.5))
        ax = fig.add_subplot(1,1,1)

        for sr_name in self.sr_order: 
            sr_points = points_dict[sr_name]
        # for sr_name, sr_points in points_dict.iteritems(): 
            mstop, mlsp, sig = np.array(zip(*sr_points))
            mass_diff = mstop - mlsp
                
            sc = ax.scatter(mstop, mass_diff, 
                            c=self.sr_colors[sr_name], 
                            label=sr_name, 
                            s=80)#, cmap=cm)

        ax.set_xlabel(self.x_label, x=0.98, ha='right')
        ax.set_ylabel(self.y_label, y=0.98, va='top')

        leg = ax.legend(scatterpoints=1, frameon=False)

        fig.savefig(join(self.plot_dir,save_name),bbox_inches='tight')
        plt.close(fig)
        
    def _plot_points(self, points, ax, color):
        mstop, mlsp, sig = np.array(zip(*points))
        mass_diff = mstop - mlsp
                
        sc = ax.scatter(mstop, mass_diff, 
                        c=color, 
                        s=80)#, cmap=cm)
        return sc


class DiffVsStop(object): 
    x_label = r'$m_{\tilde{t}}$ [GeV]'
    y_label = r'$m_{\tilde{t}} - m_{\chi}$ [GeV]'
    def __init__(self, cb_label=r'$\frac{s}{\sqrt{s + b + (\sigma b)^2} }$', 
                 plot_dir='parameter_space_plots'):
        self.cb_label = cb_label
        self.plot_dir = plot_dir
        self.do_log = True
        if not isdir(plot_dir): 
            os.mkdir(plot_dir)
    
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

    # print lsp, stop, z

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

if __name__ == '__main__': 
    run()
