#!/usr/bin/env python3.3
"""
build fit input files
"""

_files_help="can specify either a root directory or a set of files"

import argparse
from scharm.aggregate import fitinputs
from scharm.aggregate.sample_selector import SampleSelector
from scharm.aggregate.file_selector import get_all_files
from os.path import isfile
import sys, os

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
    args = parser.parse_args(sys.argv[1:])
    return args

def run():
    args = get_config()

    all_files = get_all_files(args.files, systematic=args.systematic)
    selected_samples = SampleSelector(args.meta).select_samples(all_files)

    input_maker = fitinputs.FitInputMaker(meta_path=args.meta)
    yaml_summary = input_maker.make_inputs(all_files)
    print(yaml_summary)

if __name__ == '__main__':
    run()
