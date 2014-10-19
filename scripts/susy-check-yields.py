#!/usr/bin/env python3

"""
Check / dump the yields in some h5 histograms
"""
_h5_files_help = "can take either a list of files or a directory"

import argparse, sys
from os.path import isdir
from os import walk
from scharm.aggregate.normalizer import Normalizer
from scharm.aggregate import histread

def run():
    # _dump_counts({(43, 'smag'): dict(w=20, n=8)})
    args = get_args()
    h5_files = []
    h5_files += _get_files(args.h5_files)
    quiet = not args.verbose

    counts = {}

    normalizer = Normalizer(args.meta, h5_files, quiet=quiet)
    for dsid, hfile, norm in normalizer.byid():
        for region in args.regions:
            weighted_counts = histread.get_count(hfile[region]['met'])
            raw_counts = histread.get_count(hfile[region]['event_weight'])
            counts[dsid,region] = {
                'w': weighted_counts, 'n': weighted_counts * norm,
                'r': raw_counts}

    _dump_counts(counts)

def get_args():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('h5_files', nargs='+')
    parser.add_argument('-m', '--meta', required=True)
    parser.add_argument('-r', '--regions', nargs='+', default=['cr_t'])
    parser.add_argument('-v', '--verbose', action='store_true')

    return parser.parse_args(sys.argv[1:])


def _dump_counts(counts):
    """counts should be a dictionary of counts, keyed by (dsid, region)"""
    ids = set()
    regions = set()
    for dsid, region in counts:
        ids.add(dsid)
        regions.add(region)

    sreg = sorted(regions)
    fmt_tmp = '{:>10} | ' + '{XXX} {XXX} {XXX} | '*len(sreg)
    fmt_string = fmt_tmp.replace('XXX', ':12.2f')
    reg_args = [''] + sreg
    print(fmt_tmp.replace('XXX} {XXX} {XXX', ':^38').format(*reg_args))
    title_args = ['dsid'] + ['normed', 'weighted', 'raw']*len(sreg)
    print(fmt_tmp.replace('XXX', ':>12').format(*title_args))
    for dsid in sorted(ids):
        fmt_list = [dsid]
        for region in sreg:
            fmt_list += [counts[dsid,region][x] for x in 'nwr']
        print(fmt_string.format(*fmt_list))

def _get_files(fandd):
    """convert a list of files and dirs to a list of files"""
    files = []
    for entry in fandd:
        if isdir(entry):
            for root, dirs, files in walk(entry):
                for hf in files:
                    if hf.endswith('.h5'):
                        files.append(hf)
        else:
            files.append(entry)
    return files

if __name__ == '__main__':
    run()
