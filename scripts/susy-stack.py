#!/usr/bin/env python3.3
"""
An attempt at a dumber stacking routine for the full analysis. 
Sets up textfiles that point to collections of root files, decides on 
which systematics / regions to use based on a metafile and the 
path to the root files. 
"""

import argparse, sys
import yaml
import glob
from os.path import join, splitext, expanduser
import os
from warnings import warn

from scharm.stack.regions import Region
from scharm.stack.stacker import Stacker
from scharm.bullshit import make_dir_if_none
import h5py

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

    parser.add_argument('input_files_list')
    parser.add_argument('steering_file')
    parser.add_argument(
        '--mode', default='histmill', choices={
            'histmill','kinematic_stat','nminus'}, 
        help=d)
    parser.add_argument('-d','--dump-run', action='store_true', 
                            help="dump region config, don't run")
    parser.add_argument('-o','--hists-dir', default='hists', help=d)
    
    return parser.parse_args(sys.argv[1:])


def run_stacker(config): 
    with open(config.steering_file) as steering_yml: 
        config_dict = yaml.load(steering_yml)

    regions = {k:Region(v) for k, v in config_dict['regions'].items()}
    hists_dir = config.hists_dir
    
    make_dir_if_none(hists_dir)

    stacker = Stacker(regions)
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
        systematics = _get_systematics(ntuple)
        outsubdir = None
        if len(systematics) == 1: 
            outsubdir = systematics[0].lower()

            # if the systematic was applied at the distiller level we can 
            # turn it off here
            systematics = ['NONE'] 
        stacker.run_multisys(
            ntuple, hists_dir, systematics, 
            outsubdir=outsubdir, tuple_n=tuple_n)

_shift_sf = list('BCUT') + ['EL','MU']
scale_factor_systematics = ['NONE'] + [
    part + shift for part in _shift_sf for shift in ['UP','DOWN']
    ]

def _get_systematics(ntuple): 
    tld = dirname(ntuple).split('/')[-1]
    syst = tld.upper()
    if syst == 'NONE': 
        return scale_factor_systematics
    else: 
        return [syst]

if __name__ == '__main__': 
    run()
