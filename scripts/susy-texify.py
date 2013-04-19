#!/usr/bin/env python2.7

import argparse
import yaml 
from stop.stack.table import make_latex_bg_table, unyamlize
from stop.stack.table import LatexCutsConfig
from stop.systematics import transfer, tex

import sys, re
from tempfile import TemporaryFile

def get_config(): 
    d = 'default: %(default)s'
    c = "with no argument is '%(const)s'"

    parser = argparse.ArgumentParser(description=__doc__)

    shared_parser = argparse.ArgumentParser(add_help=False)
    shared_parser.add_argument('config_file', help='input yaml file')

    subs = parser.add_subparsers(dest='which')
    counts = subs.add_parser('counts', parents=[shared_parser])
    counts.add_argument('--systematics', action='store_true')
    count_signal_mode = counts.add_mutually_exclusive_group()
    count_signal_mode.add_argument(
        '-s','--signal-point', default='stop-150-90', 
        help="assumes <particle>-<something> type name, " + d)
    count_signal_mode.add_argument('-m','--max-point', action='store_true')
    counts.add_argument('-f', '--filters', nargs='+', default=[])

    regions = subs.add_parser('regions', parents=[shared_parser])

    transfer = subs.add_parser('transfer', parents=[shared_parser])
    transfer.add_argument('-p','--phys-type', default='ttbar', help=d)
    transfer.add_argument('-r','--rel-errors', action='store_true')
    transfer.add_argument('-x','--exclude', nargs='+')
    transfer.add_argument('-o','--only', nargs='+')
    transfer.add_argument('--green', type=float, default=0.2, help=d)

    filt_counts = subs.add_parser('filter', parents=[shared_parser])
    filt_counts.add_argument('--only-systematic')
    filt_counts.add_argument('--region-regex')
    filt_counts.add_argument('--region-veto-regex')
    filt_counts.add_argument('--physics-regex')
    filt_counts.add_argument('--physics-veto-regex') 
    filt_counts.add_argument('-i','--overwrite', action='store_true')

    args = parser.parse_args(sys.argv[1:])
    return args

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

def run(): 
    args = get_config()
    action = {
        'counts': get_counts, 
        'regions': get_regions, 
        'transfer': get_transfer, 
        'filter': filter_counts, 
        }[args.which]
    action(args)

def filter_counts(args): 
    with open(args.config_file) as config_yml: 
        config = yaml.load(config_yml)
    counts_file = config['files']['counts']
    with open(counts_file) as counts_yml: 
        counts = yaml.load(counts_yml)
    kept = {}
    if args.only_systematic: 
        counts = {args.only_systematic: counts[args.only_systematic]}

    region_regex = region_veto_regex = None
    if args.region_regex: 
        region_regex = re.compile(args.region_regex)
    if args.region_veto_regex: 
        region_veto_regex = re.compile(args.region_veto_regex)

    physics_regex = physics_veto_regex = None
    if args.physics_regex: 
        physics_regex = re.compile(args.physics_regex)
    if args.physics_veto_regex: 
        physics_veto_regex = re.compile(args.physics_veto_regex)

    for syst_name, phys_dict in counts.iteritems(): 
        kept.setdefault(syst_name, {})
        for phys_name, reg_dict in phys_dict.iteritems(): 
            if physics_regex and not physics_regex.search(phys_name): 
                continue
            if physics_veto_regex and physics_veto_regex.search(phys_name): 
                continue
            kept[syst_name].setdefault(phys_name, {})
            for reg_name, counts_dict in reg_dict.iteritems(): 
                if region_regex and not region_regex.search(reg_name): 
                    continue
                if region_veto_regex and region_veto_regex.search(reg_name): 
                    continue
                kept[syst_name][phys_name][reg_name] = counts_dict

    if args.overwrite: 
        with open(counts_file, 'w') as counts_yml: 
            counts_yml.write(yaml.dump(kept))
    else: 
        print yaml.dump(kept)

def get_transfer(args): 
    with open(args.config_file) as config_yml: 
        config = yaml.load(config_yml)
    counts_file = config['files']['counts']
    with open(counts_file) as counts_yml: 
        counts = yaml.load(counts_yml)
    table = transfer.TransferTable(config, counts)
    trans_factors = table.get_tf_table(args.phys_type)
    if args.only: 
        def filt(k): 
            for pat in args.only: 
                if pat in k: 
                    return True
            return False
        trans_factors = {
            k:v for k,v in trans_factors.iteritems() if filt(k)}

    if not args.rel_errors: 
        printer = tex.TransferFactorTable(trans_factors)
        printer.green_threshold = args.green
        out = TemporaryFile()
        printer.write(out)
        out.seek(0)
        for line in out: 
            print line.strip()
    else: 
        rel_print = tex.TransferFactorRelitiveErrorTable(trans_factors)
        out = TemporaryFile()
        rel_print.write(out)
        out.seek(0)
        for line in out: 
            print line.strip()

def get_regions(args): 
    cuts_config = LatexCutsConfig()
    with open(args.config_file) as config: 
        config_dict = yaml.load(config)
    latex_config = cuts_config.latex_config_file(config_dict)
    latex_config.seek(0)
    for line in latex_config: 
        print line.strip()

def get_counts(args): 
    with open(args.config_file) as config: 
        config_dict = yaml.load(config)
        counts_file = config_dict['files']['counts']

    with open(counts_file) as counts: 
        counts_dict = yaml.load(counts)

    sig_point = '' if args.max_point else args.signal_point
    needed = get_signal_finder(sig_point)
    filtered = {}
    for syst, cutdict in counts_dict.iteritems(): 
        filtered[syst] = {n:c for n, c in cutdict.iteritems() if needed(n)}

    base_systematic = filtered['NONE']
    all_systematics = set(filtered.keys()) - set(['NONE'])

    out_file = TemporaryFile()
    phys_cut_dict = unyamlize(base_systematic)

    killkeys = set()
    used_physics = set(config_dict['backgrounds']['used'] + ['data'])
    used_physics |= set(k[0] for k in phys_cut_dict if k[0].startswith('stop'))
    for phys, region in phys_cut_dict: 
        if not phys in used_physics: 
            killkeys.add( (phys, region) )
        for filt in args.filters: 
            if filt in region: 
                killkeys.add( (phys,region))
        
    for key in killkeys: 
        del phys_cut_dict[key]

    if args.max_point: 
        update_with_max_contamination(phys_cut_dict)

    make_latex_bg_table(phys_cut_dict, title='baseline', 
                        out_file=out_file)

    if args.systematics: 
        for systematic in all_systematics: 
            syst_dict = filtered[systematic]
            phys_cut_dict = unyamlize(syst_dict)
            make_latex_bg_table(phys_cut_dict, title=systematic, 
                                out_file=out_file)

    out_file.seek(0)
    for line in out_file: 
        print line.strip()

if __name__ == '__main__': 
    run()
