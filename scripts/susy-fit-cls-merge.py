#!/usr/bin/env python3
"""
Merge several cls files. Will throw exceptions if duplicate info is found.
"""
_overwrite_help = 'allow duplicate info (earlier file is overwritten)'
_veto_help = 'veto XXX-YYY formatted list of points'
import argparse
import yaml
import sys

from scharm import datasets

def get_args():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('cls_files', nargs='+')
    parser.add_argument('-v', '--veto-bad-points', help=_veto_help)
    parser.add_argument('-a', '--allow-overwrite', action='store_false',
                        dest='noover', help=_overwrite_help)
    return parser.parse_args()

def _file_iter(file_list, veto_set):
    for cfile in file_list:
        if cfile:
            with open(cfile) as yml:
                yield _flattify(yaml.load(yml), veto_set)

def _flattify(cls_dict, veto_set):
    cls_flat = {}
    for conf, points in cls_dict.items():
        for pt in points:
            ms, ml = pt['scharm_mass'], pt['lsp_mass']
            if (ms, ml) in veto_set:
                continue
            cls_flat[conf, ms, ml] = pt
    return cls_flat

def _throw_if_overwrite(point, newpt):
    if None in [point, newpt]:
        return
    dup_pars = set(point) & set(newpt)
    bad_dup = dup_pars - set(['scharm_mass', 'lsp_mass'])
    if bad_dup:
        tmp = 'point ({scharm_mass}, {lsp_mass}): tried to overwrite "{}"'
        raise ValueError(tmp.format('", "'.join(bad_dup),**point))

def run():
    args = get_args()
    veto_set = {
        tuple(int(x) for x in pt.split('-')) for pt in args.veto_bad_points}

    with open(args.cls_files[0]) as yml:
        points = _flattify(yaml.load(yml), veto_set)

    for newpts in _file_iter(args.cls_files[1:], veto_set):
        for ptkey, newpt in newpts.items():
            if args.noover:
                _throw_if_overwrite(points.get(ptkey),newpt)
            points.setdefault(ptkey,{}).update(newpt)

    out_dict = {}
    for (cfg, msch, mlsp), pt in points.items():
        config = out_dict.setdefault(cfg, [])
        config.append(pt)

    sys.stdout.write(yaml.dump(out_dict))

if __name__ == '__main__':
    run()
