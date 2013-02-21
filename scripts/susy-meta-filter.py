#!/usr/bin/env python2.7

import os, sys
import argparse
from stop.meta import DatasetCache
from os.path import isfile
"""
Script to filter meta info. 
"""

def _get_parser(): 
    parser = argparse.ArgumentParser(
        formatter_class=argparse.RawDescriptionHelpFormatter, 
        description=__doc__, 
        epilog='Author: Dan Guest', 
        )
    parser.add_argument('input_meta', nargs='+')
    parser.add_argument('-c', '--strip-cutflow', action='store_true')
    parser.add_argument('-d', '--split-data', action='store_true')
    return parser

def run(): 
    args = _get_parser().parse_args(sys.argv[1:])
    original = DatasetCache(args.input_meta[0])
    keys = set(original)
    for add_ds in args.input_meta[1:]: 
        new_cache = DatasetCache(add_ds)
        new_keys = set(new_cache)
        overlap = keys & new_keys
        if overlap: 
            raise IOError("found repeat keys {}".format(overlap))
        original.update(new_cache)
    data_out = 'data.yml'
    mc_out = 'mc.yml'
    everything_out = 'all.yml'
    if args.split_data: 
        data_meta = DatasetCache(data_out)
        mc_meta = DatasetCache(mc_out)
        everything_meta = None
        all_outputs = [data_out, mc_out]
    else: 
        data_meta = None
        mc_meta = None
        everything_meta = DatasetCache(everything_out)
        all_outputs = [everything_out]
        
    if any(isfile(x) for x in all_outputs): 
        raise IOError("won't overwrite output files {}".format(all_outputs))
    
    for key, item in original.iteritems(): 
        if args.strip_cutflow and hasattr(item,'cutflow'): 
            del item.cutflow
        if data_meta is not None: 
            if item.is_data: 
                data_meta[key] = item
            else: 
                mc_meta[key] = item 
        else:
            everything_meta[key] = item

    if data_meta: 
        data_meta.write()
    if mc_meta: 
        mc_meta.write()
    if everything_meta: 
        everything_meta.write()

if __name__ == '__main__': 
    run()
