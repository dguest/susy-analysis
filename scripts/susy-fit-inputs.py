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
import yaml

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
    parser.add_argument('-f','--fast', action='store_true',
                        help="don't use (many) signal points")
    args = parser.parse_args(sys.argv[1:])
    return args

def run():
    args = get_config()

    all_files = get_all_files(args.files, systematic=args.systematic)
    selected_samples = SampleSelector(args.meta).select_samples(all_files)

    sig_pt = None
    if args.fast:
        sig_pt = 'scharm-550-50'
    input_maker = fitinputs.FitInputMaker(
        meta_path=args.meta, signal_point=sig_pt)
    summary = input_maker.make_inputs(list(selected_samples))
    out_dict = {args.systematic: summary}
    with open(args.output,'w') as out_yml:
        out_yml.write(yaml.dump(out_dict))

if __name__ == '__main__':
    run()
