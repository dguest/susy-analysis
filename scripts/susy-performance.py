#!/usr/bin/env python2.7
"""
This is intended to be the top level script for various performance scripts. 
"""

_tag_help="""
Routine to run over d3pds (distill) condense resulting ntuples (stack), and 
plot them (plot). Should probably be run from an empty directory. 
"""

_ratio_help="""
Computes the ratio between two efficiencies. Output is either plotted or 
dumped as yaml
"""

_distill_help="""
Produces one output file for the dir or textfile given. 
If a textfile is given, it should contain a list of root files. If a 
directory is given, it should be for one entire dataset. 
"""

import argparse, sys
import glob
from os.path import isdir, isfile, join, expanduser, splitext
from os.path import dirname, basename, abspath
import os
import re
import yaml
import warnings

import h5py
from h5py import Group, Dataset
from stop.hists import HistNd

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
        '-f','--filter-output', action='store_true', 
        help=('apply same output filter as is applied to the search skims'
              ' (for compatibility)'))
    tag_distill.add_argument(
        '-o','--output-dir', default='whiskey', 
        help='output dir for distillates, ' + d)
    tag_distill.add_argument('--calibration', default='~/calibration', 
                             help=d)

    tag_agg = tag_step.add_parser('stack')
    tag_agg.add_argument(
        'whiskey_dir', help='input dir (or file)')
    tag_agg.add_argument('-o', '--output-dir', default='hists', 
                         help='output dir for hists, ' + d)

    tag_plot = tag_step.add_parser('plot')
    tag_plot.add_argument('input_hists', nargs='*')
    tag_plot.add_argument('-o', '--output-dir', default='plots', 
                          help='output dir for plots, ' + d)
    tag_plot.add_argument('-b', '--binned', action='store_true', 
                          help='bin by flavortag bins')
    tag_plot.add_argument(
        '--binomial-error', action='store_true', 
        help=('calculate errors as binomial rather than using wt2 linear '
              'propagation'))
    tag_plot.add_argument('--ext', help='plot extension ' + d, default='.pdf')

    eff_ratio = tag_step.add_parser('ratio', description=_ratio_help)
    eff_ratio.add_argument('-n','--numerators', nargs='+')
    eff_ratio.add_argument('-d','--denominators', nargs='+')
    eff_ratio.add_argument('-o','--plot-dir')
    eff_ratio.add_argument('--ext', help='plot extension ' + d, 
                           default='.pdf')

    return parser.parse_args(sys.argv[1:])

# --------------------------------------------------------------------
# --- these guys should be moved into some other more general routine 
#     (call it susy-util or something)
# --------------------------------------------------------------------


# ------------------- end of things to move ------------------------

def jet_tag_efficinecy(config): 
    subs = {'distill':distill_d3pds,'stack':aggregate_jet_plots, 
            'plot':plot_jet_eff, 'ratio': jet_eff_ratio}
    subs[config.step](config)


def distill_d3pds(config): 
    if isfile(config.d3pds): 
        with open(config.d3pds) as d3pd_file: 
            files = [l.strip() for l in d3pd_file.readlines()]
        out_file = splitext(basename(config.d3pds))[0] + '.root'
    else: 
        out_file = _ntuple_name_from_ds_name(dirname(config.d3pds))
        files = glob.glob('{}/*'.format(config.d3pds))
    calibration_dir = expanduser(config.calibration)
    btag_env = join(calibration_dir, 'BTagCalibration.env')
    if not files: 
        return 
    if not isdir(config.output_dir): 
        os.mkdir(config.output_dir)
    meta_lookup = meta.DatasetCache(config.meta)

    ds_key = basename(out_file).split('-')[0]

    out_path = join(config.output_dir, out_file)
    flags = 'v'             # verbose
    if not config.filter_output: 
        flags += 'e'        # save all events
    if _is_atlfast(meta_lookup[ds_key].full_name): 
        flags += 'f'
    cut_counts = cutflow.cutflow(
        input_files=files, flags=flags, output_ntuple=out_path, 
        btag_cal_file=btag_env, cal_dir=calibration_dir)
    counts_path = splitext(out_path)[0] + '_counts.yml'
    list_counts = [list(c) for c in cut_counts]
    with open(counts_path,'w') as out_yml: 
        out_yml.write(yaml.dump(list_counts))

def aggregate_jet_plots(config): 
    from stop import hyperstack
    input_dir = config.whiskey_dir
    if isdir(config.whiskey_dir): 
        whiskey = glob.glob(join(config.whiskey_dir, '*.root'))
    elif isfile(config.whiskey_dir) and config.whiskey_dir.endswith('.txt'): 
        with open(config.whiskey_dir) as files: 
            whiskey = [l.strip() for l in files.readlines()]
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
        path = abspath(tup)
        if not isfile(path): 
            raise IOError("{} can't be found".format(path))
        hyperstack.stacksusy(
            input_file=abspath(tup), region_list=[region_dict], flags='v')
                      
def plot_jet_eff(config): 
    from stop.performance.jeteff import JetEfficiencyPlotter
    from stop.plot.efficiency import EfficiencyPlot, BinnedEfficiencyPlot
    if not isdir(config.output_dir): 
        os.mkdir(config.output_dir)
    wt2_error = not config.binomial_error
    plotter = EfficiencyPlot
    draw_bins = 'jf'
    if config.binned: 
        plotter = BinnedEfficiencyPlot
        draw_bins = ''
    outer_plotter = JetEfficiencyPlotter(do_wt2_error=wt2_error, 
                                         draw_bins=draw_bins)
    outer_plotter.plot_samples(config.input_hists, out_dir=config.output_dir, 
                               plotter=plotter, plot_extension=config.ext)

def jet_eff_ratio(config): 
    from stop.performance.jeteff import JetEffRatioCalc, JetRatioPlotter
    from stop.performance.jeteff import JetRatioDumper
    calc = JetEffRatioCalc()
    rat_dict = calc.get_ratios(config.numerators, config.denominators)
    if config.plot_dir: 
        if not isdir(config.plot_dir): 
            os.mkdir(config.plot_dir)
        
        plotter = JetRatioPlotter(rat_dict)
        
        plotter.overlay_numerators(config.plot_dir, config.ext)

    else: 
        dumper = JetRatioDumper(rat_dict)
        # print dumper.as_xml()
        print yaml.dump(dumper.as_nested_dict(),
                        default_flow_style=False)

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
