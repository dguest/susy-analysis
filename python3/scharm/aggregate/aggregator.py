import yaml
from scharm.hists import HistNd
from scharm.aggregate.normalizer import Normalizer
from scharm.aggregate import renamer
from os.path import basename, splitext, isfile
import h5py
import sys
import re
import numpy as np

def _get_objects(h5_cont):
    objects = {}
    for name, val in h5_cont.items():
        if isinstance(val, h5py.Group):
            objects[str(name)] = _get_objects(val)
        else:
            objects[str(name)] = list(val.shape)
    return objects

def get_all_objects(filepath):
    with h5py.File(filepath,'r') as hfile:
        objects = _get_objects(hfile)
    return objects

class HistDict(dict):
    """
    Dictionary with some methods for HDF5 persistence.

    Keyed by (physics, variable, region) tuples.

    The persistent format is actually rather ugly:
        - top dir is physics type
        - bottom dir is cut / region
        - all middle dirs are the variable name
    """
    def __init__(self, file_name='', filt=None, sig_prefix='scharm',
                 sig_points=[],
                 cut_set=None, var_blacklist=None):
        if not file_name:
            return None
        with h5py.File(file_name,'r') as infile:
            for proc, var_grp in infile.items():
                if proc.startswith(sig_prefix):
                    if proc not in sig_points:
                        continue
                for subpath in self._list_paths(var_grp):
                    path = proc + subpath
                    if filt:
                        if not filt in path:
                            continue
                    spl = path.split('/')
                    physics = spl[0]
                    if var_blacklist and set(spl[1:-1]) & var_blacklist:
                        continue
                    variable = '/'.join(spl[1:-1])
                    cut = spl[-1]
                    if cut_set and cut not in cut_set:
                        continue
                    nametup = (physics, variable, cut)
                    hist = infile[path]
                    self[nametup] = HistNd(hist)

    def __setitem__(self, key, value):
        if len(key) != 3 or not all(isinstance(k,str) for k in key):
            raise TypeError("key {} isn't right for HistDict".format(key))
        if any(len(s) == 0 for s in key):
            raise ValueError("zero-length strings prohibited: {}".format(key))
        super(HistDict,self).__setitem__(key, value)

    def _list_paths(self, group, prefix=''):
        paths = []
        for name, val in group.items():
            newprefix = '/'.join([prefix, name])
            if isinstance(val, h5py.Group):
                paths += self._list_paths(val, newprefix)
            else:
                paths.append(newprefix)
        return paths

    def write(self, file_name):
        if isfile(file_name):
            raise IOError('refusing to overwrite {}'.format(file_name))
        with h5py.File(file_name,'w') as outfile:
            for nametup, ndhist in self.items():
                physics, variable, cut = nametup
                path = '/'.join([''] + list(nametup))
                ndhist.write_to(outfile, path)


class SampleAggregator:
    """
    Takes some meta data and hists as an input, produces aggrigated
    histograms.
    """

    # the sum(weight^2) histograms have a special tag that tells
    # us to multiply by the squared normalization
    wt2_tag = 'Wt2'

    def __init__(self,meta_path, hfiles, variables, breakdown=False):
        self._normalizer = Normalizer(meta_path, hfiles)
        self.variables = variables
        self._plots_dict = HistDict()
        self._check_variables(variables)
        self._breakdown = breakdown

    def _check_variables(self, variables):
        if variables == 'all':
            return
        n_uniq_vars = len(set(variables))
        n_vars = len(variables)
        if n_vars != n_uniq_vars:
            repeats = []
            for v in set(variables):
                if variables.count(v) != 1:
                    repeats.append(v)
            raise ValueError('{} appears in inputs more than once'.format(
                    repeats))


    @property
    def plots_dict(self):
        """
        keys are tuples: (physics_type, variable, cut_name)
        """
        return self._plots_dict

    def aggregate(self):
        plots_dict = HistDict()
        for process, hfile, normalization in self._normalizer:

            # consolidate the samples (as they are in sbottom)
            if not self._breakdown:
                process = renamer.shorten(process) or process

            for region, vargroup in hfile.items():
                if self.variables == 'all':
                    variables = _get_all_variables(vargroup)
                else:
                    variables = self.variables
                for variable in variables:
                    # the sum(weight^2) histograms have a special tag
                    if variable.endswith(self.wt2_tag):
                        wt_norm = normalization**2
                    else:
                        wt_norm = normalization

                    h5hist = vargroup[variable]
                    idx_tuple = (process, variable, region)
                    if idx_tuple in plots_dict:
                        # if this hist already exists we only need the
                        # numpy array
                        hist = np.array(h5hist) * wt_norm
                        plots_dict[idx_tuple] += hist
                    else:
                        # if this plot doesn't exist, we need the full
                        # HistNd discription
                        hist = HistNd(h5hist) * wt_norm
                        plots_dict[idx_tuple] = hist

        self._plots_dict = plots_dict

    def write(self, file_name):
        self._plots_dict.write(file_name)
    def read(self, file_name):
        self._plots_dict = HistDict(file_name)

def _get_all_variables(group, prepend=''):
    if not isinstance(group, h5py.Group):
        return [prepend.strip('/')]
    variables = []
    for subname, subgroup in group.items():
        name = prepend + '/' + subname
        newvals = _get_all_variables(subgroup, name)
        variables += newvals
    return variables
