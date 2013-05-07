#!/usr/bin/env python2.7
"""
Top level script for vairous generic data manipulation. 
"""

_tag_list_help="""
Prints lists of files / datasets in a meta file. 
"""

import argparse, sys
from os.path import isdir, isfile, join, expanduser, splitext
from os.path import dirname, basename, abspath
import os
import re
import yaml
import warnings
import h5py
from h5py import Group, Dataset
from stop.hists import HistNd


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

    meta_list = subs.add_parser('list', description=_tag_list_help)
    meta_list.add_argument('meta_file')
    list_type = meta_list.add_mutually_exclusive_group(required=True)
    list_type.add_argument('--physics')
    meta_list.add_argument('-d', '--ntuples-dir')

    meta_rename = subs.add_parser('rename')
    meta_rename.add_argument('meta_file')
    meta_rename.add_argument('files', nargs='+')
    meta_rename.add_argument('-d', '--dummy', action='store_true')

    hist_add = subs.add_parser('hadd') 
    hist_add.add_argument('input_hists', nargs='+')
    hist_add.add_argument('-o', '--output-hist')

    return parser.parse_args(sys.argv[1:])


def list_meta_info(config): 
    from stop.meta import DatasetCache
    meta = DatasetCache(config.meta_file)
    filt_meta = {}
    if config.physics: 
        for ds_key, ds in meta.iteritems(): 
            if ds.physics_type == config.physics: 
                filt_meta[ds_key] = ds

    if config.ntuples_dir: 
        ntuples = glob.glob('{}/*.root*'.format(config.ntuples_dir))
        for ntuple in ntuples: 
            for ds in filt_meta.values(): 
                if str(ds.id) in ntuple: 
                    print ntuple
    else: 
        for ds in filt_meta.values(): 
            print ds.full_name

class HistAdder(object): 
    def __init__(self, base_group): 
        self.hists = self._search(base_group)
        
    def _search(self, group): 
        subhists = {}
        for key, subgroup in group.iteritems(): 
            if isinstance(subgroup, Group): 
                subhists[key] = self._search(subgroup)
            elif isinstance(subgroup, Dataset): 
                subhists[key] = HistNd(subgroup)
            else: 
                raise ValueError('not sure what to do with {} {}'.format(
                        type(subgroup), key))
        return subhists
    def _merge(self, hist_dict, new_hists): 
        merged = {}
        for key, subgroup in hist_dict.iteritems(): 
            if not key in new_hists: 
                raise ValueError("node {} not found in new hists".format(key))
            if isinstance(subgroup, dict): 
                merged[key] = self._merge(subgroup, new_hists[key])
            elif isinstance(subgroup, HistNd): 
                if not isinstance(new_hists[key], Dataset): 
                    raise ValueError("tried to merge non-dataset {}".format(
                            key))
                new_hist = HistNd(new_hists[key])
                merged[key] = subgroup + new_hist
            else: 
                raise ValueError('not sure what to do with {}, {}'.format(
                        type(subgroup), key))
        return merged

    def _write(self, hists, group): 
        for key, hist in hists.iteritems(): 
            if isinstance(hist, dict): 
                subgrp = group.create_group(key)
                self._write(hist, subgrp)
            else: 
                hist.write_to(group, key)

    def add(self, group): 
        self.hists = self._merge(self.hists, group)

    def write_to(self, group): 
        self._write(self.hists, group)
        
    def dump(self, group=None, base=''): 
        if not group: 
            group = self.hists
        for key, subgroup in group.iteritems(): 
            path = '/'.join([base, key])
            if isinstance(subgroup, dict): 
                self.dump(subgroup, path)
            else: 
                print path, subgroup.array.sum()

def hadd(config): 
    good_files = []
    for hist_file in config.input_hists: 
        with h5py.File(hist_file) as h5: 
            if len(h5.keys()): 
                good_files.append(hist_file)
    if len(good_files) != len(config.input_hists): 
        warnings.warn('only {} of {} files have any hists'.format(
                len(good_files), len(config.input_hists)))
    with h5py.File(good_files[0]) as base_h5: 
        hadder = HistAdder(base_h5)
    for add_file in good_files[1:]: 
        if not isfile(add_file): 
            raise IOError("{} doesn't exist".format(add_file))
        with h5py.File(add_file) as add_h5: 
            hadder.add(add_h5)
    if config.output_hist: 
        with h5py.File(config.output_hist,'w') as out_file: 
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
