#!/usr/bin/env python3.3
"""
Merge several cls files. Will throw exceptions if duplicate info is found.
"""
_overwrite_help = 'allow duplicate info (earlier file is overwritten)'
import argparse
import yaml
import sys

from scharm import datasets

def get_args():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('cls_files', nargs='+')
    parser.add_argument('-a', '--allow-overwrite', action='store_false',
                        dest='noover', help=_overwrite_help)
    return parser.parse_args()

def _file_iter(file_list):
    for cfile in file_list:
        with open(cfile) as yml:
            yield _flattify(yaml.load(yml))

def _flattify(cls_dict):
    cls_flat = {}
    for conf, points in cls_dict.items():
        for pt in points:
            cls_flat[conf, pt['scharm_mass'], pt['lsp_mass']] = pt
    return cls_flat

def _throw_if_overwrite(point, newpt):
    dup_pars = set(point) & set(newpt)
    bad_dup = dup_pars - set(['scharm_mass', 'lsp_mass'])
    if bad_dup:
        tmp = 'point ({scharm_mass}, {lsp_mass}): tried to overwrite "{}"'
        raise ValueError(tmp.format('", "'.join(bad_dup),**point))

def run():
    args = get_args()
    with open(args.cls_files[0]) as yml:
        points = _flattify(yaml.load(yml))

    for newpts in _file_iter(args.cls_files[1:]):
        dup_pts = set(points) & set(newpts)
        for dup in dup_pts:
            if args.noover:
                _throw_if_overwrite(points[dup],newpts[dup])
            points[dup].update(newpts[dup])

if __name__ == '__main__':
    run()
