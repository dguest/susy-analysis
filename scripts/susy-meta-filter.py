#!/usr/bin/env python2.7

import os, sys
import argparse
from stop.meta import DatasetCache
from os.path import isfile
from tempfile import TemporaryFile
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
    parser.add_argument('-d', '--strip-data', action='store_true')
    parser.add_argument('-s', '--strip-simulation', action='store_true')
    parser.add_argument('-m', '--strip-distiller-meta', action='store_true')
    parser.add_argument('-i', '--write-to-input', action='store_true')
    parser.add_argument('-t', '--filter-and-set-physicstype', 
                        action='store_true')
    parser.add_argument('-o', '--output')
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

    if not args.output: 
        out = TemporaryFile()
    else: 
        out = args.output

    output = DatasetCache(out)
        
    if isinstance(out, str) and isfile(out): 
        raise IOError("won't overwrite output file {}".format(out))
    
    for key, item in original.iteritems(): 
        if args.strip_cutflow and hasattr(item,'cutflow'): 
            del item.cutflow
        if args.strip_distiller_meta: 
            item.d3pds = []
            item.skim_paths = {}
            item.grl = ''
            item.distill_flags = ''
            item.calibration_dir = ''
            item.n_raw_entries = ''
            item.n_corrupted_files = 0
            item.need_rerun = None
            
        if args.strip_data and item.is_data: 
            continue
        if args.strip_simulation and not item.is_data: 
            continue
        if args.filter_and_set_physicstype: 
            from stop.runtypes import set_dataset_physics_type
            is_set = set_dataset_physics_type(item)
            if not is_set: 
                continue

        output[key] = item 

    output.write()
    if not args.output and not args.write_to_input: 
        out.seek(0)
        for line in out: 
            print line.rstrip()
    if args.write_to_input: 
        with open(args.input_meta[0],'w') as outfile: 
            out.seek(0)
            for line in out: 
                outfile.write(line)

if __name__ == '__main__': 
    run()
