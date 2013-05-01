#!/usr/bin/env python2.7
"""
This is intended to be the top level script for various performance scripts. 
"""
import argparse, sys
import glob
from os.path import isdir, isfile, basename, join, expanduser, splitext
import os
import re
import yaml

from susy.distiller import cutflow

def run(): 
    config = get_config()
    subs = {'tag':jet_tag_efficinecy}
    subs[config.which](config)

def get_config(): 
    d = 'default: %(default)s'
    c = "with no argument is '%(const)s'"

    parser = argparse.ArgumentParser(description=__doc__)
    subs = parser.add_subparsers(dest='which')
    tag = subs.add_parser('tag')
    tag_step = tag.add_subparsers(dest='step')
    tag_distill = tag_step.add_parser('distill')
    tag_distill.add_argument('d3pds', help='input d3pds')
    tag_distill.add_argument(
        '-o','--output-dir', default='whiskey', 
        help='output dir for distillates, ' + d)
    tag_distill.add_argument('--calibration', default='~/calibration', 
                             help=d)
    tag_agg = tag_step.add_parser('stack')
    tag_agg.add_argument('whiskey_dir')
    tag_agg.add_argument('-o', '--output-dir', default='hists', 
                         help='output dir for hists, ' + d)
    return parser.parse_args(sys.argv[1:])

def jet_tag_efficinecy(config): 
    subs = {'distill':distill_d3pds,'aggregate':aggregate_jet_plots}
    subs[config.step](config)

def distill_d3pds(config): 
    d3pd_contents = glob.glob('{}/*'.format(config.d3pds))
    datasets = [subdir for subdir in d3pd_contents if isdir(subdir)]
    calibration_dir = expanduser(config.calibration)
    btag_env = join(calibration_dir, 'BTagCalibration.env')
    if not datasets: 
        return 
    if not isdir(config.out_dir): 
        os.mkdir(config.out_dir)
    counts = {}
    for ds in datasets: 
        subfiles = glob.glob('{}/*.root*'.format(ds))
        out_file = _ntuple_name_from_ds_name(ds)
        out_path = join(config.out_dir, out_file)
        flags = 'ev'
        if _is_atlfast(ds): 
            flags += 'f'
        cut_counts = cutflow(
            input_files=subfiles, flags=flags, output_ntuple=out_path, 
            btag_cal_file=btag_env, cal_dir=calibration_dir)
        counts[splitext(out_file)[0]] = cut_counts
    with open(join(config.out_dir, 'obj_counts.yml'),'w') as out_yml: 
        out_yml.write(yaml.dump(counts))

def aggregate_jet_plots(config): 
    pass

def _is_atlfast(sample): 
    sim_re = re.compile('\.e[0-9]+_([af])[0-9]+_')
    char = sim_re.search(sample).group(1)
    if char == 'a': 
        return True
    elif char == 'f': 
        return False
    else: 
        raise ValueError(
            "not sure what kind of sample '{}' is".format(sample))

def _ntuple_name_from_ds_name(ds_name): 
    bname = basename(ds_name)
    fields = bname.split('.')
    type_index = fields.index('mc12_8TeV')
    dsid = fields[type_index + 1]
    sim_name = fields[type_index + 2].split('_')[0]
    return '{}-{}.root'.format(sim_name, dsid)

if __name__ == '__main__': 
    run()
