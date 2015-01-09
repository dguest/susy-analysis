#!/usr/bin/env python3
"""
Process fit output into hepdata input.
"""

import argparse
import yaml

_inf = float('inf')

def run():
    d = 'default: %(default)s'
    b = dict(action='store_true') # boolian
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('cls_file')
    parser.add_argument('-o', '--output-file', default='.txt')
    args = parser.parse_args()
    _make_best_expected(args.cls_file, args.output_file)

class Point:
    def __init__(self, vals, reg):
        self.exp = vals.get('exp', _inf)
        self.obs = vals.get('obs', _inf)
        self.ul = vals.get('ul')
        self.xsec = vals.get('xsec')
        self.ms = vals['scharm_mass']
        self.ml = vals['lsp_mass']
        self.reg = reg
    def __str__(self):
        return '({ms},{ml}): {exp}, {obs}'.format(**vars(self))

def _sort_by_mass(point):
    """Defines a sorting order for points in the 2d plane"""
    return point.ms + point.ml * 10000

def _make_best_expected(cls_file, out_file):
    with open(cls_file,'r') as cls:
        cls_dict = yaml.load(cls)
    best_points = {}
    for regname, points in cls_dict.items():
        for pdict in points:
            newpt = Point(pdict, regname)
            oldpt = best_points.get((newpt.ms, newpt.ml))
            keep = oldpt and oldpt.exp < newpt.exp
            if not keep:
                best_points[newpt.ms, newpt.ml] = newpt

    point_list = sorted(best_points.values(), key=_sort_by_mass)
    _write_hepdata_file(point_list, out_file)

def _write_hepdata_file(point_list, out_file):
    """
    Build a hepdata input file.
    """
    yheads = [
        ('Expected CLs', '{exp:.6f};'),
        ('Observed CLs', '{obs:.6f};'),
        ('UPPER LIMIT ON MOCEL CROSS-SECTION IN PB', '{ulpb};'),
        ('BEST EXPECTED SR', '{reg};'),
        ]
    out_lines = [
        '*dataset:', '*location: XXX',
        '*dscomment: XXX',
        '*qual: SQRT(S) IN GEV : 8000.0',
        '*xheader: M(SCHARM) IN GEV : M(NEUTRALINO1) IN GEV',
        '*yheader: ' + ' : '.join(x for x, _ in  yheads),
        '*data: x : x' + ' : y'*len(yheads) ]
    tmp = ' {ms: 4.0f}; {ml: 4.0f}; ' + ' '.join(y for _, y in yheads)
    for point in point_list:

        if point.exp < 0.0:
            continue
        vals = vars(point)
        vals['ulpb'] = ' - '
        if point.ul is not None:
            vals['ulpb'] = '{:.1f}'.format(point.ul * point.xsec)
        line = tmp.format(**vals)
        out_lines.append(line)
    out_lines.append('*dataend:')
    with open(out_file,'w') as out:
        out.writelines(x + '\n' for x in out_lines)


if __name__ == '__main__':
    run()
