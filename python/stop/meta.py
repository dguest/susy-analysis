
from os.path import isfile
import cPickle

class Dataset(object): 
    """
    container for dataset info
    """
    def __init__(self): 
        self.id = 0
        self.name = ''
        self.kfactor = -1
        self.filteff = -1

        self.n_raw_entries = 0
        self.total_xsec = -1

        self.d3pds = []
        self.skim_path = ''
        self.hist_path = ''

    def corrected_xsec(self): 
        assert all(x >= 0 for x in [self.total_xsec, self.kfactor, self.filteff])
        return self.total_xsec * self.kfactor * self.filteff
    @property
    def xsec_per_evt(self): 
        """
        corrected cross section / number of events
        """
        return self.corrected_xsec() / self.evts

class DatasetContainer(dict): 
    """
    A dict where dataset meta is stored. 

    Has a constructor / write method to take care of pickling. 
    In the future the constructor may deal with schema changes. 
    """
    def __init__(self, cache_name): 
        self._cache_name = cache_name
        if isfile(cache_name): 
            with open(cache_name) as cache: 
                cached_dict = cPickle.loac(cache)
            super(DatasetContainer, self).__init__(cached_dict)

    def __enter__(self): 
        return self
    def __exit__(self, exe_type, exe_val, exe_tb): 
        if exe_type is None: 
            self.write()

    def write(self, cache_name=''): 
        if not cache_name: 
            cache_name = self._cache_name
        with open(cache_name, 'w') as cache: 
            out_dict = dict(self)
            cPickle.dump(out_dict, cache)

