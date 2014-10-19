#!/usr/bin/env python3
"""
texify yaml yield files
"""

import argparse
import yaml
import sys
from itertools import product
from collections import Counter

def get_config():
    d = 'default: %(default)s'
    c = "with no argument is '%(const)s'"

    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('yield_file')
    args = parser.parse_args(sys.argv[1:])
    return args

_reg_schema = {
    'sra':'signal',
    'crz':'cr_z',
    'crt':'cr_df_leptmet',
    'crw':'cr_1l_leptmet'
    }
_proc_schema = {
    'wjet':'Wjets',
    'zjet':'Zjets',
    'top':'top',
    'bos':'other',
    }

def run():
    args = get_config()
    with open(args.yield_file) as yml:
        yields = yaml.load(yml)['nominal_yields']
    totals = Counter()
    tmp = r'\newcommand{{\{reg}{proc}}}{{{yld:.1f}}}'
    for reg, proc in product(_reg_schema, _proc_schema):
        yld = yields[_reg_schema[reg]][_proc_schema[proc]][0]
        totals[reg] += yld
        cmd = tmp.format(reg=reg, proc=proc, yld=yld)
        print(cmd)
    for reg, count in totals.items():
        print(tmp.format(reg=reg, proc='tot', yld=count))


if __name__ == '__main__':
    run()
