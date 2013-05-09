#!/usr/bin/env python2.7
"""
Top level script for vairous generic data manipulation. 
"""

_tag_list_help="""
Routines to print lists of files / datasets referenced in a meta file. 
"""

_rename_help="""
Swaps the ds key file names for something more readable. 
"""

import argparse, sys
from os.path import isdir, isfile, join, expanduser, splitext
from os.path import dirname, basename, abspath
import os
import re, glob
import yaml
import warnings
import h5py
from stop.hists import HistNd, HistAdder
from stop import meta

def run(): 
    config = get_config()
    subs = {
        'list':list_meta_info, 
        'hadd':hadd, 'rename':rename}
    subs[config.which](config)


def get_config(): 
    d = 'default: %(default)s'
    c = "with no argument is '%(const)s'"

    parser = argparse.ArgumentParser(description=__doc__)
    subs = parser.add_subparsers(dest='which')

    meta_list_shared = argparse.ArgumentParser(add_help=False)
    list_type = meta_list_shared.add_mutually_exclusive_group()
    list_type.add_argument('--physics', help='filter meta by physics')
    meta_list_shared.add_argument('meta_file')

    meta_list = subs.add_parser('list', description=_tag_list_help)
    list_subs = meta_list.add_subparsers(dest='outlist')
    list_files = list_subs.add_parser('files', parents=[meta_list_shared])
    list_files.add_argument('ntuples_dir')
    
    list_meta = list_subs.add_parser('meta', parents=[meta_list_shared])
    list_names = list_subs.add_parser('names', parents=[meta_list_shared])

    meta_rename = subs.add_parser('rename', description=_rename_help)
    meta_rename.add_argument('meta_file')
    meta_rename.add_argument('files', nargs='+')
    meta_rename.add_argument('-d', '--dummy', action='store_true')

    hist_add = subs.add_parser('hadd') 
    hist_add.add_argument('input_hists', nargs='+')
    hist_add.add_argument('-o', '--output', 
                          help='hist or (for dash-hadd) directory')
    hist_add.add_argument('-d', '--dash-hadd', action='store_true', 
                          help='multiple hadds, splits input hists at \'-\'')

    return parser.parse_args(sys.argv[1:])


def list_meta_info(config): 
    from stop.meta import DatasetCache
    meta = DatasetCache(config.meta_file)
    filt_meta = {}
    if config.physics: 
        for ds_key, ds in meta.iteritems(): 
            if ds.physics_type == config.physics: 
                filt_meta[ds_key] = ds
    else: 
        filt_meta = meta

    subopts = {'files': list_files, 'meta': list_meta, 'names':list_names}
    subopts[config.outlist](config, filt_meta)

def list_files(config, filt_meta): 
    if config.ntuples_dir: 
        ntuples = glob.glob('{}/*.root*'.format(config.ntuples_dir))
        for ntuple in ntuples: 
            for ds in filt_meta.values(): 
                if str(ds.id) in ntuple: 
                    print ntuple

def list_names(config, filt_meta): 
    for ds in filt_meta.values(): 
        print '{}/'.format(ds.full_name)
def list_meta(config, filt_meta): 
    meta = {}
    for ds in filt_meta.values(): 
        meta[ds.key] = ds.yml_dict()
    print yaml.dump(meta)


def hadd(config): 
    good_files = []
    for hist_file in config.input_hists: 
        with h5py.File(hist_file) as h5: 
            if len(h5.keys()): 
                good_files.append(hist_file)
    if len(good_files) != len(config.input_hists): 
        sys.stderr.write(
            'ACHTUNG: only {} of {} files have any hists\n'.format(
                len(good_files), len(config.input_hists)))
    if config.dash_hadd: 
        def key_from_name(fname): 
            return splitext(basename(fname))[0].split('-')[0]
        if not isdir(config.output): 
            os.mkdir(config.output)
        base_keys = {key_from_name(f) for f in good_files}
        for key in base_keys: 
            out_path = join(config.output, '{}.h5'.format(key))
            print 'making {}'.format(out_path)
            file_group = [f for f in good_files if key in f]
            _hadd(file_group, out_path)
    else: 
        _hadd(good_files, config.output)

def _hadd(good_files, output):
    with h5py.File(good_files[0]) as base_h5: 
        hadder = HistAdder(base_h5)
    for add_file in good_files[1:]: 
        if not isfile(add_file): 
            raise IOError("{} doesn't exist".format(add_file))
        with h5py.File(add_file) as add_h5: 
            hadder.add(add_h5)
    if output: 
        with h5py.File(output,'w') as out_file: 
            hadder.write_to(out_file)
    else:
        hadder.dump()

def rename(config): 
    junk_finder = re.compile('_([A-Z0-9]+_)')
    meta_lookup = meta.DatasetCache(config.meta_file)
    rename_map = {}
    used_out = set()
    for old_name in config.files: 
        old_base, old_ext = splitext(basename(old_name))
        if old_base in meta_lookup: 
            new_name = meta_lookup[old_base].name
            try: 
                junk = junk_finder.search(new_name).group(1)
                new_name = new_name.replace(junk, '')
            except AttributeError: 
                pass
            new_path = join(dirname(old_name), new_name + old_ext)
            if new_path in used_out: 
                raise ValueError('rename would overwrite {}'.format(new_path))
            used_out.add(new_path)
            rename_map[old_name] = new_path
    for old_name, new_name in rename_map.iteritems(): 
        if config.dummy: 
            print '{} --> {}'.format(old_name, new_name)
        else: 
            os.rename(old_name, new_name)
            
if __name__ == '__main__': 
    run()