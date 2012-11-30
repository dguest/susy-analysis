#!/usr/bin/env python2.7

from stop.optimize.opttools import combine, select_signals, HistBuilder
from stop.optimize import opttools
from stop.optimize import draw
from stop import hists

import h5py 
import numpy as np
from os.path import join, basename, splitext, isdir, isfile
import glob, sys, cPickle, os, re

import matplotlib.pyplot as plt
import matplotlib as mp

import ConfigParser, argparse, copy




GeV = 1e3    

def run(): 
    parser = argparse.ArgumentParser()
    parser.add_argument('config', nargs='?', help='default: %(default)s', 
                        default='optimize.cfg')

    args = parser.parse_args(sys.argv[1:])
    
    config_parser = ConfigParser.SafeConfigParser()
    config_parser.read([args.config])
    file_config = dict(config_parser.items('files'))
    selection = dict(config_parser.items('selection'))
    tune = dict(config_parser.items('tune'))
    
    whiskey_path = file_config['ntuples_location']
    distillates = glob.glob(join(whiskey_path, '*.root'))
    if 'signals' in selection: 
        distillates = select_signals(
            distillates, selection['signals'].split())

    with open(file_config['meta']) as pkl: 
        meta = cPickle.load(pkl)

    cache = 'cache'
    if 'cache' in file_config: 
        cache = file_config['cache']

    if not isdir(cache): 
        os.mkdir(cache)
    
    sys_factor = float(tune['sys_factor'])
    lumi = float(tune['lumi'])



    base_cut = 'vxp_good'
    all_cuts = 'vxp_good+lepton_veto'
    # cut = 'ctag_mainz'

    builder = HistBuilder(base_cut=base_cut)

    # FIXME: hack for now, change with more data
    if config_parser.has_section('limits'): 
        for name, value in config_parser.items('limits'): 
            builder.special_limits[name] = float(value)
    
    h5_cache = [builder.build(d) for d in distillates]
    baseline_cache = join(cache, 'baseline_cache.pkl')


    # optimize_fuckshit(h5_cache, meta, 
    #                   all_cuts=all_cuts, tune=tune)
    

    # this should loop over signals
    for signal in selection['signals'].split(): 
        baseline = opttools.get_baseline(
            baseline_cache, distillates, meta, sys_factor, 
            lumi=lumi, signal=signal)

        optimize_signal(signal, 
                        h5_cache, meta, 
                        all_cuts=all_cuts, tune=tune, baseline=baseline)



def optimize_signal(signal, h5_cache, meta, all_cuts, tune, plot_dir='plots', 
                    baseline=None): 

    sys_factor = float(tune['sys_factor'])
    lumi = float(tune['lumi'])

    hyperstash = 'stash.h5'

    sum_signal, sum_background = get_signal_and_background( 
        hyperstash, signal, all_cuts, h5_cache, meta)        

    sum_background *= lumi
    sum_signal *= lumi

    if 'j1Bu' in [a.name for a in sum_signal.axes]: 
        sum_signal.reduce('j1Bu')
    if 'j1Bu' in [a.name for a in sum_background.axes]: 
        sum_background.reduce('j1Bu')

    for axis in sum_background.axes: 
        print 'integrating {}'.format(axis.name)
        reverse = not bool(axis.name == 'j1Bu') 

        sum_signal.integrate(axis.name, reverse=reverse)
        sum_background.integrate(axis.name, reverse=reverse)


    j1_taggers = ['j1Bu']
    j2_taggers = ['j2Cu','j2Cb']
    j3_taggers = ['j3Cu','j3Cb']
    tagger_vars =  j3_taggers + j2_taggers + j1_taggers
    kinematic_vars = ['met','leadingPt']
    oddbals = ['leadingPt','j2Cu']
    cuts_to_use = kinematic_vars + j2_taggers + j3_taggers

    possible_cuts = [a.name for a in sum_signal.axes + sum_background.axes]

    cuts_to_use = [c for c in cuts_to_use if c in possible_cuts]

    make_cutflow_plots(sum_signal, sum_background, signal, sys_factor, 
                       cuts_to_use, plot_dir, baseline)

    make_other_plots(sum_signal, sum_background, signal, 
                     sys_factor, cuts_to_use, put_where='plots', 
                     baseline=baseline)

    # compare_signal_cuts(hyperstash, all_cuts, h5_cache, meta)

def optimize_fuckshit(h5_cache, meta, all_cuts, tune, 
                      plot_dir='plots'): 

    sys_factor = float(tune['sys_factor'])
    lumi = float(tune['lumi'])

    hyperstash = 'stash.h5'

    compare_signal_cuts(hyperstash, all_cuts, h5_cache, meta, lumi)


def compare_signal_cuts(hyperstash, all_cuts, h5_cache, meta, lumi, 
                        signals=['175-100','175-125','175-170'], 
                        put_where='plots'): 
    if not isdir(put_where):
        os.mkdir(put_where)

    sys_factor = 0.3

    for var in ['j2Cu','j2Cb','met','leadingPt','j3Cu','j3Cb']: 
        print 'making plots for {}'.format(var)
        gen = optimum_cuts_generator(
            hyperstash, all_cuts, var, h5_cache, 
            signals, meta, lumi, sys_factor)

        fig = plt.figure(figsize=(6,4.5))
        ax = fig.add_subplot(1,1,1)
        for array, extent, name in gen: 
            var_name, extent, units = draw.new_name_ext(var, extent)
            print 'adding {}'.format(name)
            x_pts, y_pts = hists.Hist1d(array, extent).get_xy_pts()
            handle, = ax.plot(x_pts, y_pts, label=name)
        ax.legend().get_frame().set_visible(False)
        xlab = '{v} [{u}]' if units else '{v}'
        ax.set_xlabel(xlab.format(v=var_name,u=units), x=0.98, ha='right')

        fom_template = (
            r'$\sigma \equiv '
            r'\frac{{s}}{{\sqrt{{s + b + ({:.2f} \times b)^2}} }}$')
        fom_label = fom_template.format(sys_factor)
        ax.set_ylabel(fom_label, y=0.98, va='top')

        out_name = join(put_where, 'sigcomp_{}.pdf'.format(var))
        fig.savefig(out_name, bbox_inches='tight')
            

def optimum_cuts_generator(hyperstash, all_cuts, variable, h5_cache, 
                           signals, meta, lumi, sys_factor=0.3): 
    for sig in signals: 
        sighist, bkhist = get_signal_and_background(
            hyperstash, sig, all_cuts, h5_cache, meta)

        if 'j1Bu' in [a.name for a in sighist.axes + bkhist.axes]: 
            sighist.reduce('j1Bu')
            bkhist.reduce('j1Bu')

        ax_names = [a.name for a in sighist.axes]

        for ax in ax_names: 
            print 'integrating {}'.format(ax)
            reverse = not bool(ax == 'j1Bu') 
            sighist.integrate(ax, reverse=reverse)
            bkhist.integrate(ax, reverse=reverse)

        sighist *= lumi
        bkhist *= lumi
            
        assert variable in ax_names
        significance = opttools.compute_significance(
            sighist, bkhist, kept_axes=ax_names, 
            sys_factor=sys_factor)
        array, extent = significance.project_1d(variable)
        yield array, extent, sig
    

def get_signal_and_background(hyperstash, signal, all_cuts, h5_cache, meta): 
    sum_signal = None
    sum_background = None
    if isfile(hyperstash): 
        with h5py.File(hyperstash) as hdf: 
            if signal in hdf: 
                sum_signal = hists.HistNd(hdf[signal])
            if 'background' in hdf: 
                sum_background = hists.HistNd(hdf['background'])
                
    if not sum_signal: 
        def sig_iter(h5_cache): 
            signal_finder = 'directCC_{}_{}'.format(*signal.split('-'))
            for f in h5_cache: 
                if signal_finder in f: 
                    yield f

        sum_signal, throw_away = combine(
            sig_iter(h5_cache), meta, cut=all_cuts, signal=signal)

        with h5py.File(hyperstash,'a') as hdf: 
            sum_signal.write_to(hdf, signal)
    if not sum_background: 
        def bkg_iter(h5_cache): 
            sig_finder = re.compile('directCC_[0-9]+_[0-9]+')
            for f in h5_cache:
                if not sig_finder.search(f):
                    yield f

        throw_away, sum_background = combine(
            bkg_iter(h5_cache), meta, cut=all_cuts, signal=signal)

        with h5py.File(hyperstash, 'a') as hdf: 
            sum_background.write_to(hdf, 'background')

    return sum_signal, sum_background


def make_cutflow_plots(sum_signal, sum_background, signal, sys_factor, 
                       cuts_to_use, plot_dir='plots' , baseline=None): 


    if not isdir(plot_dir): 
        os.mkdir(plot_dir)

    sub_plot_dir = join(plot_dir,signal)
    if not isdir(sub_plot_dir): 
        os.mkdir(sub_plot_dir)

    cf = draw.CutflowOptimum(sum_signal, sum_background, 
                             cuts=cuts_to_use)
    cf.compute(sys_factor)
    cf.baseline = baseline

    eff_dict = cf.get_efficiencies()

    with open(join(sub_plot_dir,'efficiencies.txt'),'w') as out: 
        cutlen = max(len(c) for c in eff_dict.keys())
        for cut, (eff, bkg) in eff_dict.iteritems(): 
            out.write('{:{}} {: 10.3} {: 10.3}\n'.format(
                    cut, cutlen, eff, bkg))

    cf.set_log()
    cf.draw_normalized(join(sub_plot_dir,'cutflow.pdf'))

def make_other_plots(sum_signal, sum_background, signal, 
                     sys_factor, cuts_to_use, put_where='plots', 
                     baseline=None): 

    print 'computing significance'
    sig_hist = opttools.compute_significance(
        sum_signal, sum_background, cuts_to_use, 
        sys_factor=sys_factor)

    fom_template = (
        r'$\sigma \equiv'
        r'\frac{{s}}{{\sqrt{{s + b + ({:.2f} \times b)^2}} }}$')
    fom_label = fom_template.format(sys_factor)

    sub_plot_dir = join(put_where,signal)
    if not isdir(sub_plot_dir): 
        os.mkdir(sub_plot_dir)

    # print 'plot 2d'
    # draw.plot_2d(sig_hist, fom_label, put_where=sub_plot_dir)

    print 'plotting'
    draw.plot_1d(sig_hist, fom_label, put_where=sub_plot_dir, 
                 baseline=baseline, signal_name=signal, 
                 signal_hist=sum_signal, background_hist=sum_background)


if __name__ == '__main__': 
    run()
