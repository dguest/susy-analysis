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
    print(yaml.dump(cls_dict))

# translation from marks naming convention
_danint_from_marks = {
    'm_scharm': 'scharm_mass',
    'm_LSP': 'lsp_mass',
    }
_danfloat_from_marks = {
    'CLs_exp': 'mean',
    }
def _dictify(line):
    """
    returns dict with the following keys:
     - scharm_mass
     - lsp_mass
     - upper (if exists)
     - lower (if exists)
     - mean
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
    with open(mark_file) as txt:
        config_name = None
        for line in txt:
            sline = line.strip()

            # region names start with ':'
            if sline.endswith(':'):
                config_name = sline.rstrip(':')
                continue

            config_list = cls_dict.setdefault(config_name,[])
            config_list.append(_dictify(sline))

if __name__ == '__main__':
    run()
