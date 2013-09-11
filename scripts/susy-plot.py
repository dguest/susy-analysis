#!/usr/bin/env python2.7
"""
Top level for mc / data stack plotting routines for the main analysis. 
"""
import argparse
from stop.stack import plot, draw
import yaml
import sys
from stop.stack.aggregator import HistDict
from stop.stack.kinplt import run_kinematic_plot
from stop.bullshit import make_dir_if_none
import re
import os

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
    kinematic_parser.add_argument('--sys-factor', type=float, default=0)
    kinematic_parser.add_argument('-z', '--fix-z-range', action='store_true')

    sr_counts = subs.add_parser('counts', 
                                parents=[plotting_general, signal_point])
    sr_counts.add_argument('fits_file')
    sr_counts.add_argument('cuts_file')
    sr_counts.add_argument('--max-val', type=float)
    
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
            'tagone':run_tagger_one_type, 'kin':run_kinematic_plot, 
            'counts':plot_counts}
    subs[args.which](args)


def plot_counts(args): 
    if args.fits_file: 
        with open(args.fits_file) as fits: 
            all_configs = yaml.load(fits)
    else: 
        from stop.runtypes import marks_types
        all_configs = {
            'blork': { 
                'backgrounds':list(marks_types), 
                'control_regions':['Znunu0', 'Wenu0', 'Wmunu0', 'ttbar0'], 
                'signal_region': 'SR0'
                }
            }
    phys_set = set(['data'])
    for config in all_configs.values(): 
        phys_set |= set(config['backgrounds'])

    if args.signal_point: 
        phys_set.add(args.signal_point)

    cuts = None
    if args.cuts_file: 
        with open(args.cuts_file) as cuts_yml: 
            cuts = {c['region_key']:c for c in yaml.load(cuts_yml)}

    hists = HistDict(args.aggregate, 
                     filt='kinematic', 
                     physics_set=phys_set, 
                     )

    for config_name, config in all_configs.iteritems(): 
        if not config_name in cuts: 
            continue
        sr_cuts = cuts[config_name]
        save_dir = 'reg-counts'
        make_dir_if_none(save_dir)
        save_name = os.path.join(save_dir, config_name + args.ext)
        _plot_config(config, hists, sr_cuts, args.signal_point, save_name, 
                     args.max_val)

def _plot_config(config, hists, cuts, sig_pt, save_name, max_val): 
    
    from matplotlib.figure import Figure
    from matplotlib.backends.backend_agg import FigureCanvasAgg as FigCanvas
    import numpy as np

    from stop.style import type_dict
    from stop.stattest import poisson_interval

    backgrounds = config['backgrounds']
    signal_region = config['signal_region']
    variable = 'kinematics'
    cr_cuts = dict(met=150, leading_jet_pt=150)

    def get_count(hist, cuts): 
        inf = float('inf')
        met_cut = cuts['met']*1e3
        ljpt_cut = cuts['leading_jet_pt']*1e3
        return hist['leadingJetPt'].slice(ljpt_cut, inf)['met'].slice(
            met_cut, inf)

    figure = Figure(figsize=(9,8))
    canvas = FigCanvas(figure)
    ax = figure.add_subplot(1,1,1)
    control_regions = config['control_regions']
    n = len(control_regions) + 1
    ind = np.arange(n)    # the x locations for the groups
    width = 0.80
    bottoms = np.zeros(n)
    leg_entries = []
    for bg in config['backgrounds']: 
        y_vals = np.zeros(n)
        sr_count = get_count(hists[bg, variable, signal_region], cuts)
        y_vals[0] = sr_count
        this_color = type_dict[bg].color
        for reg_n, cr in enumerate(control_regions,1): 
            y_vals[reg_n] = get_count(hists[bg, variable, cr], cr_cuts)
        bar = ax.bar(ind, y_vals, width, bottom=bottoms, color=this_color)
        bottoms += y_vals
        leg_entries.append( (bar[0], bg))

    if sig_pt: 
        y_vals = np.zeros(n)
        sr_count = get_count(hists[sig_pt, variable, signal_region], cuts)
        y_vals[0] = sr_count
        color = 'black'
        for reg_n, cr in enumerate(control_regions,1): 
            y_vals[reg_n] = get_count(hists[sig_pt, variable, cr], cr_cuts)
        bar = ax.bar(ind, y_vals, width, color='black', fill=False, 
                     linestyle='dashed')
        leg_entries.append( (bar[0], sig_pt))
    
    d_vals = np.zeros(n - 1)
    d_x = np.arange(1,n)
    for reg_n, cr in enumerate(control_regions): 
        d_vals[reg_n] = get_count(hists['data',variable,cr], cr_cuts)
    low, high = poisson_interval(d_vals)
    y_errors = [high - d_vals, d_vals - low]
    ax.errorbar(d_x + width/2, d_vals, fmt='ok', xerr=width/2, yerr=y_errors)

    ax.set_xticks(ind + width/2)
    ax.set_xticklabels([signal_region] + control_regions)
    if max_val: 
        ax.set_ylim(0, max_val)
    leg = ax.legend(*zip(*reversed(leg_entries)), 
                     loc='upper left', framealpha=0.0)
    canvas.print_figure(save_name, bbox_inches='tight')


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
