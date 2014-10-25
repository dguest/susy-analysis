#!/usr/bin/env python3
"""
dump info from fit inputs
"""

import argparse
import yaml
import re

def get_args():
    d = '(default: "%(default)s")'
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('fit_inputs')
    parser.add_argument('systematic', nargs='?', default='jes', help=d)
    parser.add_argument('-p', '--process', default='scharm-', help=d)
    parser.add_argument('-r', '--region', default='signal_mct150', help=d)
    return parser.parse_args()

def run():
    args = get_args()
    with open(args.fit_inputs) as yml:
        fit_inputs = yaml.load(yml)

    proc = args.process
    syst = args.systematic
    nominal = _get_yld(fit_inputs['nominal_yields'][args.region], proc)

    yld_systs = fit_inputs['yield_systematics']
    if syst in yld_systs:
        systs = _get_yld(yld_systs[syst][args.region], proc)
        for proc in sorted(systs):
            var = systs[proc]
            nom = nominal[proc]
            print('{:>10}: {:.2f}'.format(proc, (var - nom) / nom))

    elif all(syst + x in yld_systs for x in ['up','down']):
        sysup = _get_yld(yld_systs[syst + 'up'][args.region], proc)
        sysdn = _get_yld(yld_systs[syst + 'down'][args.region], proc)
        for proc in sorted(sysup):
            up = sysup[proc]
            down = sysdn[proc]
            nom = nominal[proc]
            relup = (up - nom) / nom
            reldn = (down - nom) / nom
            avg = (relup - reldn) / 2
            dm = _get_deltam(proc) or 'bg'
            print('{:>20} {dm:4}: {: 4.2f} {: 4.2f}, {:4.2f}'.format(
                    proc, reldn, relup, avg, dm=dm))

def _get_yld(region, process):
    sysdic = {}
    for proc, (yld,*oth) in region.items():
        if process in proc:
            sysdic[proc] = yld
    return sysdic

_dm_re = re.compile('\D+-(\d+)-(\d+)')
def _get_deltam(proc):
    try:
        msch, mlsp = _dm_re.search(proc).groups()
        return int(msch) - int(mlsp)
    except AttributeError:
        return None

if __name__ == '__main__':
    run()
