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

NOTE: should remove this, the functionality could be provided by the 
susy-performance routines. 
"""

_hadd_help="""
Adds the hists of datasets that were fragmented in distillation. 

NOTE: will require updating if we move to including the total number of ds
in the split ds names. 
"""

import argparse, sys
import yaml
import glob
from os.path import join, splitext, isdir, isfile, expanduser
import os
from itertools import groupby

from stop.stack.regions import Region, condense_regions
from stop.stack.stacker import Stacker
import h5py
from stop.hists import HistAdder

def run(): 
    config = get_config()
    subs = {
        'setup': setup_steering, 
        'run': run_stacker, 
        'hadd': run_hadd}
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
    
    hadd_parser = subs.add_parser('hadd', description=_hadd_help)
    hadd_parser.add_argument('steering_file')
    hadd_parser.add_argument('-c', '--clean', action='store_true', 
                             help='remove partial files after adding')
    hadd_parser.add_argument('-v','--verbose', action='store_true')
    hadd_parser.add_argument(
        '--systematic', help='only hadd this systematic', default='*')
    hadd_parser.add_argument(
        '--method', help='only hadd this method', default='*')
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

    stacker = Stacker(regions, meta_dict=config_dict['files']['meta'])
    stacker.mc_mc_sf_file = expanduser(config_dict['files']['mcsf'])
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

def run_hadd(config): 
    with open(config.steering_file) as steering: 
        steering_dict = yaml.load(steering)

    hist_dir = steering_dict['files']['hists']
    method_dir = config.method
    syst_dir = config.systematic
    all_files = glob.glob(join(hist_dir, method_dir, syst_dir, '*-*.h5'))

    def stub_finder(file_name): 
        master_stub, part, ext = file_name.rpartition('-')
        try: 
            int(splitext(ext)[0])
        except ValueError: 
            raise ValueError("{} doesn't parse in hadd".format(file_name))
        return master_stub

    for stub, in_itr in groupby(sorted(all_files), stub_finder): 
        in_files = list(in_itr)
        out_name = '{}.h5'.format(stub)
        if config.verbose: 
            print 'hadding {}'.format(out_name)
        _hadd(in_files, out_name)
        if config.clean: 
            for old_in in in_files: 
                os.remove(old_in)

def _hadd(good_files, output):
    with h5py.File(good_files[0]) as base_h5: 
        hadder = HistAdder(base_h5)
    for add_file in good_files[1:]: 
        if not isfile(add_file): 
            raise IOError("{} doesn't exist".format(add_file))
        with h5py.File(add_file) as add_h5: 
            hadder.add(add_h5)

    with h5py.File(output,'w') as out_file: 
        hadder.write_to(out_file)


if __name__ == '__main__': 
    run()
