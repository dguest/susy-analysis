#!/usr/bin/env python3
"""print cls values in some other format to a limits file"""

_overwrite_help = (
    "overwrite all existing values in old cls (by default only overwrite "
    "the observed values)")

import argparse, sys
import yaml

def get_config():
    d = 'default: %(default)s'
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('jan_files', nargs='*')
    parser.add_argument('-c', '--old-cls')
    parser.add_argument('-f', '--overwrite', help=_overwrite_help,
                        action='store_true')
    return parser.parse_args()

def run():
    args = get_config()
    cls_dict = _get_unflat_dict(args.old_cls)
    save_keys = None if args.overwrite else {'exp', 'exp_u1s', 'exp_d1s'}
    for mfile in args.jan_files:
        _add_jan_file(cls_dict, mfile, save_keys)
    print(yaml.dump(_flatten_cls_dict(cls_dict)))

def _get_unflat_dict(file_name):
    if not file_name:
        return {}
    unflat = {}
    with open(file_name) as yml:
        for region, points in yaml.load(yml).items():
            unflat_region = unflat.setdefault(region,{})
            for point in points:
                pt_key = tuple(point[x] for x in ['scharm_mass', 'lsp_mass'])
                unflat_region[pt_key] = point
    return unflat


# translation from marks naming convention
_danint_from_marks = {
    'm_scharm': 'scharm_mass',
    'm_LSP': 'lsp_mass',
    'm_stop': 'scharm_mass',
    }
_danfloat_from_jan = {
    'cls_exp': 'exp',
    'cls_obs': 'obs',
    'expUp': 'exp_u1s',
    'expDown': 'exp_d1s',
    'obsUp': 'obs_u1s',
    'obsDown': 'obs_d1s',
    'theoUp': 'obs_u1s',
    'theoDown': 'obs_d1s',
    'limit_obs': 'ul',
    'limit_exp': None,
    'limit_up': None,
    'limit_down': None,
    }
_config_name_vs_file = {
    ('250', 'C1'),
    ('350', 'C2'),
    }
def _get_config_name(file_name):
    for jan, dan in _config_name_vs_file:
        if jan in file_name:
            return dan

def _dictify(line, headers):
    """
    Returns two values: (ms, ml) and a dict of floats.
    """
    keyfield, *valfields = line.split()
    mass_keys = tuple(int(x) for x in keyfield.split('-')[1:])
    out = {'scharm_mass': mass_keys[0], 'lsp_mass': mass_keys[1]}
    for field_n, field in enumerate(valfields):
        key = _danfloat_from_jan[headers[field_n]]
        if key is None:
            continue
        out[key] = float(field)
    return mass_keys, out

def _add_jan_file(cls_dict, jan_file, save_keys=None):
    """
    Adds a {region: {mass_point: params, ... }, ...} dict to cls_dict
    If update_keys is a set update those keys, if None, only update missing
    """
    with open(jan_file) as txt:
        lines = iter(txt)
        headers = next(lines).split()[1:]
        config_name = _get_config_name(jan_file)
        config_dict = cls_dict.setdefault(config_name,{})
        for line in lines:
            mass_keys, pt = _dictify(line, headers)
            # if mass_keys not in config_dict:
            #     continue
            # slightly strange logic is required here to
            # update only the values that haven't been defined
            point_dict = config_dict.setdefault(mass_keys,{})
            if save_keys is None:
                uk = set(pt)
            elif save_keys is True:
                uk = set(pt) - set(point_dict)
            else:
                uk = set(pt) - (set(save_keys) & set(point_dict))
            for key, val in pt.items():
                if key in uk:
                    point_dict[key] = val

def _flatten_cls_dict(cls_dict):
    """flattens cls_dict to return {region: [ params, ... ], ...} dict"""
    flat_dict = {}
    for region, pt_dict in cls_dict.items():
        for params in pt_dict.values():
            flat_dict.setdefault(region,[]).append(params)
    return flat_dict

if __name__ == '__main__':
    run()
