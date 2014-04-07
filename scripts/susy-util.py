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

_cutflow_help="""
Gets the aggregated cutflow from lots of files.
"""

_check_help="""
Check meta file for missing events
"""

import argparse, sys
from os.path import isdir, isfile, join, expanduser, splitext
from os.path import dirname, basename, abspath
import os
import re, glob
import yaml
import warnings
from itertools import chain
from scharm.hists import HistNd, HistAdder
from scharm import meta
from collections import defaultdict, Counter

def run():
    config = get_config()
    subs = {
        'group': group_input_files,
        'meta':list_meta_info,
        'rename':rename,
        'cutflow':cutflow}
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

    # group stuff
    group = subs.add_parser('group', description=_group_help)
    group.add_argument(
        'input_dirs', nargs='+',
        help='dataset directories, must each have a dsid')
    group.add_argument(
        '-g', '--gb-per-job', type=int, default=0.5,
        help='number of root files to put in each out file, ' + d)
    group.add_argument('-o', '--output-dir', default='batch/d3pds',
                       help=d)

    # cutflow stuff
    cutflow = subs.add_parser('cutflow', description=_cutflow_help)
    cutflow.add_argument('count_files', nargs='+')

    return parser.parse_args(sys.argv[1:])

def cutflow(config):
    cut_counts = Counter()
    with open(config.count_files[0]) as first_file:
        cuts = [c[0] for c in yaml.load(first_file)]
    cutset = set(cuts)
    for f in config.count_files:
        with open(f) as count_file:
            cut_list = yaml.load(count_file)
        these_cuts = set(c[0] for c in cut_list)
        if these_cuts - cutset:
            if cutset - these_cuts:
                raise ValueError('weird {}'.format(cutset - these_cuts))
            cuts = [c[0] for c in cut_list]
            cutset = set(cuts)
        cut_counts.update(dict(cut_list))
    cut_strlen = max(len(x) for x in cuts) + 1
    for cut in cuts:
        print '{:>{}}: {}'.format(cut, cut_strlen, cut_counts[cut])

def list_meta_info(config):
    from scharm.meta import DatasetCache
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

# __________________________________________________________________________
# file grouping stuff (being migrated to a setup script)

def _file_group_gen(files, chunk_size=500e6):
    tot_size = 0.0
    out_ntuples = []
    for path in files:
        size = os.path.getsize(path)
        if tot_size + size > chunk_size and out_ntuples:
            yield out_ntuples
            tot_size = 0.0
            out_ntuples = []
        out_ntuples.append(path)
        tot_size += size
    if out_ntuples:
        yield out_ntuples

def group_input_files(config):
    from scharm.schema import get_prechar
    files_by_dsid = defaultdict(set)
    dsid_finder = re.compile('mc.._.TeV\.([0-9]{6})\.')
    run_finder = re.compile('data.._.TeV\.([0-9]{8})\.')
    for in_dir in config.input_dirs:
        prechar = get_prechar(in_dir)
        dsid_match = dsid_finder.search(in_dir)
        if not dsid_match:
            dsid_match = run_finder.search(in_dir)
        if not dsid_match:
            raise IOError("can't find dsid for {}".format(in_dir))
        ds_key = prechar + dsid_match.group(1).lstrip('0')
        sub_root_files = set(glob.glob('{}/*.root*'.format(in_dir)))
        files_by_dsid[ds_key] |= sub_root_files

    if not isdir(config.output_dir):
        os.makedirs(config.output_dir)

    for dsid, files in files_by_dsid.iteritems():
        file_list = sorted(files)
        random.shuffle(file_list)
        subsets = []
        for file_group in _file_group_gen(file_list, config.gb_per_job):
            subsets.append(file_group)
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
