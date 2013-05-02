#!/usr/bin/env python2.7
"""
This is intended to be the top level script for various performance scripts. 
"""

_tag_help="""
Routine to run over d3pds (distill) condense resulting ntuples (stack), and 
plot them (plot). Should probably be run from an empty directory. 
"""

import argparse, sys
import glob
from os.path import isdir, isfile, join, expanduser, splitext
from os.path import dirname, basename
import os
import re
import yaml

from susy.distiller import cutflow
from stop import meta

def run(): 
    config = get_config()
    subs = {'tag':jet_tag_efficinecy}
    subs[config.which](config)

def get_config(): 
    d = 'default: %(default)s'
    c = "with no argument is '%(const)s'"

    parser = argparse.ArgumentParser(description=__doc__)
    subs = parser.add_subparsers(dest='which')
    tag = subs.add_parser('tag', description=_tag_help)
    tag_step = tag.add_subparsers(dest='step')
    tag_distill = tag_step.add_parser('distill')
    tag_distill.add_argument('d3pds', help='input d3pd dir')
    tag_distill.add_argument('meta', help='meta yaml file')
    tag_distill.add_argument(
        '-o','--output-dir', default='whiskey', 
        help='output dir for distillates, ' + d)
    tag_distill.add_argument('--calibration', default='~/calibration', 
                             help=d)
    tag_agg = tag_step.add_parser('stack')
    tag_agg.add_argument('whiskey_dir')
    tag_agg.add_argument('-o', '--output-dir', default='hists', 
                         help='output dir for hists, ' + d)
    tag_plot = tag_step.add_parser('plot')
    tag_plot.add_argument('input_hist')
    return parser.parse_args(sys.argv[1:])

def jet_tag_efficinecy(config): 
    subs = {'distill':distill_d3pds,'stack':aggregate_jet_plots, 
            'plot':plot_jet_eff}
    subs[config.step](config)

def distill_d3pds(config): 
    d3pd_contents = glob.glob('{}/*'.format(config.d3pds))
    datasets = [subdir for subdir in d3pd_contents if isdir(subdir)]
    calibration_dir = expanduser(config.calibration)
    btag_env = join(calibration_dir, 'BTagCalibration.env')
    if not datasets: 
        return 
    if not isdir(config.output_dir): 
        os.mkdir(config.output_dir)
    meta_lookup = meta.DatasetCache(config.meta)
    ds_groups = {}
    for ds in datasets: 
        subfiles = glob.glob('{}/*.root*'.format(ds))
        ds_key = _ds_key_from_ds_name(ds)
        if not ds_key in ds_groups: 
            ds_groups[ds_key] = []
        ds_groups[ds_key] += subfiles

    counts = {}
    for ds_key, subfiles in ds_groups.iteritems(): 
        out_file = _ntuple_name_from_ds_name(dirname(subfiles[0]))
        out_path = join(config.output_dir, out_file)
        flags = 'ev'            # all events saved, verbose
        if _is_atlfast(meta_lookup[ds_key].full_name): 
            flags += 'f'
        cut_counts = cutflow.cutflow(
            input_files=subfiles, flags=flags, output_ntuple=out_path, 
            btag_cal_file=btag_env, cal_dir=calibration_dir)
        counts[splitext(out_file)[0]] = cut_counts
    with open(join(config.output_dir, 'obj_counts.yml'),'w') as out_yml: 
        out_yml.write(yaml.dump(counts))

def aggregate_jet_plots(config): 
    from stop import hyperstack
    input_dir = config.whiskey_dir
    if not isdir(config.whiskey_dir): 
        raise IOError("intput dir '{}' doesn't exist".format(whiskey_dir))
    whiskey = glob.glob(join(config.whiskey_dir, '*.root'))
    if not isdir(config.output_dir): 
        os.mkdir(config.output_dir)
    for tup in whiskey: 
        out_hist_name = splitext(basename(tup))[0] + '.h5'
        out_hist_path = join(config.output_dir, out_hist_name)
        region_dict = {
            'name':'alljet',    # the top level in the output
            'output_name': out_hist_path, 
            'type': 'CONTROL',  # doesn't really matter
            'hists': 'TAG_EFFICIENCY', 
            'jet_tag_requirements':['NOTAG'], # require one jet 
            }
        hyperstack.stacksusy(
            input_file=tup, region_list=[region_dict], flags='v')
                      
def plot_jet_eff(config): 
    hist_file_name = config.input_hist
    

def _is_atlfast(sample): 
    sim_re = re.compile('\.e[0-9]+_([as])[0-9]+_')
    try: 
        char = sim_re.search(sample).group(1)
    except AttributeError: 
        raise ValueError("can't find type of reco in {}".format(sample))
    if char == 'a': 
        return True
    elif char == 's': 
        return False
    else: 
        raise ValueError(
            "not sure what kind of sample '{}' is".format(sample))

def _ntuple_name_from_ds_name(ds_name): 
    bname = basename(ds_name)
    fields = bname.split('.')
    try: 
        type_index = fields.index('mc12_8TeV')
    except ValueError: 
        raise ValueError("can't find type string in {}".format(bname))
    dsid = fields[type_index + 1]
    sim_name = fields[type_index + 2].split('_')[0]
    return '{}-{}.root'.format(sim_name, dsid)

def _ds_key_from_ds_name(ds_name): 
    bname = basename(ds_name)
    fields = bname.split('.')
    type_index = fields.index('mc12_8TeV')
    dsid = fields[type_index + 1]
    return 's{}'.format(dsid)

if __name__ == '__main__': 
    run()
