from os.path import basename, splitext
from collections import defaultdict
import yaml

def _key_from_path(path):
    return splitext(basename(path))[0]

class Sample:
    """
    Basic simulated sample info
    """
    def __init__(self, ds_path, sample_dict):
        ds_key = _key_from_path(ds_path)
        if '-' in ds_key:
            raise ValueError('tried to run on unmerged sample: {}'.format(
                    ds_path))
        this_samp = sample_dict[ds_key]
        self.stats = this_samp.get('n_expected_entries',0)
        self.dsid = int(ds_key[1:])
        self.type_char = ds_key[0]
        self.path = ds_path
        self.key = ds_key
        if self.stats == 0 and self.type_char not in 'jem':
            raise ValueError('no expected entries in {}'.format(ds_key))

class SampleSelector:
    """
    Select FS vs AFII samples. Constructed with file paths (not files)
    """
    def __init__(self, meta_file):
        self.min_ratio_to_select_atlfast = 1.0
        with open(meta_file) as yml:
            self.meta_dict = yaml.load(yml)

    def _get_selected(self, one_dsid):
        """
        return the samples we want from a dict corresponding to one dsid
        expects a dictionary of Samples, keyed by the id char
        """
        has_afii = 'a' in one_dsid
        has_fs = 's' in one_dsid
        if has_afii and has_fs:

            # select a sample based on stats
            fullsim = one_dsid['a'].stats
            afii = one_dsid['s'].stats
            min_ratio = self.min_ratio_to_select_atlfast
            def getstat(char):
                return one_dsid[char].stats
            take_atlfast = getstat('a') / getstat('s') > min_ratio:
            sel_char = 'a' if take_atlfast else 's'

            # return the selected sample
            yield one_dsid[sel_char]
            # ...and all the other samples
            for char in one_dsid:
                if char not in 'as':
                    yield one_dsid[char]
        else:
            yield from one_dsid.values()

    def _select_keys(self, sample_list):
        by_dsid_and_char = defaultdict(dict)
        for sample_path in sample_list:
            sample = Sample(sample_path, self.meta_dict)
            by_dsid_and_char[sample.dsid][sample.type_char] = sample

        all_keys = set()
        for dsid, sims in by_dsid_and_char.items():
            all_keys.update( (x.key for x in self._get_selected(sims) ))
        return all_keys

    def select_datasets(self, sample_list):
        select_keys = self._select_keys(sample_list)
        for key in select_keys:
            yield self.meta_dict[key]['full_name']

    def select_samples(self, sample_list):
        """
        filter a list of samples to use
        """
        select_keys = self._select_keys(sample_list)
        for path in sample_list:
            sample_key = _key_from_path(path)
            if sample_key in select_keys:
                yield path

# _________________________________________________________________________
# helper functions
def _get_char_from_ds_name(sample):
    import re
    sim_re = re.compile('\.e[0-9]+_([as])[0-9]+_')
    try:
         return sim_re.search(sample).group(1)
    except AttributeError:
        raise ValueError("can't find type of reco in {}".format(sample))

def _get_dsid(sample):
    return int(sample.split('.')[1])
