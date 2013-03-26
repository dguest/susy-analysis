#!/usr/bin/env python2.7
"""
uses run / mc meta data and ntuples files to produce stacks
"""

import argparse
from stop.stack.coordinator import Coordinator
from os.path import isfile
from stop.stack import table
from tempfile import TemporaryFile
import yaml
import sys


def get_config(): 
    d = 'default: %(default)s'
    c = "with no argument is '%(const)s'"

    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('steering_file', help="created if it doesn't exist")
    parser.add_argument('-f','--force-aggregation', action='store_true')
    parser.add_argument('-r','--rerun-stack', action='store_true')
    parser.add_argument('-c', '--counts-file', default='counts.yml',help=d)
    parser.add_argument(
        '-s','--signal-point', default='stop-150-90', 
        help="assumes <particle>-<something> type name, " + d)
    parser.add_argument('-p', '--make-plots', action='store_true')
    parser.add_argument(
        '--ext', help='plot extensions, ' + d, default='.pdf')
    parser.add_argument(
        '--stats', action='store_true', help='only do statistics, not hists')
    parser.add_argument('--fast', action='store_true', 
                        help="don't do systematics")
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

    if not isfile(args.steering_file): 
        coord = Coordinator()
        with open(args.steering_file,'w') as yml: 
            coord.write(yml)
            sys.exit('wrote example steering file, quitting...')

    with open(args.steering_file) as yml: 
        coord = Coordinator(yml)
    needed_systematics = coord.get_needed_aggregates()

    to_do = [
        args.make_plots, 
        not isfile(args.counts_file), 
        needed_systematics, 
        args.force_aggregation, 
        args.rerun_stack, 
        ]
    if not any(to_do): 
        sys.exit('nothing to do...')

    systematic = 'NONE' if args.fast else 'all'
    coord.bugstream = TemporaryFile()
    coord.stack(systematic=systematic, 
                rerun=args.rerun_stack, do_stat_regions=args.stats)

    all_dict = coord.aggregate(systematic=systematic,
                               rerun=args.force_aggregation)
    if args.fast: 
        all_dict = {'NONE':all_dict}
        
    count_dict = {}
    for name, syst_dict in all_dict.iteritems(): 
        safe = not args.stats
        syst_count = table.get_physics_cut_dict(syst_dict)
        count_dict[name] = syst_count
    
    if args.counts_file: 
        with open(args.counts_file,'w') as countfile: 
            for line in yaml.dump(table.yamlize(count_dict)): 
                countfile.write(line)

    needed = get_signal_finder(args.signal_point)
    if args.make_plots: 
        from stop.stack import plot
        plots_dict = {k:v for k,v in all_dict['NONE'].items() if needed(k)}
        plotting_info = coord.get_plotting_meta()
        plotting_info['output_ext'] = args.ext
        plot.make_plots(plots_dict, plotting_info)
        
    if coord.bugstream.tell(): 
        coord.bugstream.seek(0)
        bugs_log = 'aggregate-bugs.log' 
        with open(bugs_log,'w') as bugs: 
            for line in coord.bugstream: 
                bugs.write(line)
        sys.stderr.write('wrote bugs into {}\n'.format(bugs_log))
    

if __name__ == '__main__': 
    run()
