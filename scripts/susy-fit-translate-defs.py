#!/usr/bin/env python3.3
"""
Plotting routine for the fit results (which are produced as a yaml file).
"""

import argparse, sys, tempfile
from scharm.limits.limitsty import long_alpha_names, mu_names

_transdics = {
    'alpha\_': long_alpha_names,
    'mu\_': mu_names,
    }
def _transline(line):
    for head, dic in _transdics.items():
        if line.startswith(head):
            stem = line[len(head):].split('&')[0].strip()
            rest = line.split('&')[1:]
            return ' & '.join([dic.get(stem,stem)] + rest)
    return line

def run():
    """top level routine"""
    args = _get_args()
    pars_file = _get_fit_pars(args.tex_file)
    if args.tex_file and args.in_place:
        out = tempfile.TemporaryFile('w+')
    else:
        if args.in_place:
            sys.exit('need a file to change in place')
        out = sys.stdout
    for line in pars_file:
        out.write(_transline(line))

    if args.in_place:
        pars_file.seek(0)
        pars_file.truncate()
        out.seek(0)
        for line in out:
            pars_file.write(line)

def _get_args():
    """input parser"""
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        'tex_file', help='any tex file', nargs='?')
    parser.add_argument('-i', '--in-place', action='store_true')
    return parser.parse_args()

def _get_fit_pars(fit_parameters):
    if not fit_parameters and not sys.stdin.isatty():
        return sys.stdin

    return open(fit_parameters, 'r+')

if __name__ == '__main__':
    run()
