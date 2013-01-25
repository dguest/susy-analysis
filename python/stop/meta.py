
from os.path import isfile
import cPickle
from pyAMI import query

class Dataset(object): 
    """
    container for dataset info
    """
    def __init__(self): 
        self.id = 0
        self.name = ''
        self.kfactor = 1
        self.filteff = 1

        self.n_raw_entries = 0
        self.total_xsec = -1
        self.n_expected_entries = 0

        self.d3pds = []
        self.skim_path = ''
        self.hist_path = ''

        self.physics_type = ''
        self.meta_sources = set()
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
    def __str__(self): 
        def stringify(name): 
            return '{:20}:{}'.format(name,getattr(self,name))

        out = ''
        nonsimple = ['corrected_xsec','xsec_per_evt']
        for attrib in dir(self):
            if attrib.startswith('__') or attrib in nonsimple: 
                continue
            if out: 
                out += '\n'
            out += stringify(attrib)
        return out

class DatasetCache(dict): 
    """
    A dict where dataset meta is stored. 

    Has a constructor / write method to take care of pickling. 
    In the future the constructor may deal with schema changes. 

    (hopefully that means yaml at some point)
    """
    def __init__(self, cache_name=''): 
        self._cache_name = cache_name
        if isfile(cache_name): 
            with open(cache_name) as cache: 
                cached_dict = cPickle.load(cache)
            super(DatasetCache, self).__init__(cached_dict)

    def __enter__(self): 
        return self
    def __exit__(self, exe_type, exe_val, exe_tb): 
        if exe_type is None: 
            self.write()

    def write(self, cache_name=''): 
        if not cache_name: 
            cache_name = self._cache_name
            if not cache_name: 
                raise ValueError('no file name given')
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
    def __init__(self, cache=None): 
        """
        """
        self._datasets = cache
        if cache is None: 
            self._datasets = DatasetCache()
        self.mc_tag = 'mc12_8TeV'
        self.p_tag = 'p1181'
        self.datatype_tag = 'SUSY'
        self.retry_ami = False
        self.no_ami_overwrite = False
        self.verbose = False

    def build_base_ds(self, ds_list): 
        """
        this should probably work based on the full ds name (unfortunately)
        """
        for entry in ds_list: 
            non_comment_fields = entry.split('#')[0].strip().split()
            if not non_comment_fields: 
                continue
            ds_id = non_comment_fields[0]
            if ds_id in self._datasets: 
                continue
            dataset = Dataset()
            dataset.id = ds_id
            self._datasets[ds_id] = dataset

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

            clean_line = line.split('#')[0].strip()
            if not clean_line: 
                continue

            spl = clean_line.split()
            name = spl[1]
            ds_id = spl[0]
            if ds_id in self._datasets: 
                ds = self._datasets[ds_id]
                ds.id = int(spl[0])
                if ds.name and not ds.name == spl[1]: 
                    self._print('what the fuck {} != {}'.format(
                            ds.name, spl[1]))
                ds.total_xsec = float(spl[2])
                ds.kfactor = float(spl[3])
                ds.filteff = float(spl[4])
                ds.meta_sources.add('susy lookup')
    
    def _print(self, print_string): 
        if self.verbose: 
            print print_string

    def lookup_ami(self, client): 
        """
        Matches ami
        """
        for ds_id, ds in self._datasets.iteritems(): 
            if 'ami' in ds.meta_sources and not self.retry_ami: 
                continue
            if 'susy lookup' in ds.meta_sources and self.no_ami_overwrite: 
                continue
            wildcarded = r'{mc}%{key}%{datatype}%{p_tag}%'.format(
                mc=self.mc_tag, key=ds.id, datatype=self.datatype_tag, 
                p_tag=self.p_tag)
            self._print('trying to match {}'.format(wildcarded))
            match_sets = query.get_datasets(client,wildcarded)
            if len(match_sets) != 1: 
                set_names = [s['logicalDatasetName'] for s in match_sets]
                self._print('match problem with {}, matches:'.format(
                        wildcarded))
                for s in set_names: 
                    self._print('\t' + s)
                continue
            full_name = match_sets[0]['logicalDatasetName']
            self._print( 'matched: {}'.format(full_name))
            info = query.get_dataset_info(client, full_name)

            try: 
                ds.filteff = float(info.extra['GenFiltEff_mean'])
            except KeyError: 
                ds.filteff = float(info.extra['approx_GenFiltEff'])

            try: 
                ds.total_xsec = float(info.extra['crossSection_mean'])
            except KeyError:
                ds.total_xsec = float(info.extra['approx_crossSection'])

            ds.n_expected_entries = int(info.info['totalEvents'])

            ds.full_name = full_name
            ds.meta_sources.add('ami')
            

    def write_meta(self, output_file_name): 
        """
        write out the meta info
        """
        print 'writing {}'.format(output_file_name)
        self._datasets.write(output_file_name)

    def dump(self): 
        for name, ds in self._datasets.iteritems(): 
            print name
            print ds

    def dump_sources(self): 
        for name, ds in self._datasets.iteritems(): 
            print '{}: {}'.format(name,ds.meta_sources)
