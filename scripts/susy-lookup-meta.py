#!/usr/bin/env python2.7
"""
Script to consolidate all the needed meta info in one place. 

Should be usable in a few ways: 
 - to generate a meta file that is read in by the histogramers. The cutflow 
   routine is responsible for mapping out the datasets and running over 
   them. 
 - to update the meta info before feeding the meta info to stacking routines.

Badly needs a cleanup, should only have functions to: 
 - get data meta from ami
 - get mc meta from ami
 - get mc meta from susytools
"""


import os, sys
from stop.meta import DatasetCache
from stop.lookup.susylookup import MetaFactory
import argparse, ConfigParser
from tempfile import TemporaryFile

def _get_parser(): 
    parser = argparse.ArgumentParser(
        formatter_class=argparse.RawDescriptionHelpFormatter, 
        description=__doc__, 
        epilog='Author: Dan Guest', 
        )

    def_meta = 'meta.yml'
    defaults = { 
        'output_pickle': None, 
        'steering_file': def_meta
        }
    
    d = 'default: %(default)s'

    parser.set_defaults(**defaults)
    parser.add_argument(
        'steering_file', 
        help=('either a textfile of datasets'
              ' or an existing meta file ({})'.format(d)))
    parser.add_argument('--susy-lookup', help=d)
    parser.add_argument('--output-meta', help=d)
    parser.add_argument('-f', action='store_true',
                        help='force overwrite of meta (if it exists)')

    ami_mode = parser.add_mutually_exclusive_group()
    ami_mode.add_argument('--update-ami', action='store_true')
    ami_mode.add_argument('--rewrite-ami', action='store_true')

    parser.add_argument('-m','--marks-mc', action='store_true', 
                        help='generate fresh mc file from mark\'s ds')
    parser.add_argument('--data', action='store_true', 
                        help='generate fresh data list')
    parser.add_argument('-t', '--trust-ds-names', action='store_true')
    parser.add_argument('-d','--dump', action='store_true')
    parser.add_argument(
        '--write-steering', help='rewrite steering file')

    return parser

def get_ds_lists(cache): 
    if isinstance(cache, str): 
        cache = DatasetCache(cache)
    names = []
    unnames = []
    for dskey, ds in cache.iteritems(): 
        if ds.full_name: 
            names.append(ds.full_name)
        else: 
            unnames.append(dskey)
    return names, unnames

def run(): 
    args = _get_parser().parse_args(sys.argv[1:])

    if not args.output_meta: 
        if args.steering_file.endswith('.yml'): 
            args.output_meta = args.steering_file
        else: 
            args.output_meta = 'meta.yml'
    if os.path.isfile(args.output_meta) and not args.f: 
        if not args.write_steering or args.a: 
            sys.exit(
                'output {} exists already, refusing to overwrite'.format(
                    args.output_meta))


    if args.marks_mc: 
        build_mark_file(args.steering_file)
    if args.data: 
        build_data_file(args.steering_file)

    if args.update_ami or args.rewrite_ami: 
        update(args.steering_file, overwrite=args.rewrite_ami)

    if args.susy_lookup: 
        mf = MetaFactory(args.steering_file)
        mf.verbose = True
        with open(args.susy_lookup) as susy:
            mf.lookup_susy(susy)
        mf.write_meta(args.output_meta)

    if args.write_steering: 
        found, missing = get_ds_lists(args.steering_file)
        if missing: 
            print 'missing dataset names:'
            for m in missing: 
                print m
        with open(args.write_steering, 'w') as st: 
            for ds in found: 
                st.write(ds.strip('/') + '/\n')
        return 

def update(name, overwrite=False): 
    from stop.lookup.ami import AmiAugmenter
    ami = AmiAugmenter('p1328', 'mc12_8TeV', backup_ptag='p1181')
    ami.bugstream = TemporaryFile()
    with DatasetCache(name) as ds_cache: 
        for key, ds in ds_cache.iteritems(): 
            if not ds.is_data: 
                required = [
                    ds.filteff, 
                    ds.total_xsec_fb, 
                    ]
                if not all(required) or overwrite:
                    sys.stdout.write('updating {}...'.format(ds.full_name))
                    success = ami.update_ds(ds, overwrite)
                    if success: 
                        sys.stdout.write('success\n')
                    else: 
                        sys.stdout.write('failed\n')
    dumpbugs(ami)

def dumpbugs(ami, bugslog='ami-bugs.log'): 
    if ami.bugstream.tell(): 
        ami.bugstream.seek(0)
        with open(bugslog,'w') as bugs: 
            for line in ami.bugstream: 
                bugs.write(line)
        sys.stderr.write('wrote bugs to {}\n'.format(bugslog))

def build_mark_file(name): 
    from stop.lookup.ami import AmiAugmenter
    from stop.runtypes import marks_types
    ami = AmiAugmenter('p1328', 'mc12_8TeV')
    ami.bugstream = TemporaryFile()
    ds_cache = DatasetCache(name)
    for phys_type, ids in marks_types.iteritems(): 
        new_meta = ami.get_dataset_range(ids, phys_type)
        ds_cache.update(new_meta)
    ds_cache.write()
    dumpbugs(ami)

def build_data_file(name): 
    from stop.lookup.ami import AmiAugmenter
    ami = AmiAugmenter('p1329', origin='data12_8TeV')
    ami.bugstream = TemporaryFile()
    with DatasetCache(name) as ds_cache: 
        mu_meta = ami.get_datasets_year(stream='physics_Muons')
        ds_cache.update(mu_meta)
        new_meta = ami.get_datasets_year(stream='physics_JetTauEtmiss')
        ds_cache.update(new_meta)

    if ami.bugstream.tell(): 
        ami.bugstream.seek(0)
        bugslog = 'ami-bugs.log'
        with open(bugslog,'w') as bugs: 
            for line in ami.bugstream: 
                bugs.write(line)
        sys.stderr.write('wrote bugs to {}\n'.format(bugslog))

if __name__ == '__main__': 
   run()
