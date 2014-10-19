#!/usr/bin/env python3
"""
build fit input files
"""

_files_help="can specify either a root directory or a set of files"
_syst_help="'all' will run all systematics"
_vr_prefix_help="don't include regions with this (validation) prefix"

import argparse
from scharm.aggregate import fitinputs
from scharm.aggregate.sample_selector import SampleSelector
from scharm.aggregate.file_selector import get_all_files
from scharm.schema import get_all_systematics
from concurrent.futures import ProcessPoolExecutor as Executor
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
    parser.add_argument('-s','--systematic', default='none',
                        help=(_syst_help + ', ' + d))
    parser.add_argument('-v','--vr-prefix', default='vr', help=d)
    parser.add_argument('-f','--fast', action='store_true',
                        help="don't use (many) signal points")
    args = parser.parse_args(sys.argv[1:])
    return args

def run():
    args = get_config()
    dargs = vars(args)

    # launch a bunch of processes to look at all systematics
    if args.systematic == 'all':
        dargs['quiet'] = True   # multiprocessing makes a mess of the outputs
        systs = get_all_systematics(args.files)
        syst_args = {x: dargs.copy() for x in systs}
        for syst in systs:
            syst_args[syst]['systematic'] = syst
        executor = Executor()
        counts_list = executor.map(run_systematic, syst_args.values())
        counts_dict = {}
        for subdict in counts_list:
            counts_dict.update(subdict)
    # or just do one...
    else:
        counts_dict = run_systematic(dargs)
    with open(dargs['output'],'w') as out_yml:
        translated = fitinputs.translate_to_fit_inputs(counts_dict)
        out_yml.write(yaml.dump(translated))

def run_systematic(args):
    all_files = get_all_files(args['files'], systematic=args['systematic'])
    selected_samples = SampleSelector(args['meta']).select_samples(all_files)

    sig_pt = None
    quiet = args.get('quiet', False)
    if args['fast']:
        sig_pt = 'scharm-550-50'
    input_maker = fitinputs.FitInputMaker(
        meta_path=args['meta'], signal_point=sig_pt,
        quiet=quiet, veto_region_prefix=args['vr_prefix'])
    summary = input_maker.make_inputs(list(selected_samples))
    out_dict = {args['systematic']: summary}
    if quiet:
        print('done with {}'.format(args['systematic']))
    return out_dict

if __name__ == '__main__':
    run()
