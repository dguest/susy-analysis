from os.path import isfile
import os
import cPickle
from pyAMI import query
from pyAMI.client import AMIClient
from pyAMI.auth import AMI_CONFIG, create_auth_config
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
        if not self.kfactor: 
            warn('no kfactor for {} {}, assuming 1'.format(
                    self.id, self.name))
            kfactor = 1.0
        else: 
            kfactor = self.kfactor
        if not self.filteff: 
            warn('no filt eff for {} {}, assuming 1'.format(
                    self.id, self.name)) 
            filt_eff = 1.0
        else: 
            filt_eff = self.filteff

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
        self._cache_name = cache_name
        if isfile(cache_name): 
            if cache_name.endswith('.pkl'): 
                with open(cache_name) as cache: 
                    cached_dict = cPickle.load(cache)
                super(DatasetCache, self).__init__(cached_dict)
            if cache_name.endswith('.yml'): 
                with open(cache_name) as cache: 
                    yml_dict = yaml.load(cache)
                for ds_key, yml_ds in yml_dict.iteritems(): 
                    ds = Dataset()
                    ds.from_yml(yml_ds)
                    if ds_key != ds.key: 
                        warn('changing ds keys in {}'.format(cache_name))
                    self[ds.key] = ds


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
        if cache_name.endswith('.pkl'): 
            with open(cache_name, 'w') as cache: 
                out_dict = dict(self)
                cPickle.dump(out_dict, cache)
        elif cache_name.endswith('.yml'): 
            with open(cache_name, 'w') as cache: 
                out_list = [(ds.key, ds.yml_dict()) for ds in self.values()]
                out_dict = dict(out_list)
                cache.write(yaml.dump(out_dict))
                    

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
            return ds_id[0].upper() + ds_id[1:]

    def _find_tag(self, field): 
        mc_re = re.compile('e[0-9]+(_[asr][0-9]+)+_p[0-9]+')
        data_re = re.compile('f[0-9]+(_[a-z][0-9]+)+_p[0-9]+')
        mainz_data_re = re.compile('(re|t0)pro[0-9]+_v[0-9]+_p[0-9]+')
        regexes = [mc_re, data_re, mainz_data_re]
        for regex in regexes: 
            if regex.search(field): 
                return True
        return False
        

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
                ds.bugs -= set(['no cross section','no filter efficiency'])
    
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
            if hasattr(ds, 'full_unchecked_name'): 
                del ds.full_unchecked_name
        
        return {ds.key: ds for ds in datasets}

    def lookup_ami_names(self, trust_names=False):

        if trust_names: 
            print 'assuming input names are right'
            for ds in self._datasets.values(): 
                try: 
                    if ds.full_unchecked_name: 
                        ds.full_name = ds.full_unchecked_name
                        del ds.full_unchecked_name
                except AttributeError: 
                    pass
                
            return 

        datasets = self._datasets.values()
        no_full_name_ds = [ds for ds in datasets if not ds.full_name]
        
        full_named_datasets = self._lookup_ami_names(no_full_name_ds)
        self._datasets.update( full_named_datasets )

    def lookup_ami(self, client=_get_ami_client(), stream=None): 
        """
        Matches ami
        """
        if self.clear_ami: 
            for ds in self._datasets: 
                self._datasets[ds].meta_sources -= set(['ami'])
        tot = len(self._datasets)
        for n, (ds_key, ds) in enumerate(self._datasets.iteritems()): 
            if stream: 
                stream.write('\rlooking up data in ami'
                             ' ({} of {})'.format(n, tot))
                stream.flush()
            if 'ami' in ds.meta_sources:
                continue
            if 'susy lookup' in ds.meta_sources and self.no_ami_overwrite: 
                continue

            if ds.full_name: 
                info = query.get_dataset_info(client, ds.full_name)
                if ds.origin.startswith('mc'): 
                    self._write_mc_ami_info(ds, info)
                self._write_ami_info(ds, info)
            else: 
                ds.bugs.add('ambiguous dataset')
            

        if stream: 
            stream.write('\n')

    def _write_mc_ami_info(self, ds, info):
        if not ds.filteff:
            try: 
                ds.filteff = float(info.extra['GenFiltEff_mean'])
            except KeyError: 
                try: 
                    ds.filteff = float(info.extra['approx_GenFiltEff'])
                except KeyError: 
                    ds.bugs.add('no filter efficiency')
                    ds.filteff = 1.0

        try: 
            new_xsec = self._get_ami_cross_section(info)
        except KeyError: 
            if not ds.total_xsec_fb:
                ds.bugs.add('no cross section')
            return 

        if not ds.total_xsec_fb:
            ds.total_xsec_fb = new_xsec
        else:
            diff = ds.total_xsec_fb - new_xsec
            rel_dif = abs(diff / ds.total_xsec_fb)
            if rel_dif > 0.1: 
                warn('for sample {id} {name}: '
                     'ami gives xsec of {ami} fb, '
                     'susytools gives {st} (diff {diff:.1%})'.format(
                        id=ds.id, name=ds.name, 
                        ami=new_xsec, st=ds.total_xsec_fb, 
                        diff=rel_dif))


    def _get_ami_cross_section(self,info): 
        xsec = 0
        try: 
            xsec = float(info.extra['crossSection_mean']) 
        except KeyError:
            xsec = float(info.extra['approx_crossSection']) 
        
        return xsec * 1e6
    
    def _write_ami_info(self, ds, info): 
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
        for ds_key, ds in self._datasets.iteritems(): 
            print ds.name
            print ds
            print

    def dump_sources(self): 
        for ds_key, ds in self._datasets.iteritems(): 
            print '{} -- sources: {}'.format(ds.id,' '.join(ds.meta_sources))


_wild_template = r'{o}.{ds_id}.{name}%NTUP%{tags}/'
_susy_wild_template = r'{o}.{ds_id}%NTUP_SUSY%{tags}/'
def _wildcard_match_ami(client, ds, match_template=_wild_template): 
    if ds.is_data: 
        if isinstance(ds.id, str): 
            raise DatasetMatchError('not even trying to match dataperiod','')
        ds_id = '{:08}'.format(ds.id)
    else: 
        ds_id = '{:06}'.format(ds.id)
    wildcarded = match_template.format(
        o=ds.origin, ds_id=ds_id, name=ds.name, tags=ds.tags)

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

    try: 
        if ds.full_unchecked_name: 
            query.get_dataset_info(client, ds.full_unchecked_name)
            return ds.full_unchecked_name
    except: 
        pass
        
    try: 
        full_name = _wildcard_match_ami(
            client, ds, _wild_template)
        print 'matched {}'.format(full_name)
    except DatasetMatchError as e: 
        try: 
            full_name = _wildcard_match_ami(
                client, ds, _susy_wild_template)
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
