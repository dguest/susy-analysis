#!/usr/bin/env python2.7
"""
Top level for mc / data stack plotting routines for the main analysis.
"""
import argparse
from scharm.stack import plot, draw
import yaml
import sys
from scharm.stack.aggregator import HistDict
from scharm.stack.kinplt import run_kinematic_plot
from scharm.stack.countplt import plot_counts
from scharm.stack.truthplt import plot_truth
from scharm.bullshit import make_dir_if_none
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
        '-s','--signal-point', default='scharm-150-90',
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
                           const='scharm-200-125')
    phys_type.add_argument('-b','--background', action='store_true')
    phys_type.add_argument('-r','--s-over-b', nargs='?',
                           const='scharm-200-125')
    phys_type.add_argument('--all', nargs='?',
                           const='scharm-200-125')
    kinematic_parser.add_argument('--sys-factor', type=float, default=0)
    kinematic_parser.add_argument('-z', '--fix-z-range', action='store_true')

    sr_counts = subs.add_parser('counts',
                                parents=[plotting_general, signal_point])
    sr_counts.add_argument('fits_file')
    sr_counts.add_argument('cuts_file')
    sr_counts.add_argument('--max-val', type=float)

    sr_comp = subs.add_parser('comp',
                                parents=[plotting_general, signal_point])
    sr_comp.add_argument('fits_file')
    sr_comp.add_argument('cuts_file')
    sr_comp.add_argument('--max-val', type=float)

    truth = subs.add_parser('truth', parents=[signal_point])
    truth.add_argument('histmill_file')
    truth.add_argument('-b','--include-background', action='store_true')
    truth.add_argument('--ext', default='.pdf')
    truth.add_argument('-r','--region', default='SR-mll')

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
            'counts':plot_counts, 'comp':plot_counts, 'truth':plot_truth}
    subs[args.which](args)


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
    warnings.warn("this is being migrated to susy-plot-stack",
                  DeprecationWarning, stacklevel=2)
    config = _get_config_info(args.steering_file)
    aggregates = [args.aggregate]
    used_physics = config['backgrounds']['used'] + ['data']
    if args.signal_point:
        used_physics.append(args.signal_point)
    plots_dict = {}
    for agg_file in aggregates:
        print 'loading {}'.format(agg_file)
        hists = HistDict(agg_file,args.filt, physics_set=used_physics,
                         var_blacklist={'truth'})
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
        # TODO:
        #  1) get lumi from aggregate file
        #  2) make a better filter for background types
        bgs = ['diboson', 'QCD','singleTop','ttbar', 'ttbarV']
        for ch in 'BCL':
            bgs.append('Wjets' + ch)
            bgs.append('Zjets' + ch)
        config = {
            'misc': { 'lumi_fb': 21 },
            'backgrounds': {'used': bgs },
            }
        return config
    else:
        with open(steering_file) as steering_yml:
            config = yaml.load(steering_yml)
        return config



if __name__ == '__main__':
    run()
