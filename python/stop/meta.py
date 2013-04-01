from os.path import isfile
import os
import cPickle
import yaml
import copy
from warnings import warn

class Dataset(object): 
    """
    Container for dataset info. 
    """
    merge_required_match = [
        'origin','id','name','tags','kfactor','filteff', 
        'n_expected_entries', 'physics_type', 'total_subsets', 
        'meta_sources']
    merge_safe_to_copy = merge_required_match + [ 
        'total_xsec_fb', 'full_name', 
        ]
    
    def __init__(self): 
        self.origin = ''
        self.id = 0
        self.name = ''
        self.tags = ''
        self.kfactor = 0
        self.filteff = 0

        self.n_raw_entries = 0
        self.total_xsec_fb = 0
        self.n_expected_entries = 0

        self.d3pds = []
        self.skim_paths = {}

        self.physics_type = ''
        self.meta_sources = set()
        self.full_name = ''

        self.bugs = set()
        self.n_corrupted_files = 0

        self.subset_index = 0
        self.total_subsets = 0

    def __add__(self, other): 
        for key in self.merge_required_match: 
            if not getattr(self, key) == getattr(other, key): 
                raise ValueError(
                    "mismatch in {}: tried to add {} to {}".format(
                        key, getattr(self,key), getattr(other,key)))
        
        new = Dataset()
        for key in self.merge_safe_to_copy: 
            new.__dict__[key] = copy.deepcopy(self.__dict__[key])
        new.n_raw_entries = self.n_raw_entries + other.n_raw_entries
        new.d3pds = self.d3pds + other.d3pds
        new.n_corrupted_files = (
            self.n_corrupted_files + other.n_corrupted_files)
        
        subsets = set()
        def add_subsets(sub, subsets): 
            if isinstance(sub, int): 
                subsets.add(sub)
            else: 
                subsets |= sub
        add_subsets(self.subset_index, subsets)
        add_subsets(other.subset_index, subsets)
        if len(subsets) == new.total_subsets: 
            subsets = 'merged'
        new.subset_index = subsets
        return new

    def split(self, n_subsets): 
        """
        Splits the dataset meta, dividing d3pds among the subsets. 
        """
        if self.skim_paths or self.n_corrupted_files: 
            raise NotImplementedError(
                "don't know how to split post-distiller ds")
        subsets = []
        for n in xrange(n_subsets): 
            subset = copy.deepcopy(self)
            subset.d3pds = []
            subset.subset_index = n
            subset.total_subsets = n_subsets
            subsets.append(subset)
        for n, d3pd in enumerate(self.d3pds): 
            subsets[n % n_subsets].d3pds.append(d3pd)
        return {s.key: s for s in subsets}

    @property
    def effective_luminosity_fb(self): 
        if not self.total_xsec_fb: 
            raise ArithmeticError(
                "no cross section can't calculate effective luminosity")

        if not self.kfactor and not self.filteff: 
            eff_lumi = self.n_raw_entries / self.total_xsec_fb
            raise EffectiveLuminosityError(
                eff_lumi, ['kfactor', 'filter efficiency'])

        kfactor = self.kfactor
        if not kfactor: 
            if not self.physics_type == 'signal': 
                corrected_x = self.total_xsec_fb * self.filteff
                eff_lumi = self.n_raw_entries / corrected_x
                raise MissingKfactorError(eff_lumi)
            else: 
                kfactor = 1.0


        if not self.filteff: 
            corrected_x = self.total_xsec_fb * self.kfactor
            eff_lumi = self.n_raw_entries / corrected_x
            raise MissingFilterEfficiencyError(eff_lumi)
        else: 
            filt_eff = self.filteff

        assert self.total_xsec_fb
        assert kfactor
        assert filt_eff
        corrected_x = self.total_xsec_fb * kfactor * filt_eff
        return self.n_raw_entries / corrected_x

    def __str__(self): 
        def stringify(name): 
            return '{:20}:{}'.format(name,getattr(self,name))

        out = ''
        for attrib in self.__dict__:

            if out: 
                out += '\n'
            out += stringify(attrib)
        return out
    def _yml_iter(self): 
        build_from_full_name = set(['id', 'origin', 'tags', 'name'])

        if self.total_subsets: 
            yield 'total_subsets', self.total_subsets
            yield 'subset_index', self.subset_index

        for key, value in self.__dict__.iteritems(): 
            skip_conditions = [ 
                not value and value is not False, 
                key == 'full_unchecked_name', 
                self.full_name and key in build_from_full_name, 
                key in ['subset_index','total_subsets'], 
                ]
            if any(skip_conditions): 
                continue
            if isinstance(value, set):
                yield key, list(value)
            else: 
                yield key, value
    def yml_dict(self): 
        return dict(self._yml_iter())

    def from_yml(self, yml_dict): 
        for key, value in yml_dict.items(): 
            if hasattr(self,key) and isinstance(getattr(self, key), set): 
                value = set(value)
            elif key == 'full_name': 
                spl = value.split('.')
                self.origin = spl[0]
                self.id = int(spl[1])
                self.name = spl[2]
                self.tags = spl[-1]
            setattr(self, key, value)

    @property
    def key(self): 
        return self._key()
    @property 
    def mergekey(self): 
        return self._key(merge=True)

    def _key(self, merge=False): 
        """
        should be used to identify datasets in the dict
        """
        if self.origin.startswith('mc'): 
            char = 's'
        elif self.origin.startswith('data'): 
            if 'Muon' in self.name: 
                char = 'm'
            elif 'JetTauEtmiss' in self.name: 
                char = 'd'
            else: 
                warn('not sure which stream {} comes from'
                     ' assume jet'.format(self.id))
                char = 'd'
        else: 
            warn("not sure if ds with id {} is mc or data".format(self.id))
            char = 'q'
        ds_id = self.id
        if isinstance(ds_id, str): 
            ds_id = ds_id[0].upper() + ds_id[1:]
        if not self.total_subsets or self.subset_index == 'merged' or merge: 
            return '{}{}'.format(char, ds_id)
        else: 
            if isinstance(self.subset_index,set): 
                substr = ','.join(sorted(self.subset_index))
            else: 
                substr = self.subset_index
            return '{}{}-{}'.format(char, ds_id, substr)
    @property
    def is_data(self): 
        if self.origin.startswith('data'): 
            return True
        else:
            return False

class DatasetCache(dict): 
    """
    A dict where dataset meta is stored. 

    Has a constructor / write method to take care of pickling / yaml.
    """
    def __init__(self, cache_name=''): 
        self._cache = cache_name
        self.exception_dump = 'meta-dump.yml'
            
        if isinstance(cache_name, str) and isfile(cache_name): 
            if cache_name.endswith('.pkl'): 
                with open(cache_name) as cache: 
                    cached_dict = cPickle.load(cache)
                super(DatasetCache, self).__init__(cached_dict)
            if cache_name.endswith('.yml'): 
                with open(cache_name) as cache: 
                    yml_dict = yaml.load(cache)
                self._from_yml(yml_dict)

    def _from_yml(self, yml_dict): 
        for ds_key, yml_ds in yml_dict.iteritems(): 
            ds = Dataset()
            ds.from_yml(yml_ds)
            if ds_key != ds.key: 
                warn('changing ds keys in {}'.format(self._cache))
            self[ds.key] = ds


    def __enter__(self): 
        return self
    def __exit__(self, exe_type, exe_val, exe_tb): 
        if exe_type is None: 
            self.write()
        else: 
            self.write(self.exception_dump)
            warn('wrote {} due to caught exception in exit'.format(
                    self.exception_dump))
            

    def write(self, cache=''): 
        if not cache: 
            cache = self._cache
        if not cache: 
            raise ValueError('no file to write to')

        if isinstance(cache, str): 
            self._write_to_file(cache)
        else: 
            out_list = [(ds.key, ds.yml_dict()) for ds in self.values()]
            out_dict = dict(out_list)
            cache.write(yaml.dump(out_dict))
                

    def _write_to_file(self, cache_name): 
        if cache_name.endswith('.pkl'): 
            with open(cache_name, 'w') as cache: 
                out_dict = dict(self)
                cPickle.dump(out_dict, cache)
        elif cache_name.endswith('.yml'): 
            with open(cache_name, 'w') as cache: 
                out_list = [(ds.key, ds.yml_dict()) for ds in self.values()]
                out_dict = dict(out_list)
                cache.write(yaml.dump(out_dict))
                    

class MetaTextCollector(object): 
    """
    Class to gather meta stored in textfiles produced alongside d3pd skims.
    """
    def __init__(self, root_extension='skim-output.root', 
                 text_extension='n-entries.txt'): 
        self.root_extension = root_extension
        self.text_extension = text_extension
        
    def _get_recorded_events(self, root_file): 
        txt_name = root_file.replace(self.root_extension,
                                     self.text_extension)
        if not isfile(txt_name): 
            raise MissingMetaError("can't find meta", txt_name)
        with open(txt_name) as textfile: 
            n_events = int(next(textfile))
        return n_events
    def get_recorded_events(self, d3pd_list): 
        n_events = 0
        for d3pd in d3pd_list: 
            n_events += self._get_recorded_events(d3pd)
        return n_events

class MissingMetaError(IOError): 
    def __init__(self, message, metafile=None): 
        super(MissingMetaError,self).__init__(message)
        self.metafile = metafile
    def __str__(self): 
        if self.metafile: 
            return self.message + ', metafile: ' + self.metafile
        else: 
            return message

class EffectiveLuminosityError(ArithmeticError): 
    def __init__(self, best_guess_fb, missing_values, 
                 problem='missing lumi normalization info'): 
        super(EffectiveLuminosityError, self).__init__(problem)
        self.best_guess_fb = best_guess_fb
        self.missing_values = missing_values
    def __str__(self): 
        problem = 'missing: {}, best guess: {} fb'.format(
            ', '.join(self.missing_values), self.best_guess_fb)
        return problem
    
class MissingKfactorError(EffectiveLuminosityError): 
    def __init__(self, best_guess): 
        super(MissingKfactorError, self).__init__(
            best_guess, ['kfactor'])

class MissingFilterEfficiencyError(EffectiveLuminosityError): 
    def __init__(self, best_guess): 
        super(MissingFilterEfficiencyError).__init__(
            best_guess,['filter efficiency'])
