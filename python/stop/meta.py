
from os.path import isfile
import cPickle
from pyAMI import get_dataset_info

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
        self.n_expected_entries = 0

        self.d3pds = []
        self.skim_path = ''
        self.hist_path = ''

        self.physics_type = ''
        self.meta_source = ''
        self.full_name = ''

    def corrected_xsec(self): 
        assert all(x >= 0 for x in [self.total_xsec, self.kfactor, self.filteff])
        return self.total_xsec * self.kfactor * self.filteff
    @property
    def xsec_per_evt(self): 
        """
        corrected cross section / number of events
        """
        return self.corrected_xsec() / self.evts

class DatasetCache(dict): 
    """
    A dict where dataset meta is stored. 

    Has a constructor / write method to take care of pickling. 
    In the future the constructor may deal with schema changes. 

    (hopefully that means yaml at some point)
    """
    def __init__(self, cache_name): 
        self._cache_name = cache_name
        if isfile(cache_name): 
            with open(cache_name) as cache: 
                cached_dict = cPickle.loac(cache)
            super(DatasetCache, self).__init__(cached_dict)

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

class MetaFactory(object): 
    """
    This pulls together: 
     - steering list
     - susy xsection / n events info

    and generates the meta file, which is used to steer everything up to 
    histogram generation. 
    """
    def __init__(self, ds_list): 
        """
        list of datasets to map out. If a list entry has two values, the 
        second is the physics type
        """
        self._datasets = DatasetCache()
        self.mc_tag = 'mc12_8TeV'
        self.p_tag = 'p1181'
        self.datatype_tag = 'SUSY'

        for entry in ds_list: 
            non_comment_fields = entry.split('#')[0].strip().split()
            ds_name = non_comment_fields[0]
            dataset = Dataset()
            dataset.name = ds_name
            self._datasets[ds_name] = dataset

    def lookup_susy(self, susy_file): 
        """
        Matches whatever datasets have a cross section listed in the susy file.
        """
        id_line = ''
        for line in susy_file: 

            # first line of the susy textfile is some kind of 
            # field description         
            if not id_line: 
                id_line = line
                continue

            line = line.split('#')[0].strip()
            if not line: 
                continue
            spl = line.split()
            name = spl[0]
            if name in self._datasets: 
                ds = self._datasets[name]
                ds.id = int(spl[0])
                ds.name = spl[1]
                ds.total_xsec = float(spl[2])
                ds.kfactor = float(spl[3])
                ds.filteff = float(spl[4])
                ds.meta_source = 'susy textfile'
                self._datasets[name] = ds # TODO: is this line needed???
    
    def lookup_ami(self, client): 
        """
        Matches ami
        """
        for ds_key, ds in self._datasets.iteritems(): 
            wildcarded = r'{mc}%{key}%{datatype}%{p_tag}/'.format(
                mc=self.mc_tag, key=ds.name, datatype=self.datatype_tag, 
                p_tag=self.p_tag)
            match_sets = query.get_datasets(wildcarded)
            if len(match_sets) != 1: 
                raise LookupError(
                    'match problem with {}, matches: {}'.format(
                        ds_key, match_sets))
            full_name = match_sets[0]
            info = query.get_dataset_info(client, full_name)
            ds.kfactor = 1.0
            try: 
                ds.filteff = info.extra['GenFiltEff_mean']
            except KeyError: 
                ds.filteff = info.extra['approx_GenFiltEff']

            try: 
                ds.total_xsec = info.extra['crossSection_mean']
            except KeyError:
                ds.total_xsec = info.extra['approx_crossSection']

            ds.n_expected_entries = info.info['totalEvents']

            ds.full_name = full_name
            ds.meta_source = 'ami'
            

    def write_meta(self, output_file_name): 
        """
        write out the meta info
        """
        pass
