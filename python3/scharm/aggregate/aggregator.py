import yaml
from scharm.hists import HistNd
from scharm.aggregate.normalizer import Normalizer
from scharm.aggregate import renamer
from scharm.aggregate.aggschema import HistDict
from os.path import basename, splitext
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


class SampleAggregator:
    """
    Takes some meta data and hists as an input, produces aggrigated
    histograms.
    """

    # the sum(weight^2) histograms have a special tag that tells
    # us to multiply by the squared normalization
    wt2_tag = 'Wt2'

    def __init__(self,meta_path, hfiles, variables, breakdown=False,
                 quiet=False, ignored=set()):
        self._normalizer = Normalizer(meta_path, hfiles, quiet=quiet)
        self.variables = variables
        self._plots_dict = HistDict()
        self._check_variables(variables)
        self._breakdown = breakdown
        self._renamer = renamer.Renamer()
        self._ignored = ignored

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

            if process in self._ignored:
                continue

            # consolidate the samples (as they are in sbottom)
            if not self._breakdown:
                process = self._renamer.shorten(process) or process

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
