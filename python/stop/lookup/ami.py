from pyAMI import query
from pyAMI.client import AMIClient
from pyAMI.auth import AMI_CONFIG, create_auth_config
import os, sys
from stop import meta 

class AmiAugmenter(object): 
    """
    Class to wrap ami augmentation. 
    """
    def __init__(self, p_tag, origin='mc12_8TeV', backup_ptag=None): 
        self.p_tag = p_tag
        self.backup_ptag = backup_ptag
        self.origin = origin
        self.ntup_filter = 'NTUP_SUSY'
        self._setup_ami_client()
        self.outstream = sys.stdout
        self.bugstream = sys.stderr

    def _setup_ami_client(self): 
        self.client = AMIClient()
        if not os.path.exists(AMI_CONFIG):
            create_auth_config()
        self.client.read_config(AMI_CONFIG)

    def get_dataset_range(self, ds_range, physics_type=None): 
        datasets = []
        for num in ds_range: 
            self.outstream.write('looking up {}, category {}\n'.format(
                    num, physics_type))
            ds = self.ds_from_id(num)
            if physics_type: 
                ds.physics_type = physics_type
            datasets.append(ds)
        ds_dict = {ds.key: ds for ds in datasets}
        return ds_dict

    def _ldn(self, ddict): 
        return ddict['logicalDatasetName']

    def ds_from_id(self, ds_id, stream=None): 
        if 'data' in self.origin: 
            args = {'run':ds_id}
        else: 
            args = {'dataset_number':str(ds_id)}

        match_sets = query.get_datasets(self.client,'%', **args)

        if not match_sets: 
            raise DatasetMatchError('found nothing with {}'.format(
                    args.items()), match_sets)
        
        if len(match_sets) > 1: 
            type_filtered = []
            for m in match_sets: 
                if self.ntup_filter in m['logicalDatasetName']: 
                    if not stream or stream in m['logicalDatasetName']:
                        type_filtered.append(m)
            match_sets = type_filtered
        if not match_sets: 
            raise DatasetMatchError('found nothing with {} and {}'.format(
                    args.items(), self.ntup_filter),[])

        if len(match_sets) > 1: 
            tagged_matches = []
            for m in match_sets: 
                if self.p_tag in m['logicalDatasetName']: 
                    tagged_matches.append(m)
            match_sets = tagged_matches
                
        if not len(match_sets) == 1:
            raise DatasetMatchError('problem matching {} with {}'.format(
                    args.items(), self.p_tag), match_sets)

        # TODO: look into how to call update_ds here to remove duplicate code
        ldn = match_sets[0]['logicalDatasetName']
        info = query.get_dataset_info(self.client, ldn)
        ds = meta.Dataset()
        ds.origin = self.origin
        ds.id = int(ds_id)
        ds.name = ldn.split('.')[2]
        ds.full_name = ldn
        self._write_ami_info(ds, info)
        if not ds.is_data: 
            self._write_mc_ami_info(ds, info)
            ds.physics_type = 'data'
        return ds

    def update_ds(self, ds, overwrite=True): 
        if ds.is_data: 
            args = {'run':str(ds.id)}
        else: 
            args = {'dataset_number':str(ds.id)}

        match_sets = query.get_datasets(self.client,'%', **args)
        primary_matches = []
        secondary_matches = []
        for match in match_sets: 
            ldn = self._ldn(match)
            if ldn.endswith(self.p_tag) and self.ntup_filter in ldn: 
                primary_matches.append(self._ldn(match))
            if ldn.endswith(self.backup_ptag) and self.ntup_filter in ldn: 
                secondary_matches.append(self._ldn(match))

        if len(primary_matches) > 1: 
            raise DatasetMatchError('problem matching {} with {}'.format(
                    ds.key, self.p_tag), primary_matches)
        info = query.get_dataset_info(self.client, primary_matches[0])
        
        def add_info(ds): 
            checks = [ 
                not ds.n_expected_entries and 'totalEvents' in info.info, 
                not ds.filteff and 'approx_GenFiltEff' in info.extra, 
                not ds.total_xsec_fb and 'approx_crossSection' in info.extra, 
                ]
            if any(checks): 
                self._write_ami_info(ds, info)
                if not ds.is_data: 
                    self._write_mc_ami_info(ds, info)
                return True
            else: 
                return False
                
        added = add_info(ds)
        if added: 
            return True
        if not secondary_matches: 
            return False
        if len(secondary_matches) > 1: 
            raise DatasetMatchError('problem matching {} with {}'.format(
                    ds.key, self.p_tag), secondary_matches)
        info = query.get_dataset_info(self.client, secondary_matches[0])
        added = add_info(ds)
        return added

    def get_datasets_year(self, year=12, stream=None): 
        datasets = {}
        periods = query.get_periods(self.client, year=year)
        hep_periods = []
        for period in periods: 
            if period.project.endswith('TeV'): 
                hep_periods.append(period.name)
        runs = query.get_runs(self.client, hep_periods, year=year)
        for run in runs: 
            self.outstream.write('looking for {}...'.format(run))
            try: 
                ds = self.ds_from_id(run, stream)
            except DatasetMatchError as err: 
                if err.matches: 
                    raise
                else: 
                    self.outstream.write('nothing\n')
                    continue
            self.outstream.write('found: {}\n'.format(ds.full_name))
            datasets[ds.key] = ds
        return datasets

    def lookup_ami_names(self, datasets, trust_names=False):
        """
        quick but pretty dirty 
        """
        if trust_names: 
            print 'assuming input names are right'
            for ds in datasets.values(): 
                try: 
                    if ds.full_unchecked_name: 
                        ds.full_name = ds.full_unchecked_name
                        del ds.full_unchecked_name
                except AttributeError: 
                    pass
            return 
        datasets = datasets.values()
        no_full_name_ds = [ds for ds in datasets if not ds.full_name]
        
        full_named_datasets = self._lookup_ami_names(no_full_name_ds)
        datasets.update( full_named_datasets )


    def _lookup_ami_names(self, datasets): 
        """
        Ugly (but maybe fast) lookup
        """
        pool_tuples = [(self.client, ds) for ds in datasets]
        pool = multiprocessing.Pool(self.processes)
        full_ds_names = pool.map(match_dataset, pool_tuples)

        for full_name, ds in zip(full_ds_names, datasets): 
            ds.full_name = full_name
            if hasattr(ds, 'full_unchecked_name'): 
                del ds.full_unchecked_name
        
        return {ds.key: ds for ds in datasets}

    def lookup_ami(self, datasets, stream=None): 
        """
        Matches ami
        """
        if self.clear_ami: 
            for ds in datasets: 
                datasets[ds].meta_sources -= set(['ami'])
        tot = len(datasets)
        for n, (ds_key, ds) in enumerate(datasets.iteritems()): 
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
            elif not ds.is_data: 
                ds.bugs.add('ambiguous dataset')

        if stream: 
            stream.write('\n')

    def _write_mc_ami_info(self, ds, info, overwrite=False):
        if not ds.filteff or overwrite:
            filteff_list = ['GenFiltEff_mean', 'approx_GenFiltEff']
            for name in filteff_list: 
                if name in info.extra: 
                    ds.filteff = float(info.extra[name])
                    break

        if not ds.filteff: 
            self.bugstream.write(
                "can't find filteff for {} {}. In info: {}\n".format(
                    ds.key, ds.name, ', '.join(info.extra.keys())))

        new_xsec = 0.0
        xsec_list = ['crossSection_mean', 'approx_crossSection']
        for name in xsec_list: 
            if name in info.extra: 
                # ami stores in nb
                new_xsec = float(info.extra[name])*1e6
                break

        if not new_xsec: 
            self.bugstream.write(
                "can't find xsection for {} {}. In info: {}\n".format(
                    ds.key, ds.name, ', '.join(info.extra.keys())))
            return 

        if not ds.total_xsec_fb or overwrite:
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
    
    def _write_ami_info(self, ds, info): 
        ds.n_expected_entries = int(info.info['totalEvents'])
        ds.meta_sources.add('ami')



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
        if not self.matches: 
            return problem + ' no matches'
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
