#!/usr/bin/env python3
"""
Rename configs in cls files.
"""
import argparse
import yaml
import sys

from scharm import datasets

def get_args():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('cls_file')
    parser.add_argument('extension')
    return parser.parse_args()

def run():
    args = get_args()
    with open(args.cls_file) as yml:
        cls_dict = yaml.load(yml)
    renamed = {x + args.extension: y for x, y in cls_dict.items()}
    sys.stdout.write(yaml.dump(renamed))

if __name__ == '__main__':
    run()
