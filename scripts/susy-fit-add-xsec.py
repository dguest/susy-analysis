#!/usr/bin/env python3
"""add cross sections to a cls file (for use with plots)"""
_in_place_help = 'update cls file rather than dumping to stdout'

import argparse
import yaml
import re
import sys

from scharm import datasets

def get_args():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('cls_file')
    parser.add_argument('meta_file')
    parser.add_argument('-i', '--in-place', action='store_true',
                        help=_in_place_help)
    return parser.parse_args()

def run():
    args = get_args()
    with open(args.meta_file) as yml:
        meta = yaml.load(yml)
    xdic = xsec_from_meta(meta)

    inplc = args.in_place
    with open(args.cls_file) as yml:
        config_dict = yaml.load(yml)
    for points in config_dict.values():
        for pt in points:
            pt_mass = pt['scharm_mass'], pt['lsp_mass']
            if pt_mass in xdic:
                pt['xsec'] = xdic[pt_mass]

    out_text = yaml.dump(config_dict)

    if inplc:
        with open(args.cls_file, 'w') as yml:
            yml.write(out_text)
    else:
        sys.stdout.write(out_text)

def xsec_from_meta(meta):
    """return a dictionary of xsections keyed by (mscharm, mlsp)"""
    xdic = {}
    for ds in meta.values():
        if ds.get('physics_type') == 'signal':
            mkey = datasets.get_mass_pair(ds['full_name'])
            if mkey:
                xdic[mkey] = ds['total_xsec_fb']
    return xdic

if __name__ == '__main__':
    run()
