from stop import meta
from stop.hists import HistNd
from os.path import basename, splitext, isfile
import h5py
import warnings
import sys
import re
from collections import Counter
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
    with h5py.File(filepath) as hfile: 
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
    def __init__(self, file_name='', filt=None, physics_set=None, 
                 cut_set=None, var_blacklist=None): 
        if not file_name: 
            return None
        with h5py.File(file_name) as infile: 
            for path in self._list_paths(infile): 
                if filt: 
                    if not filt in path: 
                        continue
                spl = path.split('/')
                physics = spl[1]
                if physics_set and physics not in physics_set: 
                    continue
                if var_blacklist and set(spl[2:-1]) & var_blacklist: 
                    continue
                variable = '/'.join(spl[2:-1])
                cut = spl[-1]
                if cut_set and cut not in cut_set: 
                    continue
                nametup = (physics, variable, cut)
                hist = infile[path]
                self[nametup] = HistNd(hist)

    def __setitem__(self, key, value): 
        if len(key) != 3 or not all(isinstance(k,basestring) for k in key): 
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
            for nametup, ndhist in self.iteritems():
                physics, variable, cut = nametup
                path = '/'.join([''] + list(nametup))
                ndhist.write_to(outfile, path)
    

class SampleAggregator(object): 
    """
    Takes some meta data and whiskey as an input, produces aggrigated 
    histograms. 
    """
    signal_name_template_met_filter = ( 
        '{}-{stop_mass_gev}-{lsp_mass_gev}-TMF{met_filter_gev:.0f}')
    signal_name_template = '{}-{stop_mass_gev}-{lsp_mass_gev}'

    def __init__(self,meta_path, whiskey, variables): 
        self.whiskey = whiskey
        self.variables = variables
        self.filter_meta = meta.DatasetCache(meta_path)
        self.lumi_fb = 20.661
        self.signal_prestring = 'stop'
        self._plots_dict = HistDict()
        self.outstream = sys.stdout
        self.bugstream = sys.stderr
        self._check_variables(variables)
        self.out_prepend = ''

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

    def _get_matched_signame(self,ds): 
        """
        this is slightly hackish, but does the right thing by renaming the 
        physics type. 
        """
        old_finder = re.compile('directCC_([0-9]+)_([0-9]+)')
        met_finder = finder = re.compile(
            'stopToCharmLSP_t([0-9]+)_n([0-9]+)_MET([0-9]+)')

        found = met_finder.findall(ds.full_name)
        if not found: 
            found = old_finder.findall(ds.full_name)
        if not found: 
            return None
        generator_info = { 
                'stop_mass_gev': int(found[0][0]), 
                'lsp_mass_gev': int(found[0][1]), 
                }
        if len(found) == 3: 
            generator_info['met_filter_gev'] = found[0][2]

        namestring = self.signal_name_template
        return namestring.format(self.signal_prestring,
                                 **generator_info)
        return None

    def _get_physics_type(self, file_meta): 
        physics_type = file_meta.physics_type
        if not physics_type:
            if file_meta.is_data: 
                physics_type = 'data'
            else: 
                raise OSError('got unknown physics in {}'.format(f))

        if physics_type == 'signal': 
            physics_type = self._get_matched_signame(file_meta)
        if not physics_type: 
            raise OSError("couldn't classify {}".format(
                    file_meta.full_name))
        return physics_type

    def _check_for_bugs(self, ds): 
        if not ds.total_xsec_fb and not ds.is_data: 
            self.bugstream.write(
                'no cross section for {} {}, skipping\n'.format(
                    ds.key, ds.name))
            return True
        if ds.n_corrupted_files: 
            self.bugstream.write(
                '{} bad files in {}\n'.format(ds.n_corrupted_files, ds.key))
            
        return False

    @property
    def plots_dict(self): 
        """
        keys are tuples: (physics_type, variable, cut_name)
        """
        return self._plots_dict


    def _get_lumi_scale(self, file_meta): 
        if file_meta.is_data: 
            return 1.0
        else: 
            try: 
                eff_lumi_fb = file_meta.get_effective_luminosity_fb()
            except meta.EffectiveLuminosityError as exc: 
                eff_lumi_fb = exc.best_guess_fb
                bugline = '{}: {}\n'.format(
                    file_meta.name, str(exc))
                self.bugstream.write(bugline)
            if not eff_lumi_fb: 
                raise ArithmeticError(
                    'lumi problem in {}'.format(file_meta.full_name))
                    
            return self.lumi_fb / eff_lumi_fb

    def aggregate(self): 
        merged_files = Counter()
        plots_dict = HistDict()
        numfiles = len(self.whiskey)
        for filenum, f in enumerate(self.whiskey): 
            if self.outstream and self.outstream.isatty():
                self.outstream.write(
                    '\r{}adding file {} of {}'.format(
                        self.out_prepend, filenum, numfiles))
                self.outstream.flush()
            meta_name = basename(splitext(f)[0])
            if meta_name not in self.filter_meta: 
                short_name = meta_name.split('-')[0]
                if short_name != meta_name: 
                    merged_files[short_name] += 1
                    meta_name = short_name
                else: 
                    self.bugstream.write('could not identify {}\n'.format(
                            meta_name))
                    continue
    
            file_meta = self.filter_meta[meta_name]
            if self._check_for_bugs(file_meta): 
                continue

            lumi_scale = self._get_lumi_scale(file_meta)
            def scale_hist(variable, hist): 
                if variable.endswith('Wt2'): 
                    hist *= lumi_scale**2.0
                else: 
                    hist *= lumi_scale
                return hist

            physics_type = self._get_physics_type(file_meta)
    
            with h5py.File(f) as hfile: 
                for cut_name, vargroup in hfile.iteritems(): 
                    if self.variables == 'all': 
                        variables = _get_all_variables(vargroup)
                    else: 
                        variables = self.variables
                    for variable in variables:
                        h5hist = vargroup[variable]
                        idx_tuple = (physics_type, variable, cut_name)
                        if idx_tuple in plots_dict: 
                            # if this hist already exists we only need the 
                            # numpy array 
                            hist = np.array(h5hist)
                            hist = scale_hist(variable, hist)
                            plots_dict[idx_tuple] += hist
                        else: 
                            # if this plot doesn't exist, we need the full
                            # HistNd discription
                            hist = HistNd(h5hist) 
                            hist = scale_hist(variable, hist)
                            plots_dict[idx_tuple] = hist

        if self.outstream and self.outstream.isatty(): 
            self.outstream.write('\n')
        self._plots_dict = plots_dict
        for mkey, mcount in merged_files.iteritems(): 
            expected_count = self.filter_meta[mkey].total_subsets
            if mcount != expected_count: 
                self.bugstream.write(
                    '{}: expected {} files, only found {}\n'.format(
                        mkey, expected_count, mcount))

    def write(self, file_name): 
        self._plots_dict.write(file_name)
    def read(self, file_name): 
        self._plots_dict = HistDict(file_name)
                
def _get_all_variables(group, prepend=''): 
    if not isinstance(group, h5py.Group): 
        return [prepend.strip('/')]
    variables = []
    for subname, subgroup in group.iteritems(): 
        name = prepend + '/' + subname
        newvals = _get_all_variables(subgroup, name)
        variables += newvals
    return variables

                        
class MissingCacheError(IOError): 
    """
    This is supposed to get thrown if something isn't in the cache. 
    Not currently used. 
    """
    def __init__(self,variable, physics, what=''): 
        self.variable = variable
        self.physics = physics
        if not what: 
            what = 'var: {}, in {} sample missing'.format(variable, physics)
        super(MissingCacheError,self).__init__(what)