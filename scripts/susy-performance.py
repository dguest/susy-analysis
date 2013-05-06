#!/usr/bin/env python2.7
"""
This is intended to be the top level script for various performance scripts. 
"""

_tag_help="""
Routine to run over d3pds (distill) condense resulting ntuples (stack), and 
plot them (plot). Should probably be run from an empty directory. 
"""

_tag_list_help="""
Prints lists of files / datasets in a meta file. 
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
    tag_distill.add_argument(
        'd3pds', help='input d3pd dir OR file listing d3pds')
    tag_distill.add_argument('meta', help='meta yaml file')
    tag_distill.add_argument(
        '-o','--output-dir', default='whiskey', 
        help='output dir for distillates, ' + d)
    tag_distill.add_argument('--calibration', default='~/calibration', 
                             help=d)

    tag_list = tag_step.add_parser('list', description=_tag_list_help)
    tag_list.add_argument('meta_file')
    list_type = tag_list.add_mutually_exclusive_group(required=True)
    list_type.add_argument('--physics')
    tag_list.add_argument('-d', '--ntuples-dir')

    tag_agg = tag_step.add_parser('stack')
    tag_agg.add_argument(
        'whiskey_dir', help='input dir (or file)')
    tag_agg.add_argument('-o', '--output-dir', default='hists', 
                         help='output dir for hists, ' + d)

    tag_plot = tag_step.add_parser('plot')
    tag_plot.add_argument('input_hists', nargs='*')
    tag_plot.add_argument('-o', '--output-dir', default='plots', 
                          help='output dir for plots, ' + d)
    tag_plot.add_argument(
        '-b', '--binomial-error', action='store_true', 
        help=('calculate errors as binomial rather than using wt2 linear '
              'propagation'))
    return parser.parse_args(sys.argv[1:])

def jet_tag_efficinecy(config): 
    subs = {'distill':distill_d3pds,'stack':aggregate_jet_plots, 
            'plot':plot_jet_eff, 'list':list_meta_info}
    subs[config.step](config)

def list_meta_info(config): 
    from stop.meta import DatasetCache
    meta = DatasetCache(config.meta_file)
    filt_meta = {}
    if config.physics: 
        for ds_key, ds in meta.iteritems(): 
            if ds.physics_type == config.physics: 
                filt_meta[ds_key] = ds

    if config.ntuples_dir: 
        ntuples = glob.glob('{}/*.root*'.format(config.ntuples_dir))
        for ntuple in ntuples: 
            for ds in filt_meta.values(): 
                if str(ds.id) in ntuple: 
                    print ntuple
    else: 
        for ds in filt_meta.values(): 
            print ds.full_name

def distill_d3pds(config): 
    if isfile(config.d3pds): 
        with open(config.d3pds) as d3pd_file: 
            d3pd_contents = [l.strip() for l in d3pd_file.readlines()]
    else: 
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
    if isdir(config.whiskey_dir): 
        whiskey = glob.glob(join(config.whiskey_dir, '*.root'))
    elif isfile(config.whiskey_dir): 
        whiskey = [config.whiskey_dir]
    else:
        raise IOError("intput dir '{}' doesn't exist".format(whiskey_dir))
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
    from stop.performance.jeteff import JetEfficiencyPlotter
    if not isdir(config.output_dir): 
        os.mkdir(config.output_dir)
    wt2_error = not config.binomial_error
    plotter = JetEfficiencyPlotter(do_wt2_error=wt2_error)
    plotter.plot_samples(config.input_hists, out_dir=config.output_dir)
    

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
