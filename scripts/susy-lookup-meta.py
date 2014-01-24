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


import os, sys, re
from scharm.meta import DatasetCache, MetaFromYamlError
from scharm.lookup.susylookup import MetaFactory
from scharm.lookup import overlap 
from scharm.lookup.ami import lookup_ami_stats, sort_ds_fullsim_atlfast
from scharm.bullshit import FlatProgressMeter
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
    d = 'default: %(default)s'
    parser = subs.add_parser('choose')
    parser.add_argument('steering_file')
    parser.add_argument('-c', '--cached')
    parser.add_argument(
        '-o', '--output-file', help='required if reading AMI')
    parser.add_argument(
        '--fullsim-bias', type=float, default=1.2, 
        help=d)

def _add_build_parser(subs): 
    d = 'default: %(default)s'
    parser = subs.add_parser('build')
    parser.add_argument(
        'steering_file', default='meta.yml', 
        help=('either a textfile of datasets'
              ' or an existing meta file ({})'.format(d)))
    parser.add_argument('--susy-lookup', help=d)
    parser.add_argument('-m','--mc', action='store_true', 
                        help='generate fresh mc file from Will\'s ds')
    parser.add_argument('--jets', action='store_true', 
                        help='generate fresh data list')
    parser.add_argument('--muons', action='store_true', 
                        help='generate fresh data list')
    parser.add_argument('--egamma', action='store_true', 
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
        '-s', '--write-steering', help='rewrite steering file')
    

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
    if args.cached: 
        with open(args.cached) as yml: 
            sets = yaml.load(yml)
    else: 
        if not args.output_file: 
            raise OSError('output file required when readin AMI')
        sets = lookup_ami_stats(args.steering_file)
    if args.output_file: 
        with open(args.output_file,'w') as yml: 
            yml.write(yaml.dump(sets))
        return 

    ds_cache = DatasetCache(args.steering_file)
    fullsim_keys, atlfast_keys = sort_ds_fullsim_atlfast(
        sets, get_keys=True, fullsim_bias=args.fullsim_bias)
    
    atl_set = set(atlfast_keys)
    ful_set = set(fullsim_keys)
    all_set = atl_set | ful_set
    len_name = max(len(ds_cache[d].name) for d in all_set)
    output_tmp = '{dsid} {name:{len}} {atl:>9} {full:>9} {choice} {exp}'
    for key in sorted(ds_cache): 
        if not key in all_set: 
            continue
        ds = ds_cache[key]
        stats = sets[key]
        is_atl = _is_atlfast(ds)
        should_be_atl = key in atl_set
        choice = 'atlfast' if is_atl else 'fullsim'
        exp = '!' if is_atl != should_be_atl else ''
        print output_tmp.format(
            dsid=ds.id, name=ds.name, len=len_name, 
            atl=stats.get('atlfast_stat',None), 
            full=stats.get('fullsim_stat',None), 
            choice=choice, exp=exp)
            

_atlfinder = re.compile('(_a([0-9])+)+')
def _is_atlfast(ds): 
    if _atlfinder.search(ds.full_name): 
        return True
    return False


def build(args): 
    if args.mc: 
        build_will_file(args.steering_file)
    if args.jets: 
        build_jets_file(args.steering_file)
    if args.muons: 
        build_muon_file(args.steering_file)
    if args.egamma: 
        build_egamma_file(args.steering_file)
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
     - Truth MET (assuming _<mass>_<mass>_MET<met> form for ds name)

    """
    overlap_tool = overlap.OverlapMetaAdder(sherpa_boson_pt_gev=40)
    with DatasetCache(name) as ds_cache: 
        overlap.clear_overlap_info(ds_cache)
        overlap_tool.add_overlap_info(ds_cache)
        

def update(name, overwrite=False): 
    from stop.lookup.ami import AmiAugmenter
    aug = AmiAugmenter('p1328', 'mc12_8TeV', backup_ptag='p1181')
    aug.bugstream = TemporaryFile()
    with DatasetCache(name) as ds_cache: 
        for key, ds in ds_cache.iteritems(): 
            if not ds.is_data: 
                required = [
                    ds.filteff, 
                    ds.total_xsec_fb, 
                    ]
                if not all(required) or overwrite:
                    sys.stdout.write('updating {}...'.format(ds.full_name))
                    success = aug.update_ds(ds, overwrite)
                    if success: 
                        sys.stdout.write('success\n')
                    else: 
                        sys.stdout.write('failed\n')
    dumpbugs(aug)

def dumpbugs(aug, bugslog='ami-bugs.log'): 
    if aug.bugstream.tell(): 
        aug.bugstream.seek(0)
        with open(bugslog,'w') as bugs: 
            for line in aug.bugstream: 
                bugs.write(line)
        sys.stderr.write('wrote bugs to {}\n'.format(bugslog))

def build_will_file(name): 
    from scharm.lookup.ami import AmiAugmenter
    from scharm.runtypes import wills_samples
    do_not_use = {'top_samples','Wjets_samples','Zjets_samples'}
    wills_types = [
        (k.split('_')[0], v) for k,v in wills_samples.items() 
        if k not in do_not_use]
    aug = AmiAugmenter('p1328', 'mc12_8TeV')
    aug.bugstream = TemporaryFile()
    ds_cache = DatasetCache(name)
    for phys_type, ids in wills_types: 
        new_meta = aug.get_dataset_range(ids, phys_type)
        ds_cache.update(new_meta)
    ds_cache.write()
    dumpbugs(aug, 'mc-bugs.log')

def build_stop_file(name): 
    from scharm.lookup.ami import AmiAugmenter
    from scharm.runtypes import stop_signal
    aug = AmiAugmenter('p1328', 'mc12_8TeV')
    aug.bugstream = TemporaryFile()
    ds_cache = DatasetCache(name)
    for phys_type, ids in stop_signal.iteritems(): 
        new_meta = aug.get_dataset_range(ids, phys_type)
        ds_cache.update(new_meta)
    ds_cache.write()
    dumpbugs(aug, 'stopgrid-bugs.log')

def build_scharm_file(name): 
    from scharm.lookup.ami import AmiAugmenter
    from scharm.runtypes import scharm
    aug = AmiAugmenter('p1512', 'mc12_8TeV')
    aug.bugstream = TemporaryFile()
    ds_cache = DatasetCache(name)
    for phys_type, ids in scharm.iteritems(): 
        new_meta = aug.get_dataset_range(ids, phys_type)
        ds_cache.update(new_meta)
    ds_cache.write()
    dumpbugs(aug, 'scharmgrid-bugs.log')

def build_variations_file(name): 
    from scharm.lookup.ami import AmiAugmenter
    from scharm.runtypes import variations
    aug = AmiAugmenter('p1512', 'mc12_8TeV')
    aug.bugstream = TemporaryFile()
    ds_cache = DatasetCache(name)
    for phys_type, ids in variations.iteritems(): 
        new_meta = aug.get_dataset_range(ids, phys_type)
        ds_cache.update(new_meta)
    ds_cache.write()
    dumpbugs(aug, 'variation-bugs.log')

def build_jets_file(name): 
    from scharm.lookup.ami import AmiAugmenter
    aug = AmiAugmenter('p1512', origin='data12_8TeV')
    aug.bugstream = TemporaryFile()
    with DatasetCache(name) as ds_cache: 
        new_meta = aug.get_datasets_year(stream='physics_JetTauEtmiss')
        ds_cache.update(new_meta)

    dumpbugs(aug, 'muon-bugs.log')

def build_muon_file(name): 
    from scharm.lookup.ami import AmiAugmenter
    aug = AmiAugmenter('p1512', origin='data12_8TeV')
    aug.bugstream = TemporaryFile()
    with DatasetCache(name) as ds_cache: 
        mu_meta = aug.get_datasets_year(stream='physics_Muons')
        ds_cache.update(mu_meta)

    dumpbugs(aug, 'muon-bugs.log')

def build_egamma_file(name): 
    from scharm.lookup.ami import AmiAugmenter
    aug = AmiAugmenter('p1512', origin='data12_8TeV')
    aug.bugstream = TemporaryFile()
    with DatasetCache(name) as ds_cache: 
        meta = aug.get_datasets_year(stream='physics_Egamma')
        ds_cache.update(meta)

    dumpbugs(aug, 'egamma-bugs.log')


if __name__ == '__main__': 
   run()
