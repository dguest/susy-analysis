#!/usr/bin/env python2.7
"""
An attempt at a dumber stacking routine for the full analysis. 
Sets up textfiles that point to collections of root files, decides on 
which systematics / regions to use based on a metafile and the 
path to the root files. 

NOTE: Should really consider merging this with the susy-performance stacking
routine, which manages the construction of batch submit scripts. 
"""

_setup_help="""
Sets up the textfiles. Can set up multiple textfiles, which are simply 
named for batch submission. 

NOTE: should remove this, or move to susy-setup. 
"""

import argparse, sys
import yaml
import glob
from os.path import join, splitext, expanduser
import os
from itertools import groupby
import errno
from warnings import warn

from stop.stack.regions import Region, condense_regions
from stop.stack.stacker import Stacker
from stop.bullshit import make_dir_if_none
import h5py
from stop.hists import HistAdder
from stop.sysdef import get_systematics

def run(): 
    config = get_config()
    subs = {
        'setup': setup_steering, 
        'run': run_stacker}
    subs[config.which](config)

def get_config(): 
    d = 'default: %(default)s'
    c = "with no argument is '%(const)s'"

    parser = argparse.ArgumentParser(description=__doc__)
    subs = parser.add_subparsers(dest='which')

    setup_parser = subs.add_parser('setup', description=_setup_help)
    setup_parser.add_argument('steering_file')
    setup_parser.add_argument('output_file', nargs='?', 
                              default='ntuples.txt', help=d)
    setup_parser.add_argument('-s','--split-output', type=int, 
                              help='split output into multiple files')

    run_parser = subs.add_parser('run', description='run the stacker')
    run_parser.add_argument('input_files_list')
    run_parser.add_argument('steering_file')
    run_parser.add_argument(
        '--mode', default='histmill', choices={'histmill','kinematic_stat'}, 
        help=d)
    run_parser.add_argument('-d','--dump-run', action='store_true', 
                            help="dump region config, don't run")
    run_parser.add_argument('-o','--hists-dir', default='hists', help=d)
    
    return parser.parse_args(sys.argv[1:])

def setup_steering(config): 
    warn('setup routine is going to be removed', FutureWarning, stacklevel=2)
    with open(config.steering_file) as meta_yml: 
        config_dict = yaml.load(meta_yml)
    ntuples = config_dict['files']['ntuples']
    all_files = []
    for basedir in ntuples.values(): 
        all_files += glob.glob( join(basedir, '*.root*'))
    if config.split_output: 
        subfiles = {x:[] for x in xrange(config.split_output)}
        for in_n, in_file in enumerate(all_files): 
            subfiles[in_n % config.split_output].append(in_file)
        for file_n in xrange(config.split_output): 
            file_name = '{}-{n}{}'.format(
                *splitext(config.output_file), n=file_n)
            with open(file_name, 'w') as out_file: 
                out_file.writelines('\n'.join(subfiles[file_n]) + '\n')
    else: 
        with open(config.output_file,'w') as out_file: 
            out_file.writelines('\n'.join(all_files) + '\n')


def run_stacker(config): 
    with open(config.steering_file) as steering_yml: 
        config_dict = yaml.load(steering_yml)

    regions = {k:Region(v) for k, v in config_dict['regions'].iteritems()}
    hists_dir = config.hists_dir
    
    make_dir_if_none(hists_dir)

    if config.mode == 'kinematic_stat': 
        regions = condense_regions(regions)
        super_dict = {n:v.get_yaml_dict() for n,v in regions.iteritems()}
        with open(join(hists_dir, 'superregions.yml'), 'w') as super_yml: 
            super_yml.write(yaml.dump(super_dict))

    stacker = Stacker(regions)
    # meta_dict=config_dict['files'].get('meta',None)
    #stacker.mc_mc_sf_file = expanduser(config_dict['files']['mcsf'])
    stacker.rerun = True
    stacker.make_dirs = True
    stacker.verbose = False
    stacker.dummy = config.dump_run

    ntuples = []
    with open(config.input_files_list) as file_list: 
        for line in file_list: 
            ntuples.append(line.strip())
    stacker.total_ntuples = len(ntuples)

    for tuple_n, ntuple in enumerate(ntuples): 
        systematics = get_systematics(ntuple)
        outsubdir = None
        if len(systematics) == 1: 
            outsubdir = systematics[0].lower()

            # if the systematic was applied at the distiller level we can 
            # turn it off here
            systematics = ['NONE'] 
        stacker.run_multisys(
            ntuple, hists_dir, systematics, 
            outsubdir=outsubdir, tuple_n=tuple_n)


if __name__ == '__main__': 
    run()
