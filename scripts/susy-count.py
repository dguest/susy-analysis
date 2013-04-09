#!/usr/bin/env python2.7

import argparse
import yaml 
from stop.stack.table import make_latex_bg_table, unyamlize
import sys
from tempfile import TemporaryFile

def get_config(): 
    d = 'default: %(default)s'
    c = "with no argument is '%(const)s'"

    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('config_file', help="yaml file")
    parser.add_argument('-t', '--dump-tex', action='store_true')
    parser.add_argument('--systematics', action='store_true')
    parser.add_argument(
        '-s','--signal-point', default='stop-150-90', 
        help="assumes <particle>-<something> type name, " + d)
    parser.add_argument('-f', '--filters', nargs='*', default=[])
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
