#!/usr/bin/env python2.7
"""
Top level script for vairous cross checks. 
"""

_meta_help="""
Check meta file for missing events
"""


import argparse, sys
from os.path import isdir, isfile, join, expanduser, splitext
from os.path import dirname, basename, abspath
import os
import re
import yaml
import warnings
from stop.meta import DatasetCache
from collections import Counter

def run(): 
    config = get_config()
    subs = {
        'meta': check_meta, 
        }
    subs[config.which](config)


def get_config(): 
    d = 'default: %(default)s'
    c = "with no argument is '%(const)s'"

    parser = argparse.ArgumentParser(description=__doc__)
    subs = parser.add_subparsers(dest='which')

    # check meta
    check = subs.add_parser('meta', description=_meta_help)
    check.add_argument('meta_file')

    return parser.parse_args(sys.argv[1:])

def check_meta(config): 
    type_expected_counter = Counter()
    type_found_counter = Counter()
    
    ds_meta = DatasetCache(config.meta_file)
    for key, ds in ds_meta.iteritems(): 
        expected = ds.n_expected_entries
        found = ds.n_raw_entries
        if expected != found: 
            temp_str = 'sample {} ({}): expected {:,}, found {:,}\n'
            sys.stderr.write(temp_str.format(
                    ds.key, ds.physics_type, expected, found))
        type_found_counter[ds.physics_type] += found
        type_expected_counter[ds.physics_type] += expected

    nameslen = max(len(n) for n in type_expected_counter) + 1
    explen = max(len(str(n)) for n in type_expected_counter.values()) + 1
    for phys_type in type_expected_counter: 
        expected = type_expected_counter[phys_type]
        found = type_found_counter[phys_type]
        print '{:{}}: {:{te}} of {:{te}} ({:.2%})'.format(
            phys_type, nameslen, found, expected, float(found)/expected, 
            te=explen)

if __name__ == '__main__': 
    run()
