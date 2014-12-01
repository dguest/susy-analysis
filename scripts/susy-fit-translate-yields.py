#!/usr/bin/env python3
"""
Translate yields to json, strip off useless stuff.
"""

import argparse, yaml, json
import sys

def _get_args():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('yields_file')
    return parser.parse_args()

def run():
    args = _get_args()
    with open(args.yields_file, 'r') as yml:
        dic = yaml.load(yml)
    sys.stdout.write(json.dumps(dic['nominal_yields'], indent=2))

if __name__ == '__main__':
    run()
