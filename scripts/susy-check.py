#!/usr/bin/env python2.7
"""
Top level script for vairous cross checks. 
"""

_meta_help="""
Check meta file for missing events
"""

_stext_help="""
Check susy events textfile. 
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
        'stext': check_susy_events_file
        }
    subs[config.which](config)


def get_config(): 
    d = 'default: %(default)s'
    c = "with no argument is '%(const)s'"

    parser = argparse.ArgumentParser(description=__doc__)
    subs = parser.add_subparsers(dest='which')

    # check meta
    metacheck = subs.add_parser('meta', description=_meta_help)
    metacheck.add_argument('meta_file')

    # check susy-events file
    susy_text_check = subs.add_parser('stext', description=_stext_help)
    susy_text_check.add_argument('text_file')
    susy_text_check.add_argument('meta_file', nargs='?')

    return parser.parse_args(sys.argv[1:])

def check_susy_events_file(config): 
    """
    For summing over the susy group provided textfile on event counts. 
    """
    datasets = set()
    ds_counts = {}
    repeats = 0
    with open(config.text_file) as susy_file: 
        running_total = 0
        for line in susy_file: 
            sline = [e.strip() for e in line.split('|')]
            if len(sline) >= 5: 
                ds_name = sline[4].split()[1]
                if ds_name.startswith('data12'): 
                    dsid = int(ds_name.split('.')[1])
                    total_events = int(sline[3].split()[1])
                    if dsid in datasets: 
                        repeats += 1
                        if total_events != ds_counts[dsid]: 
                            prob = 'ACHTUNG: {} != {} in {}'.format(
                                ds_counts[dsid], total_events, dsid)
                            sys.stderr.write(prob + '\n')
                                    
                    else: 
                        datasets.add(dsid)
                        running_total += total_events
                    ds_counts[dsid] = total_events

    print '{:,} total events in {} datasets ({} repeats)'.format(
        running_total, len(datasets), repeats)
    if config.meta_file: 
        meta_dsids = set()
        data_expected = 0
        data_skimmed = 0
        cache = DatasetCache(config.meta_file)
        for key, ds in cache.iteritems(): 
            keychar = 'd'
            if key.startswith(keychar): 
                meta_dsids.add(int(key.lstrip(keychar)))
                data_expected += ds.n_expected_entries
                data_skimmed += ds.n_raw_entries
        not_in_susy = meta_dsids - datasets
        def listinate(dsids): 
            return ', '.join(str(i) for i in dsids)
        if not_in_susy: 
            print '{} found in AMI and not susy'.format(
                listinate(not_in_susy))

        not_in_meta = datasets - meta_dsids
        if not_in_meta: 
            print '{} found in susy and not AMI'.format(
                listinate(not_in_meta))

        skim_ami_frac = float(data_skimmed) / float(data_expected)
        skim_susy_frac = float(data_skimmed) / float(running_total)
        print 'expected from AMI: {:,}'.format(data_expected)
        print 'skimmed: {:,} ({:.1%} AMI, {:.1%} SUSY)'.format(
            data_skimmed, skim_ami_frac, skim_susy_frac)

def _is_total_line(sline):
    if not sline[0].startswith('='): 
        return False
    numbs = sline[0].lstrip('=').split('/')
    if not len(numbs) == 2: 
        return False
    if numbs[0] == numbs[1]: 
        return True
    raise ValueError('just fucking shit')
    

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
