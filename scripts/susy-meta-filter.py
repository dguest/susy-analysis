#!/usr/bin/env python2.7

import os, sys
import argparse
from stop.meta import DatasetCache
from os.path import isfile, splitext
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
    parser.add_argument(
        '-m', '--merge', action='store_true', 
        help='merge meta info from jobs which may be split by distiller')
    parser.add_argument('-k','--kleen', action='store_true', 
                        help='clean out distiller metainfo')
    output = parser.add_mutually_exclusive_group()
    output.add_argument('-i', '--write-to-input', action='store_true')
    output.add_argument('-o', '--output')
    output.add_argument('-n', '--n-outputs', type=int)
    return parser

def correct_general(ds): 
    """
    fix any generally stale stuff around the ds
    """
    if ds.is_data: 
        ds.bugs -= set(['ambiguous dataset','bad files','no cross section', 
                        'no filter efficiency'])
        if not ds.physics_type: 
            ds.physics_type = 'data'
    else: 
        if 'no cross section' in ds.bugs: 
            ds.total_xsec_fb = 0.0
        if 'no filter efficiency' in ds.bugs: 
            ds.filteff = 0.0
        ds.bugs -= set(['no cross section', 'no filter efficiency',
                        'bad files'])

def strip_distiller_meta(item, merging=True): 
    item.d3pds = []
    item.skim_paths = {}
    item.grl = ''
    item.distill_flags = ''
    item.calibration_dir = ''
    item.missing_branch = ''
    item.need_rerun = None
    item.btag_env = ''
    item.bugs -= set(['no d3pds'])
    if not merging: 
        if item.subset_index and not item.subset_index == 'merged': 
            raise MergingError(
                'tried to delete subset info from non-merged dataset')
        item.subset_index = 0
        item.total_subsets = 0
        item.n_raw_entries = 0
        item.n_corrupted_files = 0
        item.meta_sources -= set(['distiller'])
        item.sum_event_weight = 0.0

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

    outputs = []
    out_stream = None
    if args.n_outputs: 
        for n in xrange(args.n_outputs): 
            base, ext = splitext(args.input_meta[0])
            out_name = '{}-{}.yml'.format(base, n)
            outputs.append(DatasetCache(out_name))
    elif args.write_to_input: 
        outputs = [DatasetCache(args.input_meta[0])]
    elif args.output: 
        if isinstance(args.output, str) and isfile(args.output): 
            raise IOError(
                "won't overwrite output file {}".format(args.output))
        outputs = [DatasetCache(args.output)]
    else: 
        out_stream = TemporaryFile()
        outputs = [DatasetCache(out_stream)]
        
    mergers = {}
    for out_n, (key, item) in enumerate(original.iteritems()): 
        if args.strip_cutflow and hasattr(item,'cutflow'): 
            del item.cutflow
        if args.strip_data and item.is_data: 
            continue
        if args.strip_simulation and not item.is_data: 
            continue
        correct_general(item)

        subout = out_n % len(outputs)

        if args.merge: 
            strip_distiller_meta(item, merging=True)
            if item.mergekey != item.key: 
                if not item.mergekey in mergers: 
                    mergers[item.mergekey] = item 
                else: 
                    mergers[item.mergekey] += item 
                if mergers[item.mergekey].subset_index == 'merged': 
                    merged = mergers[item.mergekey]
                    if args.kleen: 
                        strip_distiller_meta(merged, merging=False)
                    outputs[subout][item.mergekey] = merged
                    del mergers[item.mergekey]
            else: 
                if args.kleen: 
                    strip_distiller_meta(item, merging=False)
                outputs[subout][key] = item 
        else: 
            if args.kleen: 
                strip_distiller_meta(item, merging=False)
            outputs[subout][key] = item 

    for output in outputs: 
        output.write()
    if out_stream: 
        out_stream.seek(0)
        for line in out_stream: 
            print line.rstrip()


class MergingError(IOError): 
    def __init__(self, problem): 
        super(MergingError, self).__init__(problem)

if __name__ == '__main__': 
    run()

