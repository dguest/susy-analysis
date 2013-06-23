#!/usr/bin/env python2.7
"""
Top level for mc / data stack plotting routines for the main analysis. 
"""
import argparse
from stop.stack import plot, draw
from os.path import isfile, join
from stop.stack import table
from tempfile import TemporaryFile
import yaml
import sys
import glob
from stop.stack.aggregator import HistDict
import re
from itertools import chain, product

def get_config(): 
    d = 'default: %(default)s'
    c = "with no argument is '%(const)s'"

    plotting_general = argparse.ArgumentParser(add_help=False)
    plotting_general.add_argument(
        'steering_file', help="created if it doesn't exist")
    plotting_general.add_argument(
        '--ext', help='plot extensions, ' + d, default='.pdf')
    plotting_general.add_argument('--filt')

    top_parser = argparse.ArgumentParser(description=__doc__)
    subs = top_parser.add_subparsers(dest='which')

    parser = subs.add_parser('mill', parents=[plotting_general])
    parser.add_argument(
        '--mode', choices={'histmill','kinematic_stat'}, 
        default='histmill', help='default: %(default)s')
    parser.add_argument('--scale', choices={'log','linear', 'both'}, 
                        default='linear', help=d)
    parser.add_argument(
        '-s','--signal-point', default='stop-150-90', 
        help="assumes <particle>-<something> type name, " + d)

    overlay_parser = subs.add_parser('tagger', parents=[plotting_general])
    overlay_parser.add_argument(
        '-s','--signal-point', default='stop-150-90', 
        help="assumes <particle>-<something> type name, " + d)
    overlay_parser.add_argument('-o', '--output-dir', 
                               help=d, default='tag-plots')

    single_parser = subs.add_parser('tagone', parents=[plotting_general])
    single_parser.add_argument('-p','--physics-type', required=True)
    single_parser.add_argument('-o', '--output-dir', 
                               help=d, default='tag-plots')

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
            'tagone':run_tagger_one_type}
    subs[args.which](args)

_overlay_cut = 'preselection'
def run_tagger_overlay(args): 
    with open(args.steering_file) as steering_yml: 
        config = yaml.load(steering_yml)
    hists_base = config['files']['hadd-hists']
    aggregates = glob.glob(
        join(hists_base,'histmill','baseline','aggregate.h5'))

    physics_set = {'Zjets','Wjets','ttbar','t'}
    if args.signal_point: 
        physics_set.add(args.signal_point)

    plots_dict = {}
    for agg_file in aggregates: 
        print 'loading {}'.format(agg_file)
        hists = HistDict(agg_file, args.filt, 
                         physics_set=physics_set, cut_set={_overlay_cut})
        plots_dict.update(hists)

    for jetn in xrange(3): 
        draw.tagger_overlay_plot_for_jet_number(
            plots_dict, jetn, args.signal_point, args.ext, args.output_dir)

def run_tagger_one_type(args): 
    with open(args.steering_file) as steering_yml: 
        config = yaml.load(steering_yml)
    hists_base = config['files']['hadd-hists']
    aggregates = glob.glob(
        join(hists_base,'histmill','baseline','aggregate.h5'))

    physics_set = {args.physics_type}

    plots_dict = {}
    for agg_file in aggregates: 
        print 'loading {}'.format(agg_file)
        hists = HistDict(agg_file, args.filt, 
                         physics_set=physics_set, cut_set={_overlay_cut})
        plots_dict.update(hists)

    for jetn in xrange(3): 
        draw.tagger_plot_for_jet_number(
            plots_dict, jetn, args.physics_type, args.ext, args.output_dir)

def run_plotmill(args): 
    with open(args.steering_file) as steering_yml: 
        config = yaml.load(steering_yml)
    hists_base = config['files']['hadd-hists']
    used_physics = config['backgrounds']['used'] + ['data']

    aggregates = glob.glob(
        join(hists_base,args.mode,'baseline','aggregate.h5'))
    
    plots_dict = {}
    for agg_file in aggregates: 
        print 'loading {}'.format(agg_file)
        hists = HistDict(agg_file,args.filt, physics_set=used_physics)
        plots_dict.update(hists)
            
    needed = get_signal_finder(args.signal_point)
    plots_dict = {k:v for k,v in plots_dict.iteritems() if needed(k)}
    plotting_info = {
        'lumi_fb': config['misc']['lumi_fb'], 
        'base_dir': config['files']['plots'], 
        'output_ext': args.ext, 
        'used_backgrounds': config['backgrounds']['used'], 
        }

    do_log = args.scale == 'log'
    plot.make_plots(plots_dict, plotting_info, log=do_log)
    if args.scale == 'both': 
        plot.make_plots(plots_dict, plotting_info, log=True)
        

if __name__ == '__main__': 
    run()
