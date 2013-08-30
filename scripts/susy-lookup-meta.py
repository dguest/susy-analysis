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
from stop.lookup import overlap 
from stop.bullshit import FlatProgressMeter
import argparse, ConfigParser
from tempfile import TemporaryFile
import yaml

def _get_parser(): 
    parser = argparse.ArgumentParser(
        formatter_class=argparse.RawDescriptionHelpFormatter, 
        description=__doc__, 
        epilog='Author: Dan Guest', 
        )
    subs = parser.add_subparsers(dest='which')
    _add_build_parser(subs)
    _add_choice_parser(subs)
    return parser

def _add_choice_parser(subs): 
    parser = subs.add_parser('choose')
    parser.add_argument('steering_file')

def _add_build_parser(subs): 
    d = 'default: %(default)s'
    parser = subs.add_parser('build')
    parser.add_argument(
        'steering_file', default='meta.yml', 
        help=('either a textfile of datasets'
              ' or an existing meta file ({})'.format(d)))
    parser.add_argument('--susy-lookup', help=d)
    parser.add_argument('-m','--mc', action='store_true', 
                            help='generate fresh mc file from mark\'s ds')
    parser.add_argument('--jets', action='store_true', 
                        help='generate fresh data list')
    parser.add_argument('--muons', action='store_true', 
                        help='generate fresh data list')
    parser.add_argument('--variations', action='store_true')
    parser.add_argument('--stop', action='store_true')
    parser.add_argument('--scharm', action='store_true')
    parser.add_argument('-l', '--add-overlap', action='store_true', 
                        help=find_overlap.__doc__)

    ami_mode = parser.add_mutually_exclusive_group()
    ami_mode.add_argument('--update-ami', action='store_true')
    ami_mode.add_argument('--rewrite-ami', action='store_true')

    parser.add_argument('-d','--dump', action='store_true')
    parser.add_argument(
        '--write-steering', help='rewrite steering file')
    

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
    {'build':build, 'choose':choose}[args.which](args)

def choose(args): 
    from stop.lookup.ami import McStatsLookup
    ami = McStatsLookup('p1328', 'mc12_8TeV')
    ds_cache = DatasetCache(args.steering_file)
    sets = []
    def filt(item): 
        key, ds = item
        if ds.is_data or ds.physics_type == 'signal': 
            return False
        return True
    ds_cache = filter(filt, ds_cache.iteritems())
    prog_meter = FlatProgressMeter(len(ds_cache))
    for ds_n, (ds_key, ds) in enumerate(ds_cache): 
        prog_meter.update(ds_n)
        atlfast, fullsim = ami.get_atlfast_fullsim(ds.id)
        vals = {}
        if atlfast: 
            vals['atlfast_name'] = atlfast[0]
            vals['atlfast_stat'] = atlfast[1]
        if fullsim: 
            vals['fullsim_name'] = fullsim[0]
            vals['fullsim_stat'] = fullsim[1]
        sets.append(vals)

    print yaml.dump(sets)
    

def build(args): 
    if args.mc: 
        build_mark_file(args.steering_file)
    if args.jets: 
        build_jets_file(args.steering_file)
    if args.muons: 
        build_muon_file(args.steering_file)
    if args.variations: 
        build_variations_file(args.steering_file)
    if args.scharm: 
        build_scharm_file(args.steering_file)
    if args.stop: 
        build_stop_file(args.steering_file)

    if args.update_ami or args.rewrite_ami: 
        update(args.steering_file, overwrite=args.rewrite_ami)

    if args.susy_lookup: 
        mf = MetaFactory(args.steering_file)
        mf.verbose = True
        with open(args.susy_lookup) as susy:
            mf.lookup_susy(susy)
        mf.write_meta(args.steering_file)

    if args.add_overlap: 
        find_overlap(args.steering_file)
    
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

def find_overlap(name): 
    """
    Adds a field to datasets where overlap removal is needed. 

    Currently does the following: 
     - Truth boson Pt for Sherpa
     - Truth MET for stop signals

    """
    overlap_tool = overlap.OverlapMetaAdder(sherpa_boson_pt_gev=40)
    with DatasetCache(name) as ds_cache: 
        overlap.clear_overlap_info(ds_cache)
        overlap_tool.add_overlap_info(ds_cache)
        

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
    dumpbugs(ami, 'mc-bugs.log')

def build_stop_file(name): 
    from stop.lookup.ami import AmiAugmenter
    from stop.runtypes import stop_signal
    ami = AmiAugmenter('p1328', 'mc12_8TeV')
    ami.bugstream = TemporaryFile()
    ds_cache = DatasetCache(name)
    for phys_type, ids in stop_signal.iteritems(): 
        new_meta = ami.get_dataset_range(ids, phys_type)
        ds_cache.update(new_meta)
    ds_cache.write()
    dumpbugs(ami, 'stopgrid-bugs.log')

def build_scharm_file(name): 
    from stop.lookup.ami import AmiAugmenter
    from stop.runtypes import scharm
    ami = AmiAugmenter('p1328', 'mc12_8TeV')
    ami.bugstream = TemporaryFile()
    ds_cache = DatasetCache(name)
    for phys_type, ids in scharm.iteritems(): 
        new_meta = ami.get_dataset_range(ids, phys_type)
        ds_cache.update(new_meta)
    ds_cache.write()
    dumpbugs(ami, 'scharmgrid-bugs.log')

def build_variations_file(name): 
    from stop.lookup.ami import AmiAugmenter
    from stop.runtypes import variations
    ami = AmiAugmenter('p1328', 'mc12_8TeV')
    ami.bugstream = TemporaryFile()
    ds_cache = DatasetCache(name)
    for phys_type, ids in variations.iteritems(): 
        new_meta = ami.get_dataset_range(ids, phys_type)
        ds_cache.update(new_meta)
    ds_cache.write()
    dumpbugs(ami, 'variation-bugs.log')

def build_jets_file(name): 
    from stop.lookup.ami import AmiAugmenter
    ami = AmiAugmenter('p1329', origin='data12_8TeV')
    ami.bugstream = TemporaryFile()
    with DatasetCache(name) as ds_cache: 
        new_meta = ami.get_datasets_year(stream='physics_JetTauEtmiss')
        ds_cache.update(new_meta)

    dumpbugs(ami, 'muon-bugs.log')

def build_muon_file(name): 
    from stop.lookup.ami import AmiAugmenter
    ami = AmiAugmenter('p1329', origin='data12_8TeV')
    ami.bugstream = TemporaryFile()
    with DatasetCache(name) as ds_cache: 
        mu_meta = ami.get_datasets_year(stream='physics_Muons')
        ds_cache.update(mu_meta)

    dumpbugs(ami, 'muon-bugs.log')

if __name__ == '__main__': 
   run()
