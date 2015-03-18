#!/usr/bin/env python3
"""
Plotting routine for stack plots
"""
_bl_help = "don't plot data (make blinded plots)"
_p_help = "some changes for the paper versions (don't show event counts)"
_ext_help = (
    'Plot extensions. Can be a matplotlib image extension or'
    ' ".txt" for hepdata dump')
import argparse
import yaml
import sys

def get_config():
    d = 'default: %(default)s'
    c = "with no argument is '%(const)s'"
    def_pts = ['scharm-400-200', 'scharm-550-50']
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('aggregate')
    parser.add_argument('steering_file', nargs='?')
    parser.add_argument('--scale', choices={'log','linear', 'both'},
                        default='both', help=d)
    parser.add_argument('-f', '--fit-parameters', help='from fit-results')
    parser.add_argument('-b', '--blind', action='store_true', help=_bl_help)
    parser.add_argument(
        '--filt', help='only print plots matching some proc/variable/region')
    parser.add_argument(
        '-s','--signal-points', default=def_pts, nargs='+',
        help="assumes <particle>-<something> type name, " + d)
    parser.add_argument(
        '--ext', help=_ext_help + ' ' + d, default='.pdf')
    parser.add_argument('-o', '--output-dir',
                        help=d, default='plots')
    parser.add_argument(
        '-t','--theme', choices={'dan','sbot','brimstone','brony', 'light'},
        default='dan', help=d)
    parser.add_argument('--serial', action='store_true',
                        help='disable multiprocess')
    parser.add_argument('-p', '--paper', action='store_true', help=_p_help)
    parser.add_argument('-a', '--approved', action='store_true')
    subset = parser.add_mutually_exclusive_group()
    subset.add_argument('-e', '--paper-subset', action='store_true')
    subset.add_argument('-d', '--debugging-subset', action='store_true')

    args = parser.parse_args(sys.argv[1:])
    return args

def run():
    args = get_config()
    run_plotmill(args)

_paper_plots = [
    ('signal_mct150', 'mass_ct'),
    ('signal_mct150', 'met'),
    ('signal_mct150', 'mass_cc'),
    ('signal_mct150', 'j0_pt'),
    ('cr_z', 'mass_ll'),
    ('cr_z', 'mass_ct'),
    ('cr_w', 'mass_ct'),
    ('cr_w', 'met'),
    ('cr_t', 'mass_ll'),
    ('cr_t', 'met'),
    ]

_debug_plots = [
    ('signal_mct150', 'mass_ct'),
    ('signal_mct150', 'j0_flavor_truth_label'),
    ]

def run_plotmill(args):
    from scharm.aggregate import plot
    from scharm.aggregate.aggschema import HistDict

    from scharm.bullshit import helvetify
    helvetify()
    from numpy import seterr
    seterr(invalid='ignore', divide='ignore')

    mudic = _get_par_dict(args.fit_parameters)

    config = _get_config_info(args.steering_file)
    aggregates = [args.aggregate]
    plots_dict = {}

    fastplots = None
    if args.paper_subset:
        fastplots = _paper_plots
    elif args.debugging_subset:
        fastplots = _debug_plots

    for agg_file in aggregates:
        print('loading {}...'.format(agg_file), end='', flush=True)
        hists = HistDict(agg_file,args.filt, sig_points=args.signal_points,
                         sig_prefix='scharm', fast=fastplots,
                         var_blacklist={'truth'})
        plots_dict.update(hists)
        print('done')

    needed = _get_signal_finder(args.signal_points)
    plots_dict = {k:v for k,v in plots_dict.items() if needed(k)}
    pltconfig = {
        'lumi_fb': config['misc']['lumi_fb'],
        'base_dir': args.output_dir,
        'output_ext': '_blinded' + args.ext if args.blind else args.ext,
        'theme': args.theme,
        'serial': args.serial,
        'show_event_counts': not args.paper,
        'approval_status': '' if args.approved else 'Work in Progress',
        }
    do_log = args.scale == 'log'
    more_args = dict(mu_dict=mudic, blind=args.blind)
    plot.make_plots(plots_dict, pltconfig, log=do_log, **more_args)
    if args.scale == 'both' and not args.ext == '.txt':
        plot.make_plots(plots_dict, pltconfig, log=True, **more_args)

def _get_signal_finder(signal_points):
    if signal_points:
        signal_head = signal_points[0].split('-')[0]
        def needed(tup):
            phys = tup[0]
            if not phys.startswith(signal_head):
                return True
            for point in signal_points:
                if phys == point:
                    return True
            return False
    else:
        def needed(tup):
            return True
    return needed

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
            'misc': { 'lumi_fb': 20.3 },
            }
        return config
    else:
        with open(steering_file) as steering_yml:
            config = yaml.load(steering_yml)
        return config

def _get_par_dict(fit_parameters, pfx='mu_'):
    """
    expects fit parameters as yaml.
    returns a {par:after_fit_val} dict with `pfx` removed from the par name
    """
    if not fit_parameters:
        return {}

    import yaml
    with open(fit_parameters) as yml:
        full_dict = yaml.load(yml)
    mupars = [x for x in full_dict if x.startswith(pfx)]
    proc_val_dict = {}
    for par in mupars:
        proc = par[len(pfx):]
        af = full_dict[par]['after']
        proc_val_dict[proc] = (af['value'], af['error'])
    return proc_val_dict

if __name__ == '__main__':
    run()
