#!/usr/bin/env python3
"""
Process fit output into hepdata input.
"""

import argparse
import yaml
from os.path import join, isdir
from os import mkdir

_inf = float('inf')
_nan = float('nan')

def run():
    d = 'default: %(default)s'
    b = dict(action='store_true') # boolian
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('cls_file')
    parser.add_argument(
        '-o', '--output-dir', default='signal_grid', help=d)
    args = parser.parse_args()
    if not isdir(args.output_dir):
        mkdir(args.output_dir)
    best_expected_name = join(args.output_dir, 'best_expected_sr.dat')
    _make_best_expected(args.cls_file, best_expected_name)
    _make_other_records(args.cls_file, args.output_dir)

class Point:
    def __init__(self, vals, reg):
        self.exp = vals.get('exp', _inf)
        self.obs = vals.get('obs', _inf)
        self.ul = vals.get('ul', _nan)
        self.xsec = vals.get('xsec', _nan)
        self.ulpb = self.ul * self.xsec
        self.sig_unct = vals.get('sig_unct', _nan)
        self.ms = vals['scharm_mass']
        self.ml = vals['lsp_mass']
        self.reg = reg
    def __str__(self):
        return '({ms},{ml}): {exp}, {obs}'.format(**vars(self))

def _sort_by_mass(point):
    """Defines a sorting order for points in the 2d plane"""
    return point.ms + point.ml * 10000

_best_expected = ('BEST EXPECTED SR', '{reg};',
                  'Best expected signal region for signal points')

def _make_best_expected(cls_file, out_file):
    with open(cls_file,'r') as cls:
        cls_dict = yaml.load(cls)
    best_points = {}
    # have to sort points to be consistent with limit plots when CLs values
    # are identical
    for regname in sorted(cls_dict):
        points = cls_dict[regname]
        for pdict in points:
            newpt = Point(pdict, regname)
            oldpt = best_points.get((newpt.ms, newpt.ml))
            # this '<=' is also needed to resolve identical CLs values
            keep = oldpt and oldpt.exp <= newpt.exp
            if not keep:
                best_points[newpt.ms, newpt.ml] = newpt

    point_list = sorted(best_points.values(), key=_sort_by_mass)
    _write_hepdata_file(point_list, out_file, [_best_expected])

_all_yheads = [
    ('EXPECTED CLS', '{exp:.6f};', 'Expected CLs for signal points'),
    ('OBSERVED CLS', '{obs:.6f};', 'Observed CLs for signal points'),
    ('UPPER LIMIT ON MODEL CROSS-SECTION IN PB', '{ulpb:.0f};',
     '95% CLs upper limit on model cross-section for signal points'),
    ('TOTAL SIGNAL UNCERTAINTY', '{sig_unct:.2f};',
     'Total experimental uncertainty for signal points'),
    ]

def _make_other_records(cls_file, out_dir):
    with open(cls_file,'r') as cls:
        cls_dict = yaml.load(cls)
    for regname, pdict in cls_dict.items():
        points = sorted([Point(x, regname) for x in pdict],key=_sort_by_mass)
        points = [x for x in points if (x.ms - x.ml) >= 80]
        for parameter, fmt, info in _all_yheads:
            outname = parameter.lower().replace(' ','_').replace('-','_')
            out_path = join(out_dir, '{}_{}.dat'.format(regname,outname))
            fmt_tuple = (parameter, fmt, info)
            _write_hepdata_file(points, out_path, [fmt_tuple], regname)

# ___________________________________________________________________________
# file writer

def _write_hepdata_file(point_list, out_file, yheads=_all_yheads, reg=None):
    """
    Build a hepdata input file.
    """
    comment_y_vals = ', '.join(c for _, _, c in yheads)
    if reg:
        comment_y_vals += ' in {}'.format(reg)
    out_lines = [
        '*dataset:', '*location: XXX',
        '*dscomment: {}'.format(comment_y_vals),
        '*qual: SQRT(S) IN GEV : 8000.0',
        '*xheader: M(SCHARM) IN GEV : M(NEUTRALINO1) IN GEV',
        '*yheader: ' + ' : '.join(x for x, _, _ in  yheads),
        '*data: x : x' + ' : y'*len(yheads) ]
    tmp = ' {ms: 4.0f}; {ml: 4.0f}; ' + ' '.join(y for _, y, _ in yheads)
    for point in point_list:

        # exclude failed fits
        if point.exp < 0.0:
            continue

        vals = vars(point)
        line = tmp.format(**vals).replace('nan',' - ')
        out_lines.append(line)
    out_lines.append('*dataend:')
    with open(out_file,'w') as out:
        out.writelines(x + '\n' for x in out_lines)


if __name__ == '__main__':
    run()
