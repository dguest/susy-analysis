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
        this_samp = sample_dict[ds_key]
        self.stats = this_samp['n_expected_entries']
        self.dsid = int(ds_key[1:])
        self.type_char = ds_key[0]
        self.path = ds_path
        self.key = ds_key

class SampleSelector: 
    """
    Select FS vs AFII samples
    """
    def __init__(self, meta_file): 
        self.min_ratio_to_select_atlfast = 1.0
        with open(meta_file) as yml:
            self.meta_dict = yaml.load(yml)

    def _get_selected(self, one_dsid): 
        """
        return the samples we want from a dict corresponding to one dsid
        """
        has_afii = 'a' in one_dsid
        has_fs = 's' in one_dsid
        if has_afii and has_fs: 
            fullsim = one_dsid['a'].stats
            afii = one_dsid['s'].stats
            min_ratio = self.min_ratio_to_select_atlfast
            if one_dsid['a'].stats / one_dsid['s'].stats > min_ratio: 
                yield one_dsid['a']
            else: 
                yield one_dsid['s']
            for char in one_dsid: 
                if char not in 'as': 
                    yield one_dsid[char]
        else: 
            yield from one_dsid.values()

    def select_samples(self, sample_list):
        """
        filter a list of paths from 
        """
        by_dsid_and_char = defaultdict(dict)
        for sample_path in sample_list: 
            sample = Sample(sample_path, self.meta_dict)
            by_dsid_and_char[sample.dsid][sample.type_char] = sample

        all_keys = set()
        for dsid, sims in by_dsid_and_char.items(): 
            all_keys.update( (x.key for x in self._get_selected(sims) ))

        for path in sample_list: 
            sample_key = _key_from_path(path)
            if sample_key in all_keys: 
                yield path
                
        