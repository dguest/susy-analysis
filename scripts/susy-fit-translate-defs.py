#!/usr/bin/env python3
"""
Translate yields / systematics tables to have better names
"""

import argparse, sys, tempfile, re
from scharm.limits.limitsty import long_alpha_names, mu_names, reg_names

_transdics = {
    'alpha\_': long_alpha_names,
    'mu\_': {k:v + ' normalization' for k, v in mu_names.items()},
    }

def _transgamma(line):
    head, tail = line.split('&', 1)
    for region, long_name in reg_names.items():
        esc_name = region.replace('_',r'\_')
        if r'_stat\_' + esc_name in head:
            return long_name + ' stat error & ' + tail

# HACK: hardcoded multijet numbers
_multijet_events = [(0.34, 0.13), (0.21, 0.04), (0.05, 0.02)]
def _get_mjline(multijet_events=_multijet_events):
    multijet_tex = [r'${} \pm {}$'.format(*x) for x in multijet_events]
    mjhead, mjsep = '      Multijet events', '         &  '
    return mjsep.join([mjhead] + multijet_tex) + r'\\'

def _transline(line):
    if 'MULTIJET_FILLER' in line:
        return _get_mjline()
    for head, dic in _transdics.items():
        if line.startswith(head):
            stem = line[len(head):].split('&')[0].strip()
            rest = line.split('&')[1:]
            return ' & '.join([dic.get(stem.replace(r'\_','_'),stem)] + rest)
    if 'Uncertainty of channel' in line or line.startswith(r'{\bf  channel}'):
        head, mail = line.split('&',1)
        mid, *tail = mail.partition(r'\\')
        oldregs = mid.split('&')
        def transreg(reg):
            return reg_names.get(reg.replace(r'\_','_').strip(), reg)
        newregs = [transreg(x) for x in oldregs]
        return ' & '.join([head.strip()] + newregs) + ' '.join(tail) + '\n'
    if line.startswith(r'gamma\_'):
        return _transgamma(line)
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
