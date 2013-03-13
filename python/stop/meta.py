from os.path import isfile
import os
import cPickle
import re
import multiprocessing
import yaml
from warnings import warn

class Dataset(object): 
    """
    container for dataset info
    """
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

    @property
    def skim_path(self): 
        warn('skim_path is deprecated, use dict skim_paths', FutureWarning)
        return self.skim_paths['nominal']
    @skim_path.setter
    def skim_path(self, value): 
        warn('skim_path is deprecated, use dict skim_paths', FutureWarning)
        self.skim_paths['nominal'] = value

    @property
    def effective_luminosity_fb(self): 
        if not self.total_xsec_fb: 
            raise ArithmeticError(
                "no cross section can't calculate effective luminosity")

        if not self.kfactor and not self.filteff: 
            eff_lumi = self.n_raw_entries / self.total_xsec_fb
            raise EffectiveLuminosityException(
                eff_lumi, ['kfactor', 'filter efficiency'])

        kfactor = self.kfactor
        if not kfactor: 
            if not self.physics_type == 'signal': 
                corrected_x = self.total_xsec_fb * self.filteff
                eff_lumi = self.n_raw_entries / corrected_x
                raise MissingKfactorException(eff_lumi)
            else: 
                kfactor = 1.0


        if not self.filteff: 
            corrected_x = self.total_xsec_fb * self.kfactor
            eff_lumi = self.n_raw_entries / corrected_x
            raise MissingFilterEfficiencyException(eff_lumi)
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

        for key, value in self.__dict__.iteritems(): 
            skip_conditions = [ 
                not value and value is not False, 
                key == 'full_unchecked_name', 
                self.full_name and key in build_from_full_name, 
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
        """
        should be used to identify datasets in the dict
        """
        if self.origin.startswith('mc'): 
            char = 's'
        elif self.origin.startswith('data'): 
            char = 'd'
        else: 
            warn("not sure if ds with id {} is mc or data".format(self.id))
            char = 'q'
        ds_id = self.id
        if isinstance(ds_id, str): 
            ds_id = ds_id[0].upper() + ds_id[1:]
        return '{}{}'.format(char, ds_id)
    @property
    def is_data(self): 
        if self.origin.startswith('data'): 
            return True
        else:
            return False

class DatasetCache(dict): 
    """
    A dict where dataset meta is stored. 

    Has a constructor / write method to take care of pickling. 
    In the future the constructor may deal with schema changes. 

    (hopefully that means yaml at some point)
    """
    def __init__(self, cache_name=''): 
        self._cache = cache_name
            
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
                    


class MetaFactory(object): 
    """
    This pulls together: 
     - steering list
     - susy xsection / n events info

    and generates the meta file, which is used to steer everything up to 
    histogram generation. 

    Should work on stripping this down and making it into a susy 
    lookup class. 
    """
    def __init__(self, steering=None): 
        """
        Initalize from either: 
         - a text file list of datasets
         - an existing meta file
        """
        self._datasets = None
        if steering is None: 
            self._datasets = DatasetCache()
        elif steering.endswith('.pkl') or steering.endswith('.yml'): 
            self._datasets = self._build_from_meta(steering)
        elif steering.endswith('.txt'): 
            with open(steering) as ds_list:
                self.add_ugly_ds_list(ds_list)
        else: 
            raise ValueError('{} is an unsupported file for MetaFactory'
                             'constructor')
        self.clear_ami = False
        self.no_ami_overwrite = False
        self.verbose = False

    def _build_from_meta(self, meta_file_name): 
        return DatasetCache(meta_file_name)

    def _dataset_from_name(self, entry): 
        """
        tries to build a dataset from a logical name. 
        returns partial info if that's all we can find. 
        """
        entry = entry.split('_tid')[0]

        fields = entry.split('#')[0].strip().split('.')
        
        if not any(fields[0].startswith(x) for x in ['data','mc']): 
            stripped_name = '.'.join(fields[1:])
            if stripped_name: 
                try: 
                    return self._dataset_from_name(stripped_name)
                except ValueError as er: 
                    if "can't find origin" in str(er): 
                        raise ValueError("can't find origin for {}".format(
                                entry.strip()))
                    else: 
                        raise
            else: 
                raise ValueError("can't find origin")
        ds = Dataset()
        try: 
            ds.origin, ds_id, ds.name, prod, group, tags = fields
            required = [ 
                'SUSY' in group, 
                prod == 'merge', 
                self._find_tag(tags)
                ]

            if not all(required): 
                raise ValueError('{} is not a ds name'.format(
                        '.'.join(fields)))

            ds.id = self._get_id(ds_id)
            ds.tags = tags.rstrip('/')
            ds.full_unchecked_name = entry.strip().rstrip('/')
        except ValueError as e: 
            try: 
                ds.origin, ds_id, ds.name = fields[0:3]
                ds.id = self._get_id(ds_id)
                others = fields[3:]
                for field in others: 
                    if self._find_tag(field): 
                        ds.tags = field.rstrip('/')

            except ValueError as e: 
                if 'to unpack' in str(e):
                    raise ValueError(
                        "{} isn't parseable as ds name".format(entry))
                else: 
                    raise 
        return ds
            
    def _get_id(self, ds_id): 
        try: 
            return int(ds_id)
        except ValueError: 
            if not isinstance(ds_id, str): 
                raise ValueError(
                    "ds id {} should be an int or string".format(ds_id))
            return ds_id

    def _find_tag(self, field): 
        mc_re = re.compile('e[0-9]+(_[asr][0-9]+)+_p[0-9]+')
        data_re = re.compile('f[0-9]+(_[a-z][0-9]+)+_p[0-9]+')
        mainz_data_re = re.compile('(re|t0)pro[0-9]+_v[0-9]+_p[0-9]+')
        regexes = [mc_re, data_re, mainz_data_re]
        for regex in regexes: 
            if regex.search(field): 
                return True
        return False

    @property
    def datasets(self): 
        return self._datasets

    def add_ugly_ds_list(self, ds_list): 
        """
        this has been hacked to work with vincent's weird ds names
        """
        if self._datasets:
            datasets = self._datasets
        else:
            datasets = DatasetCache()

        for entry in ds_list: 
            real_entry = entry.split('#')[0].strip()
            if not real_entry: 
                continue
            ds = self._dataset_from_name(real_entry)

            if not ds.key in datasets: 
                datasets[ds.key] = ds
        self._datasets = datasets

    def lookup_susy(self, susy_file): 
        """
        Matches whatever datasets have a cross section listed in the susy file.
        """
        id_line = ''
        split_dict = {}
        for line in susy_file: 

            # first line of the susy textfile is some kind of 
            # field description         
            if not id_line: 
                id_line = line
                continue

            clean_line = line.split('#')[0].strip()
            if not clean_line: 
                continue

            spl = clean_line.split()
            try: 
                name = spl[1]
                ds_id = int(spl[0])
            except IndexError: 
                raise ValueError("can't parse {}".format(clean_line))
            split_dict[ds_id] = spl
            

        for ds in self._datasets.values(): 
            if ds.id in split_dict: 
                spl = split_dict[ds.id]
                ds.id = int(spl[0])
                ds.name = spl[1]
                ds.total_xsec_fb = float(spl[2]) * 1e3
                ds.kfactor = float(spl[3])
                ds.filteff = float(spl[4])
                ds.meta_sources.add('susy lookup')
    
    def _print(self, print_string): 
        if self.verbose: 
            print print_string

    def write_meta(self, output_file_name): 
        """
        write out the meta info
        """
        self._datasets.write(output_file_name)

    def dump(self): 
        for ds_key, ds in self._datasets.iteritems(): 
            print ds.name
            print ds
            print

    def dump_sources(self): 
        for ds_key, ds in self._datasets.iteritems(): 
            print '{} -- sources: {}'.format(ds.id,' '.join(ds.meta_sources))


class EffectiveLuminosityException(ArithmeticError): 
    def __init__(self, best_guess_fb, missing_values, 
                 problem='missing lumi normalization info'): 
        super(EffectiveLuminosityException, self).__init__(problem)
        self.best_guess_fb = best_guess_fb
        self.missing_values = missing_values
    def __str__(self): 
        problem = 'missing: {}, best guess: {} fb'.format(
            ', '.join(self.missing_values), self.best_guess_fb)
        return problem
    
class MissingKfactorException(EffectiveLuminosityException): 
    def __init__(self, best_guess): 
        super(MissingKfactorException, self).__init__(
            best_guess, ['kfactor'])

class MissingFilterEfficiencyException(EffectiveLuminosityException): 
    def __init__(self, best_guess): 
        super(MissingFilterEfficiencyException).__init__(
            best_guess,['filter efficiency'])
