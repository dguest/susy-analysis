#!/usr/bin/env python2.7
"""
Top level script for vairous generic data manipulation. 
"""

_meta_help="""
Top level for routines that print / filter / manipulate meta data. 
Routines here share common filters. 
"""

_rename_help="""
Swaps the ds key file names for something more readable. 
"""

_group_help="""
Groups d3pds into datasets by file. Will break larger datasets into multiple
outputs. 
"""

import argparse, sys
from os.path import isdir, isfile, join, expanduser, splitext
from os.path import dirname, basename, abspath
import os
import re, glob
import yaml
import warnings
import h5py
from itertools import chain
from stop.hists import HistNd, HistAdder
from stop import meta
from collections import defaultdict

def run(): 
    config = get_config()
    subs = {
        'group': group_input_files, 
        'meta':list_meta_info, 
        'hadd':hadd, 'rename':rename}
    subs[config.which](config)


def get_config(): 
    d = 'default: %(default)s'
    c = "with no argument is '%(const)s'"

    parser = argparse.ArgumentParser(description=__doc__)
    subs = parser.add_subparsers(dest='which')

    # meta stuff
    meta_list = subs.add_parser('meta', description=_meta_help)
    meta_filters = meta_list.add_argument_group(
        title='filters')
    list_type = meta_filters.add_mutually_exclusive_group()
    list_type.add_argument('--physics', help='filter meta by physics')
    list_type.add_argument('--anti-physics', help='show all but matches')
    meta_filters.add_argument(
        '--name-regex', help='filter full dataset name')
    meta_filters.add_argument(
        '-d', '--dir-filter', help='only use keys in this dir')
    meta_list.add_argument('meta_file')
    meta_list.add_argument(
        '-o', '--output-format', default='names', 
        choices={'files','meta','names','keys'})

    # rename stuff
    meta_rename = subs.add_parser('rename', description=_rename_help)
    meta_rename.add_argument('meta_file')
    meta_rename.add_argument('files', nargs='+')
    meta_rename.add_argument('-d', '--dummy', action='store_true')

    # hadd stuff 
    hist_add = subs.add_parser('hadd') 
    hist_add.add_argument('input_hists', nargs='+')
    hist_add.add_argument('-o', '--output', 
                          help='hist or (for dash-hadd) directory')
    hist_add_method = hist_add.add_mutually_exclusive_group()
    hist_add_method.add_argument(
        '-d', '--dash-hadd', action='store_true', 
        help='multiple hadds, splits input hists at \'-\'')
    hist_add_method.add_argument(
        '-r', '--recursive', action='store_true', 
        help='multiple hadds, splits input by dir and hists at \'-\'')
    hist_add.add_argument('--norm', help=(
            'normalize using this meta file (scales to 1 fb^-1)'))

    group = subs.add_parser('group', description=_group_help)
    group.add_argument(
        'input_dirs', nargs='+', 
        help='dataset directories, must each have a dsid')
    group.add_argument(
        '-n', '--n-files-per-job', type=int, default=5, 
        help='number of root files to put in each out file, ' + d)
    group.add_argument('-o', '--output-dir', default='batch/d3pds', 
                       help=d)

    return parser.parse_args(sys.argv[1:])


def list_meta_info(config): 
    from stop.meta import DatasetCache
    meta = DatasetCache(config.meta_file)
    filt_meta = {}
    filters = [config.physics, config.anti_physics, 
               config.name_regex, config.dir_filter]

    check_name = lambda x: True
    if config.name_regex: 
        name_re = re.compile(config.name_regex)
        def check_name(name): 
            return name_re.search(name)

    check_dir = lambda x: True
    if config.dir_filter: 
        files = glob.glob('{}/*.h5'.format(config.dir_filter))
        files += glob.glob('{}/*.root'.format(config.dir_filter))
        keys = set(splitext(basename(f).split('-')[0])[0] for f in files)
        def check_dir(key): 
            return key in keys

    if any(filters): 
        for ds_key, ds in meta.iteritems(): 
            pass_conditions = [ 
                not config.physics or ds.physics_type == config.physics, 
                not config.anti_physics or (
                    ds.physics_type != config.anti_physics), 
                check_name(ds.full_name), 
                check_dir(ds.key)
                ]
            if all(pass_conditions): 
                filt_meta[ds_key] = ds

    else: 
        filt_meta = meta

    subopts = {'files': list_files, 'meta': list_meta, 
               'names':list_names, 'keys': list_keys}
    subopts[config.output_format](config, filt_meta)

def list_files(config, filt_meta): 
    if not config.dir_filter: 
        raise ValueError('--dir-filter is required')
    for ds in filt_meta.values(): 
        files = glob.glob('{}/*{}*'.format(config.dir_filter, ds.key))
        for f in files: 
            print f

def list_names(config, filt_meta): 
    for ds in filt_meta.values(): 
        print '{}/'.format(ds.full_name)
def list_meta(config, filt_meta): 
    meta = {}
    for ds in filt_meta.values(): 
        meta[ds.key] = ds.yml_dict()
    print yaml.dump(meta)
def list_keys(config, filt_meta): 
    for ds in filt_meta.values(): 
        print ds.key

def hadd(config): 
    """
    Scales to 1 fb^-1 if scaling is requested. 
    """
    if config.recursive: 
        _recursive_hadd(config)
        return 
    good_files = _get_good_files(config.input_hists)
    if config.dash_hadd: 
        if config.norm: 
            raise ValueError('normalization not allowed for dash-hadd')
        _dash_hadd(good_files, config.output)
    else: 
        weights_dict = {}
        if config.norm: 
            lookup = meta.DatasetCache(config.norm)
            for in_file in good_files: 
                file_key = basename(splitext(in_file)[0])
                eff_lumi = lookup[file_key].get_effective_luminosity_fb()
                weights_dict[in_file] = 1.0/eff_lumi
        _hadd(good_files, config.output, weights_dict)

def _get_good_files(input_hists): 
    good_files = []
    for hist_file in input_hists: 
        if not hist_file.endswith('.h5'): 
            raise OSError("unrecognized extension: {}".format(
                    splitext(hist_file)[-1]))
        with h5py.File(hist_file, 'r') as h5: 
            if len(h5.keys()): 
                good_files.append(hist_file)
    if len(good_files) != len(input_hists): 
        sys.stderr.write(
            'ACHTUNG: only {} of {} files have any hists\n'.format(
                len(good_files), len(input_hists)))
    return good_files

def _recursive_hadd(config): 
    if not all(isdir(x) for x in config.input_hists): 
        raise OSError("recursive hadd requires input_hists to be dir")
    all_walk = chain(*(os.walk(x) for x in config.input_hists))
    for dirpath, dirnames, file_names in all_walk: 
        if not file_names: 
            continue
        out_path = join(config.output, *dirpath.split(os.path.sep)[1:])
        file_paths = [join(dirpath, x) for x in file_names]
        good_files = _get_good_files(file_paths)
        if isdir(out_path): 
            raise OSError(
                "output directory {} already exists, "
                " refusing overwrite".format(out_path))
        os.makedirs(out_path)
        _dash_hadd(good_files, out_path)

def _dash_hadd(good_files, output): 
    def key_from_name(fname): 
        return splitext(basename(fname))[0].split('-')[0]
    if not isdir(output): 
        os.mkdir(output)
    base_keys = {key_from_name(f) for f in good_files}
    for key in base_keys: 
        out_path = join(output, '{}.h5'.format(key))
        print 'making {}'.format(out_path)
        file_group = [f for f in good_files if key in f]
        if not _ok_extensions(file_group): 
            raise IOError(
                "file {} doens't have the right number of subfiles")
        _hadd(file_group, out_path)
    

def _ok_extensions(file_group): 
    """
    checks the 'XofY' string on the end of histogram files. 
    """
    if not 'of' in file_group[0].split('-')[-1]: 
        return True
    else: 
        extensions = [f.split('-')[-1] for f in file_group]
        numbers = set()
        total = set()
        for ext in extensions: 
            num, tot = splitext(ext)[0].split('of')
            numbers.add(num)
            total.add(tot)
        if not len(total) == 1: 
            return False
        if not len(numbers) == int(next(iter(total))): 
            return False
        return True

def _hadd(good_files, output, weights_dict={}):
    with h5py.File(good_files[0]) as base_h5:
        weight = weights_dict.get(good_files[0],1.0)
        hadder = HistAdder(base_h5, weight=weight, wt2_ext='Wt2')
    for add_file in good_files[1:]: 
        if not isfile(add_file): 
            raise IOError("{} doesn't exist".format(add_file))
        weight = weights_dict.get(add_file, 1.0)
        with h5py.File(add_file) as add_h5: 
            hadder.add(add_h5, weight=weight)
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

def group_input_files(config): 
    files_by_dsid = defaultdict(set)
    dsid_finder = re.compile('mc.._.TeV\.([0-9]{6})\.')
    run_finder = re.compile('data.._.TeV\.([0-9]{8})\.')
    for in_dir in config.input_dirs: 
        prechar = 's'           # s is for simulation 
        dsid_match = dsid_finder.search(in_dir)
        if not dsid_match: 
            dsid_match = run_finder.search(in_dir)
            prechar = 'd'       # data
        if not dsid_match: 
            raise IOError("can't find dsid for {}".format(in_dir))
        ds_key = prechar + dsid_match.group(1).lstrip('0')
        sub_root_files = set(glob.glob('{}/*.root*'.format(in_dir)))
        files_by_dsid[ds_key] |= sub_root_files
    
    if not isdir(config.output_dir): 
        os.makedirs(config.output_dir)

    for dsid, files in files_by_dsid.iteritems(): 
        file_list = sorted(files)
        n_files = len(file_list)
        subsets = []
        n_per_job = config.n_files_per_job
        for iii in xrange(0, n_files, n_per_job): 
            subset = file_list[iii:iii+n_per_job]
            subsets.append(subset)
        n_subsets = len(subsets)
        strlen_subsets = len(str(n_subsets))
        for set_n, subset in enumerate(subsets,1): 
            out_name = '{}-{:0{}d}of{}.txt'.format(
                dsid, set_n, strlen_subsets, n_subsets)
            if n_subsets == 1: 
                out_name = '{}.txt'.format(dsid)
            out_path = join(config.output_dir, out_name)
            with open(out_path,'w') as out_file: 
                for ds in subset: 
                    out_file.write(ds + '\n')
                    
    

if __name__ == '__main__': 
    run()
