#!/usr/bin/env python2.7
"""
Top level for mc / data stack plotting routines for the main analysis. 
"""
import argparse
from stop.stack import plot, draw
from os.path import isfile, join, isdir
from stop.stack import table
from tempfile import TemporaryFile
import yaml
import sys
import os
import glob
from stop.stack.aggregator import HistDict
from stop.stack.draw import h2_from_hn
import re
from itertools import chain, product
from collections import defaultdict

def get_config(): 
    d = 'default: %(default)s'
    c = "with no argument is '%(const)s'"

    plotting_general = argparse.ArgumentParser(add_help=False)
    plotting_general.add_argument('aggregate')
    plotting_general.add_argument(
        '--ext', help='plot extensions, ' + d, default='.pdf')
    plotting_general.add_argument(
        '--filt', type=_filt_converter, 
        help='exactly what is filtered depends on the routine')
    plotting_general.add_argument('-o', '--output-dir', 
                                  help=d, default='plots')
    signal_point = argparse.ArgumentParser(add_help=False)
    signal_point.add_argument(
        '-s','--signal-point', default='stop-150-90', 
        help="assumes <particle>-<something> type name, " + d)

    top_parser = argparse.ArgumentParser(description=__doc__)
    subs = top_parser.add_subparsers(dest='which')

    parser = subs.add_parser('mill', 
                             parents=[plotting_general, signal_point])
    parser.add_argument('steering_file', nargs='?')
    parser.add_argument('--scale', choices={'log','linear', 'both'}, 
                        default='log', help=d)

    overlay_parser = subs.add_parser('tagger', 
                                     parents=[plotting_general, signal_point])

    single_parser = subs.add_parser('tagone', parents=[plotting_general])
    single_parser.add_argument('-p','--physics-type', required=True)

    kinematic_parser = subs.add_parser(
        'kin', parents=[plotting_general])
    phys_type = kinematic_parser.add_mutually_exclusive_group(required=True)
    phys_type.add_argument('-s','--signal-point', nargs='?', 
                           const='stop-200-125')
    phys_type.add_argument('-b','--background', action='store_true')
    phys_type.add_argument('-r','--s-over-b', nargs='?', 
                           const='stop-200-125')
    phys_type.add_argument('--all', nargs='?', 
                           const='stop-200-125')
    
    args = top_parser.parse_args(sys.argv[1:])
    return args

def get_signal_finder(signal_point): 
    if signal_point: 
        signal_head = signal_point.split('-')[0]
        def needed(tup): 
            phys = tup[0]
            if not phys.startswith(signal_head): 
                return True
            return phys == signal_point
    else: 
        def needed(tup): 
            return True
    return needed

def run(): 
    args = get_config()
    subs = {'mill':run_plotmill, 'tagger':run_tagger_overlay, 
            'tagone':run_tagger_one_type, 'kin':run_kinematic_plot}
    subs[args.which](args)

def run_kinematic_plot(args): 
    """
    Plots S, B, S/B for values of kinematic cuts in the plane. 
    """
    from stop.runtypes import marks_types
    bg_set = set(marks_types)
    phys_set = set()
    if args.background or args.s_over_b or args.all: 
        phys_set |= bg_set

    sp_list = filter(None, [args.s_over_b, args.signal_point, args.all])
    if sp_list: 
        signal_point = sp_list[0]
        phys_set.add(signal_point)
    else: 
        signal_point = ''

    region_set = None
    if args.filt: 
        region_set = { args.filt.replace('/','-') } # TODO: make less hackish
    hists = HistDict(args.aggregate, 
                     filt='kinematics', 
                     physics_set=phys_set, 
                     cut_set=region_set, 
                     )
    hists_by_region = defaultdict(dict)
    for (phys, var, region), hist in hists.iteritems(): 
        hists_by_region[region][phys,var] = hist
    out_args = dict(odir=args.output_dir, ext=args.ext)
    for region, reg_hists in hists_by_region.iteritems(): 
        _plot_region_kinematics(
            region, reg_hists, signal_point, bg_set, **out_args)
        if args.all: 
            print 'plotting bg'
            _plot_region_kinematics(
                region, reg_hists, '', bg_set, **out_args)
            print 'plotting signal'
            _plot_region_kinematics(
                region, reg_hists, signal_point, set(), **out_args)
            

def _plot_region_kinematics(region, hists, signal_point, bg_set, 
                            odir='plots', ext='.pdf'): 
    bg_hist = None
    sig_hist = None
    for (phys, var), hist in hists.iteritems(): 
        if phys in bg_set: 
            if not bg_hist: 
                bg_hist = hist
            else: 
                bg_hist += hist
        elif phys == signal_point: 
            if sig_hist: 
                phy_keys = ', '.join([k[0] for k in hists])
                raise StandardError(
                    'tried to replace {} with {}, keys: {}'.format(
                        signal_point, phys, phy_keys))
            sig_hist = hist

    def integrate(hist, reverse=True): 
        int_hist = hist['met'].integrate(reverse=reverse)
        return int_hist['leadingJetPt'].integrate(reverse=reverse)

    if bg_hist and sig_hist: 
        num = integrate(sig_hist)
        denom = integrate(bg_hist)
        the_hist = num / denom
        meth = '{}-over-bg'.format(signal_point)
        save_args = dict(log=False, vrange=(0.2, 1.1))
        cb_label = '{} / BG'.format(signal_point)
    elif sig_hist and not bg_hist: 
        the_hist = integrate(sig_hist)
        meth = signal_point
        save_args = dict(log=True)
        cb_label = signal_point
    elif bg_hist and not sig_hist: 
        the_hist = integrate(bg_hist)
        meth = 'bg'
        save_args = dict(log=True)
        cb_label = 'Background'
    the_plot = h2_from_hn(the_hist)
    the_plot.cb_label = cb_label
    if not isdir(odir): 
        os.mkdir(odir)
        
    the_plot.save('{}/{}-{}{}'.format(odir, meth, region,ext), **save_args)

def _filt_converter(typed_path): 
    return typed_path.replace('-','/')

_overlay_cut = 'preselection'
def run_tagger_overlay(args): 
    aggregates = [args.aggregate]

    physics_set = {'Zjets','Wjets','ttbar','t'}
    if args.signal_point: 
        physics_set.add(args.signal_point)

    plots_dict = {}
    for agg_file in aggregates: 
        print 'loading {}'.format(agg_file)
        hists = HistDict(agg_file, args.filt, 
                         physics_set=physics_set, cut_set={_overlay_cut})
        plots_dict.update(hists)

    for jetn in xrange(4): 
        draw.tagger_overlay_plot_for_jet_number(
            plots_dict, jetn, args.signal_point, args.ext, args.output_dir)

def run_tagger_one_type(args): 
    aggregates = [args.aggregate]

    physics_set = {args.physics_type}

    plots_dict = {}
    for agg_file in aggregates: 
        print 'loading {}'.format(agg_file)
        hists = HistDict(agg_file, args.filt, 
                         physics_set=physics_set, cut_set={_overlay_cut})
        plots_dict.update(hists)

    for jetn in xrange(4): 
        draw.tagger_plot_for_jet_number(
            plots_dict, jetn, args.physics_type, args.ext, args.output_dir)


def run_plotmill(args): 
    config = _get_config_info(args.steering_file)
    aggregates = [args.aggregate]
    used_physics = config['backgrounds']['used'] + ['data']
    if args.signal_point: 
        used_physics.append(args.signal_point)
    plots_dict = {}
    for agg_file in aggregates: 
        print 'loading {}'.format(agg_file)
        hists = HistDict(agg_file,args.filt, physics_set=used_physics)
        plots_dict.update(hists)
            
    needed = get_signal_finder(args.signal_point)
    plots_dict = {k:v for k,v in plots_dict.iteritems() if needed(k)}
    plotting_info = {
        'lumi_fb': config['misc']['lumi_fb'], 
        'base_dir': args.output_dir, 
        'output_ext': args.ext, 
        'used_backgrounds': config['backgrounds']['used'], 
        }

    do_log = args.scale == 'log'
    plot.make_plots(plots_dict, plotting_info, log=do_log)
    if args.scale == 'both': 
        plot.make_plots(plots_dict, plotting_info, log=True)
        
def _get_config_info(steering_file): 
    if not steering_file: 
        from stop.runtypes import marks_types
        config = {
            'misc': { 'lumi_fb': 21 }, 
            'backgrounds': {'used': marks_types.keys() }, 
            }
        return config
    else: 
        with open(steering_file) as steering_yml: 
            config = yaml.load(steering_yml)
        return config
        


if __name__ == '__main__': 
    run()
