#!/usr/bin/env python2.7
"""
uses run / mc meta data and ntuples files to produce stacks. 

NOTE: the stacking routine here has been superseded by susy-stack. 
"""

import argparse
from stop.stack.coordinator import Coordinator
from stop.systematics.merger import RegionMerger, RegionMergeError
from os.path import isfile
from stop.stack import table
from tempfile import TemporaryFile
import yaml
import sys
import warnings

def get_config(): 
    d = 'default: %(default)s'
    c = "with no argument is '%(const)s'"

    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('steering_file', help="created if it doesn't exist")
    parser.add_argument('-f','--force-aggregation', action='store_true')
    parser.add_argument('-r','--rerun-stack', action='store_true')
    parser.add_argument('-c','--remake-counts', action='store_true')
    parser.add_argument(
        '--mode', choices={'histmill','kinematic_stat'}, 
        default='kinematic_stat', help='default: %(default)s')
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
    needed_systematics = coord.get_needed_aggregates(args.mode)

    if args.rerun_stack: 
        warnings.warn("we're trying to replace this with susy-stack", 
                      FutureWarning)

    to_do = [
        needed_systematics, 
        args.force_aggregation, 
        args.rerun_stack, 
        args.remake_counts, 
        ]
    if not any(to_do): 
        sys.exit('nothing to do...')

    systematic = 'NONE' if args.fast else 'all'
    coord.bugstream = TemporaryFile()
    if args.rerun_stack: 
        coord.stack(systematic=systematic, 
                    rerun=args.rerun_stack, mode=args.mode)

    count_dict = coord.aggregate(systematic=systematic,
                                 rerun=args.force_aggregation, 
                                 mode=args.mode)
    if args.fast: 
        count_dict = {'NONE':count_dict}
    
    if args.mode == 'kinematic_stat': 
        hierarchical_counts = table.yamlize(count_dict)
        with open(args.steering_file) as steer_yaml: 
            steering = yaml.load(steer_yaml)
        try: 
            RegionMerger(hierarchical_counts).merge_via_steering(steering)
        except RegionMergeError as err: 
            warnings.warn('merge error: {}, writing unmerged'.format(
                    err))
        counts_file = steering['files']['counts']
        with open(counts_file,'w') as countfile: 
            for line in yaml.dump(hierarchical_counts): 
                countfile.write(line)
        
    if coord.bugstream.tell(): 
        coord.bugstream.seek(0)
        bugs_log = 'aggregate-bugs.log' 
        with open(bugs_log,'w') as bugs: 
            for line in coord.bugstream: 
                bugs.write(line)
        sys.stderr.write('wrote bugs into {}\n'.format(bugs_log))
    

if __name__ == '__main__': 
    run()
