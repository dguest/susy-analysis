from os.path import isfile
import os
import cPickle
from pyAMI import query
from pyAMI.client import AMIClient
from pyAMI.auth import AMI_CONFIG, create_auth_config
import re
import multiprocessing

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
        self.skim_path = ''
        self.hist_path = ''

        self.physics_type = ''
        self.meta_sources = set()
        self.full_name = ''
        self.full_unchecked_name = ''

    def corrected_xsec(self): 
        assert all(x > 0 for x in [
                self.total_xsec, self.kfactor, self.filteff])
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

def _get_ami_client(): 
    client = AMIClient()
    if not os.path.exists(AMI_CONFIG):
       create_auth_config()
    client.read_config(AMI_CONFIG)
    return client

class MetaFactory(object): 
    """
    This pulls together: 
     - steering list
     - susy xsection / n events info

    and generates the meta file, which is used to steer everything up to 
    histogram generation. 
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
        elif steering.endswith('.pkl'): 
            self._datasets = self._build_from_meta_pkl(steering)
        elif steering.endswith('.txt'): 
            with open(steering) as ds_list:
                self.add_ugly_ds_list(ds_list)
        else: 
            raise ValueError('{} is an unsupported file for MetaFactory'
                             'constructor')
        self.retry_ami = False
        self.no_ami_overwrite = False
        self.verbose = False

    def _build_from_meta_pkl(self, meta_file_name): 
        return DatasetCache(meta_file_name)

    def _dataset_from_name(self, entry): 
        """
        tries to build a dataset from a logical name. 
        returns partial info if that's all we can find. 
        """
        fields = entry.split('#')[0].strip().split('.')
        if fields[0] == 'user': 
            fields = non_comment_fields[2:]
        ds = Dataset()
        try: 
            ds.origin, ds.id, ds.name, prod, group, tags = fields
            required = [ 
                'SUSY' in group, 
                prod == 'merge', 
                self._find_tag(tags)
                ]
            if not all(required): 
                raise ValueError('{} is not a ds name'.format(
                        '.'.join(fields)))
                                 
            ds.tags = tags.rstrip('/')
            ds.full_unchecked_name = entry.strip()
        except ValueError as e: 
            try: 
                ds.origin, ds.id, ds.name = fields[0:3]
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

            
    def _find_tag(self, field): 
        tag_re = re.compile('e[0-9]+(_[asr][0-9]+)+_p[0-9]+')
        return tag_re.search(field)
        

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

            if not ds.id in datasets: 
                datasets[ds.id] = ds
        self._datasets = datasets

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
                ds.name = spl[1]
                ds.total_xsec_fb = float(spl[2])
                ds.kfactor = float(spl[3])
                ds.filteff = float(spl[4])
                ds.meta_sources.add('susy lookup')
    
    def _print(self, print_string): 
        if self.verbose: 
            print print_string

    def _lookup_ami_names(self, datasets): 

        client = _get_ami_client()
        pool_tuples = [(client, ds) for ds in datasets]
        pool = multiprocessing.Pool(self.processes)
        full_ds_names = pool.map(match_dataset, pool_tuples)

        for full_name, ds in zip(full_ds_names, datasets): 
            ds.full_name = full_name
        
        return {ds.id: ds for ds in datasets}

    def lookup_ami_names(self):

        datasets = self._datasets.values()
        no_full_name_ds = [ds for ds in datasets if not ds.full_name]
        
        full_named_datasets = self._lookup_ami_names(no_full_name_ds)
        self._datasets.update( full_named_datasets )

    def lookup_ami(self, client=_get_ami_client(), stream=None): 
        """
        Matches ami
        """
        tot = len(self._datasets)
        for n, (ds_id, ds) in enumerate(self._datasets.iteritems()): 
            if stream: 
                stream.write('\rlooking up data in ami'
                             ' ({} of {})'.format(n, tot))
                stream.flush()
            if 'ami' in ds.meta_sources and not self.retry_ami: 
                continue
            if 'susy lookup' in ds.meta_sources and self.no_ami_overwrite: 
                continue

            info = query.get_dataset_info(client, ds.full_name)

            self._write_ami_info_where_empty(ds, info)
        if stream: 
            stream.write('\n')

    def _write_ami_info_where_empty(self,ds, info): 
        if not ds.filteff:
            try: 
                ds.filteff = float(info.extra['GenFiltEff_mean'])
            except KeyError: 
                ds.filteff = float(info.extra['approx_GenFiltEff'])

        if not ds.total_xsec_fb: 
            try: 
                xsec = float(info.extra['crossSection_mean'])
            except KeyError:
                xsec = float(info.extra['approx_crossSection'])
            ds.total_xsec_fb = xsec * 1000.0
    
        ds.n_expected_entries = int(info.info['totalEvents'])
    
        ds.meta_sources.add('ami')
            

    def write_meta(self, output_file_name): 
        """
        write out the meta info
        """
        self._datasets.write(output_file_name)

    def get_found_full_ds_names(self): 
        full_names = []
        for ds in self._datasets.values(): 
            if ds.full_name:
                full_names.append(ds.full_name)
        return sorted(full_names)
    def get_unnamed_ds(self): 
        missing = []
        for ds in self._datasets.values(): 
            if not ds.full_name: 
                missing.append(ds)
        return missing

    def dump(self): 
        for ds_id, ds in self._datasets.iteritems(): 
            print ds.name
            print ds
            print

    def dump_sources(self): 
        for ds_id, ds in self._datasets.iteritems(): 
            print '{} -- sources: {}'.format(ds.id,' '.join(ds.meta_sources))


_wild_template = r'{o}.{ds_id}.{name}%NTUP%{tags}/'
_aggressive_wild_template = r'{o}.{ds_id}%NTUP%{tags}/'
def _wildcard_match_ami(client, ds, match_template=_wild_template): 
    wildcarded = match_template.format(
        o=ds.origin, ds_id=ds.id, name=ds.name, tags=ds.tags)

    print 'trying to match {}'.format(wildcarded)
    match_sets = query.get_datasets(client,wildcarded)
    if len(match_sets) != 1: 
        raise DatasetMatchError(
            'could not match dataset with {}'.format(wildcarded), 
            match_sets)
    return match_sets[0]['logicalDatasetName']


def match_dataset(blob): 
    client, ds = blob
    if ds.full_name: 
        return ds.full_name

    if ds.full_unchecked_name: 
        try: 
            query.get_dataset_info(client, ds.full_unchecked_name)
            return ds.full_unchecked_name
        except: 
            pass
        
    try: 
        full_name = _wildcard_match_ami(
            client, ds, _aggressive_wild_template)
        print 'matched {}'.format(full_name)
    except DatasetMatchError as e: 
        try: 
            full_name = _wildcard_match_ami(
                client, ds, _wild_template)
            print 'matched {}'.format(full_name)
        except DatasetMatchError as e: 
            full_name = None 
            print e
    return full_name

class DatasetMatchError(ValueError): 
    def __init__(self, info, matches): 
        super(DatasetMatchError, self).__init__(info)
        self.matches = matches

    def __str__(self): 
        problem = super(DatasetMatchError,self).__str__()
        problem += ' matches: '
        for match in self.matches: 
            try: 
                st = match['logicalDatasetName']
            except TypeError: 
                st = str(match)
            problem += ('\n\t' + st)
        if len(self.matches) == 0: 
            problem += ('none')
        return problem
