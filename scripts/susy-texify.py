#!/usr/bin/env python2.7

import argparse
import yaml 
from stop.stack.table import make_latex_bg_table, unyamlize
from stop.stack.table import LatexCutsConfig
import sys
from tempfile import TemporaryFile

def get_config(): 
    d = 'default: %(default)s'
    c = "with no argument is '%(const)s'"

    parser = argparse.ArgumentParser(description=__doc__)
    subs = parser.add_subparsers(dest='which')
    counts = subs.add_parser('counts')
    counts.add_argument('config_file', help="yaml file")
    counts.add_argument('--systematics', action='store_true')
    counts.add_argument(
        '-s','--signal-point', default='stop-150-90', 
        help="assumes <particle>-<something> type name, " + d)
    counts.add_argument('-f', '--filters', nargs='*', default=[])

    regions = subs.add_parser('regions')
    regions.add_argument('config_file')

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


def run(): 
    args = get_config()
    action = {
        'counts': get_counts, 
        'regions': get_regions, 
        }[args.which]
    action(args)

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

    needed = get_signal_finder(args.signal_point)
    filtered = {}
    for syst, cutdict in counts_dict.iteritems(): 
        filtered[syst] = {n:c for n, c in cutdict.iteritems() if needed(n)}

    base_systematic = filtered['NONE']
    all_systematics = set(filtered.keys()) - set(['NONE'])

    out_file = TemporaryFile()
    phys_cut_dict = unyamlize(base_systematic)

    killkeys = set()
    used_physics = set(config_dict['backgrounds']['used'] + ['data'])
    for phys, region in phys_cut_dict: 
        if not phys in used_physics: 
            killkeys.add( (phys, region) )
        for filt in args.filters: 
            if filt in region: 
                killkeys.add( (phys,region))
        
    for key in killkeys: 
        del phys_cut_dict[key]
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
