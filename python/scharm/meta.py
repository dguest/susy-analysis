from os.path import isfile
import os, re
import cPickle
import yaml
import copy
from warnings import warn

# 's' is for fullsim, 'a' for atlfast
stream_schema = {
    'm':'Muon',
    'e':'Egamma',
    'j':'JetTauEtmiss'
}

class Dataset(object):
    """
    Container for dataset info.
    """
    atlfinder = re.compile('(_a([0-9]+))+')
    merge_required_match = [
        'origin','id','name','tags','kfactor','filteff',
        'n_expected_entries', 'physics_type', 'total_subsets',
        'meta_sources']
    merge_safe_to_copy = merge_required_match + [
        'total_xsec_fb', 'full_name',
        ]

    def __init__(self, full_name=''):
        self.origin = ''
        self.id = 0
        self.name = ''
        self.tags = ''
        self.full_name = full_name
        if full_name:
            origin, dsid, name, pstp, fmt, tags = full_name.split('.')
            self.origin = origin
            self.id = int(dsid)
            self.name = name
            self.tags = tags.strip('/')

        self.kfactor = 0
        self.filteff = 0

        self.total_xsec_fb = 0
        self.n_expected_entries = 0
        self.sum_event_weight = 0.0

        self.physics_type = ''
        self.meta_sources = set()
        self.preferred = None   # True or False

        self.n_corrupted_files = 0
        self.overlap = {}

        self._cached_key = None

        # --- DEPRECATED ---
        self._bugs = set()

        self.subset_index = 0
        self.total_subsets = 0

        self.d3pds = []
        self.skim_paths = {}

    def get_effective_luminosity_fb(self):
        if not self.total_xsec_fb:
            error = "no cross section for {}: {}".format(
                self.key, self.full_name)
            raise ArithmeticError(error)

        sum_evt_wt = self.sum_event_weight

        if not self.kfactor and not self.filteff:
            eff_lumi = sum_evt_wt / self.total_xsec_fb
            raise EffectiveLuminosityError(
                eff_lumi, ['kfactor', 'filter efficiency'])

        kfactor = self.kfactor
        if not kfactor:
            if not self.physics_type == 'signal':
                corrected_x = self.total_xsec_fb * self.filteff
                eff_lumi = sum_evt_wt / corrected_x
                raise MissingKfactorError(eff_lumi)
            else:
                kfactor = 1.0


        if not self.filteff:
            corrected_x = self.total_xsec_fb * self.kfactor
            eff_lumi = sum_evt_wt / corrected_x
            raise MissingFilterEfficiencyError(eff_lumi)
        else:
            filt_eff = self.filteff

        assert self.total_xsec_fb
        assert kfactor
        assert filt_eff
        corrected_x = self.total_xsec_fb * kfactor * filt_eff
        return sum_evt_wt / corrected_x

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
                key.startswith('_'),
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
            if not hasattr(self, key):
                raise MetaFromYamlError("{} isn't a member of {}".format(
                        key, type(self).__name__))
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
        if not self._cached_key:
            self._cached_key = self._key()
        return self._cached_key
    @property
    def mergekey(self):
        warn("trying to remove merging of dataset meta", FutureWarning,
             stacklevel=2)
        return self._key(merge=True)

    def _get_char(self):
        if self.origin.startswith('mc'):
            if self.atlfinder.search(self.tags):
                return 'a'
            return 's'
        elif self.origin.startswith('data'):
            char = 'd'
            for ch, stream in stream_schema.items():
                if stream in self.name:
                    return ch
            warn('not sure which stream {} comes from'.format(self.id))
            return 'd'
        else:
            warn("not sure if ds with id {} is mc or data".format(self.id))
            return 'q'

    def _key(self, merge=False):
        """
        should be used to identify datasets in the dict
        """
        char = self._get_char()
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
        self.exception_dump = 'META_DUMP.yml'

        if isinstance(cache_name, str) and isfile(cache_name):
            if cache_name.endswith('.pkl'):
                with open(cache_name) as cache:
                    cached_dict = cPickle.load(cache)
                super(DatasetCache, self).__init__(cached_dict)
            elif cache_name.endswith('.yml'):
                with open(cache_name) as cache:
                    yml_dict = yaml.load(cache)
                self._from_yml(yml_dict)
            else:
                raise OSError('extension not recognized: {}'.format(
                        cache_name))

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
        else:
            new_name = os.path.splitext(cache_name)[0] + '.yml'
            warn("didn't recognize extension on {}, writing {}".format(
                    cache_name, new_name), stacklevel=2)
            self._write_to_file(new_name)

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

        if not isfile(txt_name) or not self.text_extension in txt_name:
            raise MissingMetaError("can't find meta", txt_name, root_file)
        with open(txt_name) as textfile:
            values = textfile.readlines()
            if len(values) == 0:
                raise MissingMetaError('no n_events', txt_name, root_file)
            n_events = int(values[0])
            if len(values) > 1:
                sum_evt_weight = float(values[1])
            else:
                sum_evt_weight = 0.0
        return n_events, sum_evt_weight
    def get_recorded_events(self, d3pd_list, aggressive=False):
        """
        returns a tuple: recorded events. If aggressive will mutate list
        by removing corrupted files
        """
        n_events = 0
        total_sum_wt = 0.0
        bad_d3pds = []
        for d3pd in d3pd_list:
            try:
                n_raw, sum_wt = self._get_recorded_events(d3pd)
                n_events += n_raw
                total_sum_wt += sum_wt
            except MissingMetaError as err:
                if not aggressive:
                    raise
                bad_d3pds.append(err.root_file)
        for bad in bad_d3pds:
            d3pd_list.remove(bad)
        n_corrupted = len(bad_d3pds)
        return n_events, total_sum_wt, n_corrupted

class MetaFromYamlError(OSError):
    def __init__(self, error):
        super(MetaFromYamlError, self).__init__(error)

class MissingMetaError(IOError):
    """
    Raised by meta collector if some info can't be found.
    """
    def __init__(self, message, meta_file=None, root_file=None):
        super(MissingMetaError,self).__init__(message)
        self.meta_file = meta_file
        self.root_file = root_file
    def __str__(self):
        if self.meta_file:
            return self.message + ', metafile: ' + self.meta_file
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
