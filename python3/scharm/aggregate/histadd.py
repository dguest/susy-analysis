from scharm.hists import HistNd
import h5py
import numpy as np
from h5py import Dataset, Group
from os.path import isfile, isdir, join, splitext, basename
from itertools import chain
import os
import warnings

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
        _dash_hadd(good_files, config.output, fast=config.fast, 
                   aggressive=config.aggressive)
    else: 
        weights_dict = {}
        if config.norm: 
            raise ValueError("normalization not currently supported...")
        # NOTE: this was commented out because we're phasing out the meta
        #       wrapper.
            # lookup = meta.DatasetCache(config.norm)
            # for in_file in good_files: 
            #     file_key = basename(splitext(in_file)[0])
            #     eff_lumi = lookup[file_key].get_effective_luminosity_fb()
            #     weights_dict[in_file] = 1.0/eff_lumi
        _hadd(good_files, config.output, weights_dict, fast=config.fast)

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
        _dash_hadd(good_files, out_path, fast=config.fast, 
                   aggressive=config.aggressive)

def _dash_hadd(good_files, output, fast=False, aggressive=False): 
    def key_from_name(fname): 
        return splitext(basename(fname))[0].split('-')[0]
    if not isdir(output): 
        os.mkdir(output)
    base_keys = {key_from_name(f) for f in good_files}
    for key in base_keys: 
        out_path = join(output, '{}.h5'.format(key))
        print('making {}'.format(out_path))
        file_group = [f for f in good_files if key in f]
        missing = _get_missing_subfiles(file_group)
        if missing: 
            subfiles_str = ', '.join(str(x) for x in sorted(missing))

            prob = "file {} can't be created, missing subfiles: {}".format(
                out_path, subfiles_str)
            if aggressive: 
                warnings.warn(prob, stacklevel=2)
            else: 
                raise IOError(prob)
        _hadd(file_group, out_path, fast=fast)
    

def _get_missing_subfiles(file_group): 
    """
    checks the 'XofY' string on the end of histogram files. 
    """
    if not 'of' in file_group[0].split('-')[-1]: 
        return []
    else: 
        extensions = [f.split('-')[-1] for f in file_group]
        numbers = set()
        total_set = set()
        for ext in extensions: 
            num, tot = splitext(ext)[0].split('of')
            numbers.add(int(num))
            total_set.add(int(tot))
        total = int(next(iter(total_set)))
        if not len(total_set) == 1: 
            gname = file_group[0].split('-')[0]
            raise IOError('two totals ({}) found for {}'.format(
                    ', '.join(total_set), gname))
        if not len(numbers) == total: 
            return set(range(1, total + 1)) - numbers
        return []

def _hadd(good_files, output, weights_dict={}, fast=False):
    with h5py.File(good_files[0],'r') as base_h5:
        weight = weights_dict.get(good_files[0],1.0)
        hadder = HistAdder(base_h5, weight=weight, wt2_ext='Wt2')
        # TODO: move these counters into a class
        sum_wt = base_h5.attrs.get('total_event_weight', 0)
        total_evt = base_h5.attrs.get('total_events', 0)
    for add_file in good_files[1:]: 
        if not isfile(add_file): 
            raise IOError("{} doesn't exist".format(add_file))
        if fast: 
            weight = weights_dict.get(add_file, None)
            with h5py.File(add_file,'r') as add_h5: 
                hadder.fast_add(add_h5, weight=weight)
                sum_wt += add_h5.attrs.get('total_event_weight', 0)
                total_evt += add_h5.attrs.get('total_events', 0)
        else: 
            weight = weights_dict.get(add_file, 1.0)
            with h5py.File(add_file,'r') as add_h5: 
                hadder.add(add_h5, weight=weight)
                sum_wt += add_h5.attrs.get('total_event_weight', 0)
                total_evt += add_h5.attrs.get('total_events', 0)
    if output: 
        with h5py.File(output,'w') as out_file: 
            hadder.write_to(out_file)
            out_file.attrs['total_events'] = total_evt
            if sum_wt: 
                out_file.attrs['total_event_weight'] = sum_wt
    else:
        hadder.dump()

# ___________________________________________________________________________
# HistAdder class

class HistAdder(object): 
    """
    Generic histogram adder. Traverses the first given file to map out 
    histograms, then finds the corresponding hists for each call to add 
    and adds them. 
    """
    def __init__(self, base_group, weight=1.0, wt2_ext=None): 
        self.wt2_ext = wt2_ext
        self.hists = self._search(base_group, weight)
        
    def _search(self, group, weight): 
        subhists = {}
        for key, subgroup in group.items(): 
            if isinstance(subgroup, Group): 
                subhists[key] = self._search(subgroup, weight)
            elif isinstance(subgroup, Dataset): 
                # proper treating of weighted hists
                # FIXME: this should be a hist attribute
                if self.wt2_ext and key.endswith(self.wt2_ext): 
                    subhists[key] = HistNd(subgroup) * weight**2
                else: 
                    subhists[key] = HistNd(subgroup)*weight
            else: 
                raise HistAddError('not sure what to do with {} {}'.format(
                        type(subgroup), key))
        return subhists
    def _merge(self, hist_dict, new_hists, weight): 
        merged = {}
        for key, subgroup in hist_dict.items(): 
            if not key in new_hists: 
                raise HistAddError(
                    "node {} not found in new hists".format(key))
            if isinstance(subgroup, dict): 
                merged[key] = self._merge(subgroup, new_hists[key], weight)
            elif isinstance(subgroup, HistNd): 
                if not isinstance(new_hists[key], Dataset): 
                    raise HistAddError(
                        "tried to merge non-dataset {}".format(key))
                # proper treating of weighted hists
                if self.wt2_ext and key.endswith(self.wt2_ext): 
                    new_hist = HistNd(new_hists[key]) * weight**2
                else: 
                    new_hist = HistNd(new_hists[key])*weight
                merged[key] = subgroup + new_hist
            else: 
                raise HistAddError('not sure what to do with {}, {}'.format(
                        type(subgroup), key))
        return merged

    def _fast_merge(self, hist_dict, new_hists, weight): 
        """
        circumvents lots of error checking and array copying used in the
        normal merge. 

        Basic benchmarking: 
         - fast merge:   8.025s
         - normal merge: 21.071s
         - speedup of 2.6
        """
        keys = hist_dict.keys()
        for key in keys:
            subgroup = hist_dict[key]
            if not key in new_hists: 
                raise HistAddError(
                    "node {} not found in new hists".format(key))
            if isinstance(subgroup, dict): 
                self._fast_merge(hist_dict[key], new_hists[key], weight)
            elif isinstance(subgroup, HistNd): 
                if not isinstance(new_hists[key], Dataset): 
                    raise HistAddError(
                        "tried to merge non-dataset {}".format(key))
                # proper treating of weighted hists
                if weight is None: 
                    new_arr = np.array(new_hists[key])
                elif self.wt2_ext and key.endswith(self.wt2_ext): 
                    new_arr = np.array(new_hists[key]) * weight**2
                else: 
                    new_arr = np.array(new_hists[key])*weight

                hist_dict[key] += new_arr
            else: 
                raise HistAddError('not sure what to do with {}, {}'.format(
                        type(subgroup), key))

    def _write(self, hists, group): 
        for key, hist in hists.items(): 
            if isinstance(hist, dict): 
                subgrp = group.create_group(key)
                self._write(hist, subgrp)
            else: 
                hist.write_to(group, key)

    def add(self, group, weight=1.0): 
        self.hists = self._merge(self.hists, group, weight)
    def fast_add(self, group, weight=None): 
        self._fast_merge(self.hists, group, weight)

    def write_to(self, group): 
        self._write(self.hists, group)
        
    def dump(self, group=None, base=''): 
        if not group: 
            group = self.hists
        for key, subgroup in group.items(): 
            path = '/'.join([base, key])
            if isinstance(subgroup, dict): 
                self.dump(subgroup, path)
            else: 
                print(path, subgroup.array.sum())

class HistAddError(Exception): 
    def __init__(self, args): 
        super(HistAddError, self).__init__(args)

