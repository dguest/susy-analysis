#!/usr/bin/env python2.7
"""
An attempt at a dumber stacking routine. Runs from a textfile, decides on 
which systematics / regions to use based on a metafile. 
"""

_setup_help="""
sets up the textfile used to steer the actual stacker. 
"""


import argparse, sys
import yaml
import glob
from os.path import join, splitext, isdir
import os

from stop.stack.regions import Region, condense_regions
from stop.stack.stacker import Stacker

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
    return parser.parse_args(sys.argv[1:])

def setup_steering(config): 
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

class SystMap(object): 
    """
    Maps an ntuple path to a systematic (or list of systematics) that should 
    be applied. 
    """
    def __init__(self, ntuples_dict): 
        self.scale_factor_systematics = ['NONE'] + [
            part + shift for part in 'BCUT' for shift in ['UP','DOWN']
            ]
        self.map = {v:k for k,v in ntuples_dict.iteritems()}
    def get_systematics(self, ntuple): 
        syst = None
        for k in self.map:
            if k in ntuple: 
                if syst: 
                    raise ValueError(
                        'tried to assign both {} and {} to {}'.format(
                            syst, self.map[k], ntuple))
                syst = self.map[k]
        if syst is None: 
            raise ValueError("couldn't find syst for {}".format(ntuple))
        if syst == 'NONE': 
            return self.scale_factor_systematics
        else: 
            return [syst]

def run_stacker(config): 
    with open(config.steering_file) as steering_yml: 
        config_dict = yaml.load(steering_yml)

    syst_map = SystMap(config_dict['files']['ntuples'])

    regions = {k:Region(v) for k, v in config_dict['regions'].iteritems()}
    hists_dir = config_dict['files']['hists']
    if not isdir(hists_dir): 
        os.mkdir(hists_dir)
    if config.mode == 'kinematic_stat': 
        regions = condense_regions(regions)
        super_dict = {n:v.get_yaml_dict() for n,v in regions.iteritems()}
        with open(join(hists_dir, 'superregions.yml'), 'w') as super_yml: 
            super_yml.write(yaml.dump(super_dict))

    stacker = Stacker(regions)
    stacker.rerun = True
    stacker.make_dirs = True
    stacker.verbose = False

    ntuples = []
    with open(config.input_files_list) as file_list: 
        for line in file_list: 
            ntuples.append(line.strip())
    stacker.total_ntuples = len(ntuples)

    for tuple_n, ntuple in enumerate(ntuples): 
        systematics = syst_map.get_systematics(ntuple)
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
