#!/usr/bin/env python3.3
"""print cls values in some other format to a limits file"""

import argparse, sys
import yaml

def get_config():
    d = 'default: %(default)s'
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('mark_files', nargs='+')
    return parser.parse_args()

def run():
    args = get_config()
    cls_dict = {}
    for mfile in args.mark_files:
        _add_mark_file(cls_dict, mfile)
    print(yaml.dump(_flatten_cls_dict(cls_dict)))

# translation from marks naming convention
_danint_from_marks = {
    'm_scharm': 'scharm_mass',
    'm_LSP': 'lsp_mass',
    'm_stop': 'scharm_mass',
    }
_danfloat_from_marks = {
    'CLs_exp': 'exp',
    'CLs_obs': 'obs',
    'CLs_exp1sup': 'exp_u1s',
    'CLs_exp_1sdown': 'exp_d1s',
    }
def _dictify(line):
    """
    returns dict with the following keys:
     - scharm_mass
     - lsp_mass
     - upper (if exists)
     - lower (if exists)
     - cls_exp
     """
    out = {}
    for assignment in line.split(','):
        rawkey, rawval = assignment.split('=')
        key = rawkey.strip()
        if key in _danint_from_marks:
            out[_danint_from_marks[key]] = int(
                float(rawval.replace('GeV','').strip()))
        elif key in _danfloat_from_marks:
            out[_danfloat_from_marks[key]] = float(rawval)
    return out

def _add_mark_file(cls_dict, mark_file):
    """adds a {region: {mass_point: params, ... }, ...} dict to cls_dict"""
    with open(mark_file) as txt:
        config_name = None
        for line in txt:
            sline = line.strip()
            if not sline:
                continue

            # region names start with ':'
            if sline.endswith(':'):
                config_name = sline.rstrip(':')
                continue

            config_dict = cls_dict.setdefault(config_name,{})
            pt = _dictify(sline)
            pt_key = pt['scharm_mass'],pt['lsp_mass']
            config_dict.setdefault(pt_key,{}).update(pt)

def _flatten_cls_dict(cls_dict):
    """flattens cls_dict to return {region: [ params, ... ], ...} dict"""
    flat_dict = {}
    for region, pt_dict in cls_dict.items():
        for params in pt_dict.values():
            flat_dict.setdefault(region,[]).append(params)
    return flat_dict

if __name__ == '__main__':
    run()
