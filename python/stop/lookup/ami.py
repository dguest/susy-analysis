from pyAMI import query
from pyAMI.client import AMIClient
from pyAMI.auth import AMI_CONFIG, create_auth_config
import os, sys, re
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
        self.atlfinder = re.compile('(_a([0-9])+)+')

    def _setup_ami_client(self): 
        self.client = AMIClient()
        if not os.path.exists(AMI_CONFIG):
            create_auth_config()
        self.client.read_config(AMI_CONFIG)

    def _filter_datasets(self, in_list, filt_func): 
        if not in_list: 
            raise DatasetMatchError('found nothing to filter')
        filtered = []
        for m in in_list: 
            if filt_func(m): 
                filtered.append(m)
        if not filtered: 
            raise DatasetMatchError(
                'filter removed all {} with {}'.format(
                    args.items(), self.ntup_filter),in_list)
        return filtered

    def get_dataset_range(self, ds_range, physics_type=None, atlfast=False): 
        datasets = []
        for num in ds_range: 
            self.outstream.write('looking up {}, category {}\n'.format(
                    num, physics_type))
            ds = self.ds_from_id(num, atlfast=atlfast)
            if physics_type: 
                ds.physics_type = physics_type
            datasets.append(ds)
        ds_dict = {ds.key: ds for ds in datasets}
        return ds_dict

    def _ldn(self, ddict): 
        return ddict['logicalDatasetName']

    def ds_from_id(self, ds_id, stream=None, atlfast=False): 
        if 'data' in self.origin: 
            args = {'run':ds_id}
        else: 
            args = {'dataset_number':str(ds_id)}

        match_sets = query.get_datasets(self.client,'%', **args)

        if not match_sets: 
            raise DatasetMatchError('found nothing with {}'.format(
                    args.items()), match_sets)
        
        # TODO: replace all these calls with the _filter_datasets function
        if len(match_sets) > 1: 
            type_filtered = []
            for m in match_sets: 
                if not stream or stream in m['logicalDatasetName']:
                    type_filtered.append(m)
            if not type_filtered: 
                raise DatasetMatchError(
                    'stream filter removed all {} with {}'.format(
                        args.items(), self.ntup_filter),match_sets)
            match_sets = type_filtered

        if len(match_sets) > 1: 
            type_filtered = []
            for m in match_sets: 
                ldn = self._ldn(m)
                if self.ntup_filter in ldn: 
                    type_filtered.append(m)
            if not type_filtered: 
                raise DatasetMatchError(
                    'type filter removed all {} with {}'.format(
                        args.items(), self.ntup_filter),match_sets)
            
            match_sets = type_filtered

        if len(match_sets) > 1: 
            tagged_matches = []
            for m in match_sets: 
                if self.p_tag in m['logicalDatasetName']: 
                    tagged_matches.append(m)
            if not tagged_matches: 
                raise DatasetMatchError(
                    'p filter removed all {} with {}'.format(
                        args.items(), self.ntup_filter),match_sets)
            match_sets = tagged_matches
    
        if len(match_sets) > 1: 
            atlmatch = []
            for m in match_sets: 
                if bool(self.atlfinder.search(self._ldn(m))) == atlfast: 
                    atlmatch.append(m)
            if not atlmatch: 
                raise DatasetMatchError(
                    'atlfilter filter removed all {} with {}'.format(
                        args.items(), self.ntup_filter),match_sets)
                
        if len(match_sets) == 0:
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
            exact = [m for m in primary_matches if m == ds.full_name]
            if len(exact) == 1: 
                primary_matches = exact
            else: 
                raise DatasetMatchError(
                    'problem matching {} with {}'.format(
                        ds.key, self.p_tag), primary_matches)
        info = query.get_dataset_info(self.client, primary_matches[0])
        
        def add_info(ds): 
            checks = [ 
                not ds.n_expected_entries and 'totalEvents' in info.info, 
                not ds.filteff and 'approx_GenFiltEff' in info.extra, 
                not ds.total_xsec_fb and 'approx_crossSection' in info.extra, 
                ]
            if any(checks) or overwrite: 
                self._write_ami_info(ds, info)
                if not ds.is_data: 
                    self._write_mc_ami_info(ds, info, overwrite)
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

class DatasetMatchError(ValueError): 
    def __init__(self, info, matches): 
        super(DatasetMatchError, self).__init__(info)
        self.matches = matches

    def __str__(self): 
        problem = super(DatasetMatchError,self).__str__()
        if not self.matches: 
            return problem + ', no matches'
        problem += ', matches: '
        for match in self.matches: 
            try: 
                st = match['logicalDatasetName']
            except TypeError: 
                st = str(match)
            problem += ('\n\t' + st)
        if len(self.matches) == 0: 
            problem += ('none')
        return problem
