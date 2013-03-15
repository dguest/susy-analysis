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
    parser.add_argument('counts_file', help="yaml file")
    parser.add_argument('-t', '--dump-tex', action='store_true')
    parser.add_argument('--systematics', action='store_true')
    args = parser.parse_args(sys.argv[1:])
    return args

def run(): 
    args = get_config()

    with open(args.counts_file) as counts: 
        counts_dict = yaml.load(counts)

    base_systematic = counts_dict['NONE']
    all_systematics = set(counts_dict.keys()) - set(['NONE'])

    out_file = TemporaryFile()
    make_latex_bg_table(unyamlize(base_systematic), title='baseline', 
                        out_file=out_file)

    if args.systematics: 
        for systematic in all_systematics: 
            syst_dict = counts_dict[systematic]
            phys_cut_dict = unyamlize(syst_dict)
            make_latex_bg_table(phys_cut_dict, title=systematic, 
                                out_file=out_file)

    out_file.seek(0)
    for line in out_file: 
        print line.strip()

if __name__ == '__main__': 
    run()
