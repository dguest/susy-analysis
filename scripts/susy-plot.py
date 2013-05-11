#!/usr/bin/env python2.7
"""
Top level for mc / data stack plotting routines for the main analysis. 
"""
import argparse
from stop.stack import plot
from os.path import isfile, join
from stop.stack import table
from tempfile import TemporaryFile
import yaml
import sys
import glob
from stop.stack.aggregator import HistDict

def get_config(): 
    d = 'default: %(default)s'
    c = "with no argument is '%(const)s'"

    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('steering_file', help="created if it doesn't exist")
    parser.add_argument(
        '--ext', help='plot extensions, ' + d, default='.pdf')
    parser.add_argument(
        '-s','--signal-point', default='stop-150-90', 
        help="assumes <particle>-<something> type name, " + d)
    parser.add_argument(
        '--mode', choices={'histmill','kinematic_stat'}, 
        default='histmill', help='default: %(default)s')
    parser.add_argument('--filt')
    parser.add_argument('--scale', choices={'log','linear', 'both'}, 
                        default='linear', help=d)
    args = parser.parse_args(sys.argv[1:])
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

    with open(args.steering_file) as steering_yml: 
        config = yaml.load(steering_yml)
    hists_base = config['files']['hists']
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
