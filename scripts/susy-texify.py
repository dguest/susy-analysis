#!/usr/bin/env python2.7

import argparse
import yaml 
from stop.stack.table import make_latex_bg_table, unyamlize
from stop.stack.table import LatexCutsConfig
from stop.systematics import transfer, tex

import sys
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

def get_max_signal_contamination(phys_cut_dict, min_dm=30): 
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
        old_max, old_name = maxes.get(region, (-1.0,''))
        sig_number = phys_cut_dict[sig, region]
        if sig_number > old_max: 
            if not sig_number: 
                maxes[region] = (0.0, '--')
            else: 
                maxes[region] = (sig_number, sig)
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
        }[args.which]
    action(args)

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
