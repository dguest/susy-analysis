#!/usr/bin/env python3.3
"""
Plotting routine for the fit results (which are produced as a yaml file).
"""

import argparse, sys, tempfile
from scharm.limits.limitsty import alpha_names

_transdics = {
    'mu\_': alpha_names
    }

def _transline(line):
    for head, dic in _transdics.items():
        if line.startswith(exthead):
            stem = line[len(exthead):].split('&').strip()
            return dic.get(stem,stem)


def run():
    """top level routine"""
    args = _get_args()
    pars_file = _get_fit_pars(args.tex_file)
    if not args.tex_file:
        out_file = sys.stdout
    else:
        out = tempfile.TemporaryFile()
    for line in pars_file:

def _get_args():
    """input parser"""
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        'tex_file', help='any tex file', nargs='?')
    parser.add_argument('-e','--ext', default='.pdf', help='plot type')
    parser.add_argument('-l','--lumi', action='store_true')
    return parser.parse_args()

def _get_fit_pars(fit_parameters):
    if not fit_parameters and not sys.stdin.isatty():
        return sys.stdin

    return open(fit_parameters, 'r+')

if __name__ == '__main__':
    run()
