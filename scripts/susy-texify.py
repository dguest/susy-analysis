#!/usr/bin/env python2.7
"""
Part of the main analysis suite of routines. This prints the counts / 
stacking steering file info to latex. 
"""

import argparse
import yaml 
from stop.stack.table import make_latex_bg_table, unyamlize, make_marktable
from stop.stack.table import LatexCutsConfig, FrameWrap, TalkWrap
from stop.stack.aggregator import HistDict
from collections import defaultdict

import sys, re
from tempfile import TemporaryFile

def get_config(): 
    d = 'default: %(default)s'
    c = "with no argument is '%(const)s'"

    parser = argparse.ArgumentParser(description=__doc__)

    subs = parser.add_subparsers(dest='which')
    counts = subs.add_parser('counts')
    counts.add_argument('aggregate')
    counts.add_argument('fits_file', nargs='?', help='yaml fits steering file')
    counts.add_argument('cuts_file', nargs='?', help='SR cuts file')
    count_signal_mode = counts.add_mutually_exclusive_group()
    count_signal_mode.add_argument(
        '-s','--signal-point', const='stop-150-90', nargs='?', 
        help="assumes <particle>-<something> type name, " + c)
    count_signal_mode.add_argument('-m','--max-point', action='store_true')
    counts.add_argument('-f', '--filters', nargs='+', default=[])

    regions = subs.add_parser('regions')
    regions.add_argument('config_file', help='input yaml file')

    args = parser.parse_args(sys.argv[1:])
    return args

def run(): 
    args = get_config()
    action = {
        'counts': get_counts, 
        'regions': get_regions, 
        }[args.which]
    action(args)

def get_signal_finder(signal_point): 
    if signal_point: 
        signal_head = signal_point.split('-')[0]
        def needed(phys): 
            if not phys.startswith(signal_head): 
                return True
            if phys == signal_point:
                return True
            return False
    else: 
        def needed(tup): 
            return True
    return needed

def get_max_signal_contamination(phys_cut_dict, min_dm=0): 
    def check_for_signal(key): 
        if not key[0].startswith('stop'): 
            return False
        st, mstop, mlsp = key[0].split('-')
        if int(mstop) - int(mlsp) > min_dm: 
            return True
        return False
    signal_keys = [k for k in phys_cut_dict if check_for_signal(k)]

    maxes = {}
    for sig, region in signal_keys: 
        old_max, old_name = maxes.get(region, ({'normed':-1.0},''))
        sig_number = phys_cut_dict[sig, region]
        if sig_number['normed'] > old_max['normed']: 
            if not sig_number['normed']: 
                maxes[region] = ({'normed':0.0,'stats':0.0}, '--')
            else: 
                maxes[region] = (sig_number, sig.replace('stop-',''))
    return maxes

def update_with_max_contamination(phys_cut_dict): 
    maxes = get_max_signal_contamination(phys_cut_dict)
    for k in phys_cut_dict.keys(): 
        if k[0].startswith('stop'): 
            del phys_cut_dict[k]
    for region, (sig_number, sig) in maxes.iteritems(): 
        phys_cut_dict['stop-max',region] = sig_number
        phys_cut_dict['stop-name',region] = sig


def get_regions(args): 
    cuts_config = LatexCutsConfig()
    with open(args.config_file) as config: 
        config_dict = yaml.load(config)
    latex_config = cuts_config.latex_config_file(config_dict)
    latex_config.seek(0)
    for line in latex_config: 
        print line.strip()

def get_counts(args): 
    if args.fits_file: 
        with open(args.fits_file) as fits: 
            all_configs = yaml.load(fits)
    else: 
        from stop.runtypes import marks_types
        all_configs = {
            'blork': { 
                'backgrounds':list(marks_types), 
                'control_regions':['Znunu0', 'Wenu0', 'Wmunu0', 'ttbar0'], 
                'signal_region': 'SR0'
                }
            }
    phys_set = set(['data'])
    for config in all_configs.values(): 
        phys_set |= set(config['backgrounds'])

    if args.signal_point:
        phys_set.add(args.signal_point)

    cuts = None
    if args.cuts_file: 
        with open(args.cuts_file) as cuts_yml: 
            cuts = {c['region_key']:c for c in yaml.load(cuts_yml)}

    hists = HistDict(args.aggregate, 
                     filt='kinematic', 
                     physics_set=phys_set, 
                     )

    out_file = TemporaryFile()
    with TalkWrap(out_file): 
        for config_name, config in all_configs.iteritems(): 
            sr_cuts = cuts.get(config_name,None)
            make_table(hists, config_name, config, args.max_point, out_file, 
                       cuts=sr_cuts)
    out_file.seek(0)
    for line in out_file: 
        print line.strip()

def make_table(hists, config_name, config, max_point=False, out_file=None,
               cuts=None): 
    GeV = 1000.0
    inf = float('inf')
    def get_count(hist, met=150, ljpt=150): 
        met_slice = hist['met'].slice(met*GeV,inf)
        return float(met_slice['leadingJetPt'].slice(ljpt*GeV,inf))

    phys_cut_dict = defaultdict(dict)
    physics_set = set(config['backgrounds']) 
    physics_set.add('data')
    region_set = set(config['control_regions'])
    if cuts: 
        region_set.add(config['signal_region'])
                     
    for (phys, var, region), hist in hists.iteritems(): 
        if region == config['signal_region'] and cuts: 
            met_cut = cuts['met']
            ljpt_cut = cuts['leading_jet_pt']
        else: 
            met_cut = ljpt_cut = 150

        def get_count(hist): 
            met_slice = hist['met'].slice(met_cut*GeV,inf)
            return float(met_slice['leadingJetPt'].slice(ljpt_cut*GeV,inf))
                
        if phys not in physics_set or region not in region_set: 
            continue
        if var == 'kinematics': 
            phys_cut_dict[phys, region]['normed'] = get_count(hist)
        else: 
            phys_cut_dict[phys, region]['wt2'] = get_count(hist)

    phys_cut_dict = dict(phys_cut_dict)
    for key in phys_cut_dict: 
        if not 'wt2' in phys_cut_dict[key]: 
            phys_cut_dict[key] = phys_cut_dict[key]['normed']

    if max_point: 
        update_with_max_contamination(phys_cut_dict)

    if not out_file: 
        out_file = TemporaryFile()
    with FrameWrap(out_file): 
        make_marktable(phys_cut_dict, title=config_name, 
                       out_file=out_file)

if __name__ == '__main__': 
    run()
