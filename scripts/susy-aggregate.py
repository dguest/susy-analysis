#!/usr/bin/env python3.3
"""
uses run / mc meta data and ntuples files to produce stacks.

When running in kinematic_stat mode, --fast or --all results in a yaml file.

Could use some refactoring: 
 - one routine should produce an aggregated histogram file. 
 - another routine should make the yaml counts files. 
"""

import argparse
from scharm.stack import aggregator as agg
from os.path import isfile
from tempfile import TemporaryFile
import yaml
import sys, os
import warnings

def get_config(): 
    d = 'default: %(default)s'
    c = "with no argument is '%(const)s'"

    parser = argparse.ArgumentParser(
        description=__doc__, 
        formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument('files', nargs='+')
    parser.add_argument('-m','--meta', required=True)
    parser.add_argument('-o','--output', required=True)
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

    aggregator = agg.SampleAggregator(
        meta_path=args.meta, 
        whiskey=args.files,
        variables='all',
        )
    # aggregator.bugstream = tempfile.TemporaryFile()
    # aggregator.out_prepend = 'systamatic: {} '.format(systematic)
    aggregator.aggregate()
    # aggregator.bugstream.seek(0)
    # for line in aggregator.bugstream: 
    #     print line
    agg_name = args.output
    if isfile(agg_name): 
        os.remove(agg_name)
    aggregator.write(agg_name)
    

if __name__ == '__main__': 
    run()
