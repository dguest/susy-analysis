#!/usr/bin/env python3.3

"""
Recurse through a directory tree, aggregate events in signal regions.
"""
_default_regions = ['signal_mct{}'.format(x) for x in [150, 200, 250]]
_regions_help = 'check in these regions, default: {}'.format(
    ', '.join(_default_regions))

_event_list_dir = 'event_list'
_run_from_file_re = '^\D([0-9]+).*\.h5'

import argparse

from os import walk
from os.path import basename, join
import h5py, re, sys
from collections import defaultdict

def get_args():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('root_dir')
    parser.add_argument(
        '-r', '--regions', nargs='+', default=_default_regions,
        help=_regions_help)
    return parser.parse_args()

def run():
    args = get_args()

    events = _get_events(args.root_dir, args.regions)
    regions = defaultdict(list)
    for event, run, region in events:
        regions[region].append( (run, event))
    for region, events in ((x,regions[x]) for x in sorted(regions)):
        sys.stdout.write('\n{} ({} events):\n'.format(region, len(events)))
        for run, event in sorted(events):
            sys.stdout.write('{:4d} {:4d}\n'.format(run, event))

def _get_events(base, regions):
    events = set()
    for root, dirs, files in walk(base):
        dirpath = root.split('/')
        tld = dirpath[-1] or dirpath[-2]
        if files and tld != _event_list_dir:
            err = "found files in {}, expected them only in {}".format(
                tld, _event_list_dir)
            raise ValueError(err)
        for h5file in (join(root,x) for x in files):
            file_counts = {ern for ern in _file_iter(h5file, regions)}
            events |= file_counts
    return events

def _file_iter(filen, regions):
    file_re = re.compile(_run_from_file_re)
    run_number = int(file_re.search(basename(filen)).group(1))
    with h5py.File(filen, 'r') as hfile:
        for regname in regions:
            region = hfile[regname]
            for event, run in zip(region['event'], region['run']):
                yield event, run_number if run < 0 else run, regname

if __name__ == '__main__':
    run()
