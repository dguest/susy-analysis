#!/usr/bin/env python3.3
"""
uses run / mc meta data and ntuples files to produce stacks.
"""

_files_help = "can specify either a root directory or a set of files"
_systematics_help = "add in dominant experimental systematics"

import argparse
from scharm.aggregate import aggregator as agg
from scharm.aggregate.sample_selector import SampleSelector
from scharm.aggregate.file_selector import get_all_files
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
    parser.add_argument('files', nargs='+', help=_files_help)
    parser.add_argument('-m','--meta', required=True)
    parser.add_argument('-o','--output', required=True)
    parser.add_argument('-s','--systematic', default='none', help=d)
    parser.add_argument('-d','--dump', action='store_true',
                        help='dump samples used (and quit)')
    parser.add_argument('-b','--breakdown', action='store_true',
                        help='store breakdown of backgrounds')
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

    all_files = get_all_files(args.files, systematic=args.systematic)
    if args.dump:
        samples = SampleSelector(args.meta).select_datasets(all_files)
        for samp in samples:
            print(samp)
        sys.exit(0)
    selected_samples = SampleSelector(args.meta).select_samples(all_files)

    aggregator = agg.SampleAggregator(
        meta_path=args.meta,
        hfiles=list(selected_samples),
        variables='all',
        breakdown=args.breakdown,
        )
    aggregator.aggregate()
    agg_name = args.output
    if isfile(agg_name):
        os.remove(agg_name)
    aggregator.write(agg_name)

if __name__ == '__main__':
    run()
