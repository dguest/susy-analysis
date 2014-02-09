from pyAMI import query
from pyAMI.client import AMIClient
from pyAMI.auth import AMI_CONFIG, create_auth_config
import os, sys, re
from collections import defaultdict
from scharm import meta, bullshit

def _filter_by_ldn(match_sets, *strings): 
    for string in strings:
        if string is None: 
            continue
        filtered = []
        if len(match_sets) == 1: 
            return match_sets
        for ds in match_sets: 
            if string in ds['logicalDatasetName']:
                filtered.append(ds)
        if not filtered: 
            raise DatasetMatchError(
                '{} in LDN removed all matches'.format(string), match_sets)
        match_sets = filtered

    return match_sets

def _get_expected_counts(client, ds): 
    if ds is None: 
        return None
    ldn = _ldn(ds)
    info = query.get_dataset_info(client, ldn)
    matched_counts = int(info.info['totalEvents'])
    return matched_counts


def _ldn(ddict): 
    return ddict['logicalDatasetName']

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
        ds_dict = {}
        for num in ds_range: 
            self.outstream.write('looking up {}, category {}\n'.format(
                    num, physics_type))
            for ds in self.ds_from_id(num): 
                if physics_type: 
                    ds.physics_type = physics_type

                if ds.key in ds_dict: 
                    old = ds_dict[ds.key]
                    largest_ds = _filter_for_largest_tags(ds, old)
                    if len(largest_ds) == 1: 
                        ds_dict[ds.key] = largest_ds[0]
                    else: 
                        raise DatasetOverwriteError(
                            "tried to overwrite {}, {} with {}".format(
                                ds.key, ds.full_name, 
                                ds_dict[ds.key].full_name))
                else: 
                    ds_dict[ds.key] = ds
        return ds_dict

    def ds_from_id(self, ds_id, stream=None): 
        if stream and stream.startswith('physics'): 
            args = {'run':str(ds_id)}
        else: 
            args = {'dataset_number':str(ds_id)}

        qstr = '%'.join([self.origin, str(ds_id), stream or ''])
        match_sets = query.get_datasets(self.client,qstr, **args)

        if not match_sets: 
            raise DatasetMatchError('found nothing with {}'.format(qstr), 
                                    match_sets)
        
        match_sets = _filter_by_ldn(
            match_sets, stream, self.ntup_filter, self.p_tag)

        for match in match_sets: 
            ldn = _ldn(match)
            info = query.get_dataset_info(self.client, ldn)
            ds = meta.Dataset(ldn)
            self._write_ami_info(ds, info)
            if not ds.is_data: 
                self._write_mc_ami_info(ds, info)
                ds.physics_type = 'data'
            yield ds

    def update_ds(self, ds, overwrite=True): 
        if ds.is_data: 
            args = {'run':str(ds.id)}
        else: 
            args = {'dataset_number':str(ds.id)}

        match_sets = query.get_datasets(self.client,'%', **args)
        primary_matches = []
        secondary_matches = []
        for match in match_sets: 
            ldn = _ldn(match)
            if ldn.endswith(self.p_tag) and self.ntup_filter in ldn: 
                primary_matches.append(_ldn(match))
            if ldn.endswith(self.backup_ptag) and self.ntup_filter in ldn: 
                secondary_matches.append(_ldn(match))

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
        n_runs = len(runs)
        for run_n, run in enumerate(runs): 
            self.outstream.write(
                'looking for {} of {} {}...'.format(run_n, n_runs, run))
            try: 
                ds = next(iter(self.ds_from_id(run, stream)))
            except DatasetMatchError as err: 
                if err.matches: 
                    self.outstream.write('none in stream\n')
                    self.bugstream.write(str(err) + '\n')
                    continue
                else: 
                    self.outstream.write('nothing\n')
                    self.bugstream.write(str(err) + '\n')
                    continue
            self.outstream.write('found: {}\n'.format(ds.full_name))
            datasets[ds.key] = ds
        return datasets

    def _bug_report_line(self, line, ds, info): 
        diagnostic = 'for {} {}. In info: {}\n'.format(
            ds.key, ds.name, ', '.join(info.extra.keys()))

        responsible = ''
        argv=["SearchQuery"]
        argv.append(
            "-sql=select physicistResponsible from dataset where "
            "logicalDatasetName='{}'".format(info.info['logicalDatasetName']))
        argv.append('project=mc12_001')
        argv.append('processingStep=production')
        result = self.client.execute(argv)
        for row in result.rows(): 
            if 'physicistResponsible' in row: 
                assert not responsible, 'two responsible physicists found'
                responsible = row['physicistResponsible']

        tmp_str = '{} (email: {res}) {}' if responsible else '{} {}'
        return tmp_str.format(line, diagnostic, res=responsible)

    def _write_mc_ami_info(self, ds, info, overwrite=False):
        if not ds.filteff or overwrite:
            filteff_list = ['GenFiltEff_mean', 'approx_GenFiltEff']
            for name in filteff_list: 
                if name in info.extra: 
                    ds.filteff = float(info.extra[name])
                    break

        if not ds.filteff: 
            self.bugstream.write(
                self._bug_report_line("can't find filteff", ds, info))

        new_xsec = 0.0
        xsec_list = ['crossSection_mean', 'approx_crossSection']
        for name in xsec_list: 
            if name in info.extra: 
                # ami stores in nb
                new_xsec = float(info.extra[name])*1e6
                break

        if not new_xsec: 
            self.bugstream.write(
                self._bug_report_line("can't find xsection", ds, info))
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


class McStatsLookup(object): 
    """
    Tool to look up stats in mc datasets that exist in both atlfast and 
    fullsim. 
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


    def get_atlfast_fullsim(self, ds_id, stream=None): 
        args = {'dataset_number':str(ds_id)}

        match_sets = query.get_datasets(self.client,'%', **args)

        if not match_sets: 
            raise DatasetMatchError('found nothing with {}'.format(
                    args.items()), match_sets)

        match_sets = _filter_by_ldn(
            match_sets, stream, self.ntup_filter, self.p_tag)

        atlfast_ds = None
        fullsim_ds = None
        for m in match_sets: 
            ldn = _ldn(m)
            if self.atlfinder.search(_ldn(m)): 
                if atlfast_ds: 
                    raise DatasetMatchError('at least two atlfast', 
                                            [atlfast_ds,m])
                atlfast_ds = m
            else: 
                fullsim_ds = _largest_fullsim_filter(fullsim_ds, m)[0]

        atlfast_counts = _get_expected_counts(self.client, atlfast_ds)
        fullsim_counts = _get_expected_counts(self.client, fullsim_ds)
        atl = (_ldn(atlfast_ds), atlfast_counts) if atlfast_ds else None
        ful = (_ldn(fullsim_ds), fullsim_counts) if fullsim_ds else None
        return  atl, ful

# ===== filtering functions ======
# for when I don't know what ds to take...
def _largest_s_tag_filter(*datasets): 
    stag_finder = re.compile('_s([0-9]+)')
    stags = defaultdict(list)
    def get_stag(ds): 
        return int(stag_finder.search(ds.full_name).group(1))
    for ds in datasets: 
        stags[get_stag(ds)].append(ds)
    return sorted(stags.iteritems(), reverse=True)[0][1]

def _largest_fullsim_filter(*datasets): 
    fullsim_finder = re.compile('(_r([0-9]+))+')
    largest = []
    largest_number = 0
    for ds in filter(None, datasets): 
        ldn = ds.full_name
        fullsim_number = int(fullsim_finder.search(ldn).group(2))
        if fullsim_number > largest_number: 
            largest = [ds]
            largest_number = fullsim_number
        elif fullsim_number == largest_number: 
            largest.append(ds)
    return largest

def _filter_for_largest_tags(*datasets): 
    ds_list = datasets
    filters = [_largest_s_tag_filter, _largest_s_tag_filter]
    for filt in filters: 
        ds_list = filt(*ds_list)
    return ds_list
    

class DatasetMatchError(ValueError): 
    def __init__(self, info, matches=[]): 
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
        return problem

class DatasetOverwriteError(ValueError): 
    def __init__(self, what): 
        super(DatasetOverwriteError, self).__init__(what)

# ====================== convenience functions ======================

def lookup_ami_stats(meta_file): 
    """
    Small wrapper function on McStatsLookup
    """
    ami = McStatsLookup('p1328', 'mc12_8TeV')
    ds_cache = meta.DatasetCache(meta_file)
    sets = {}
    def filt(item): 
        key, ds = item
        if ds.is_data or ds.physics_type == 'signal': 
            return False
        return True
    ds_cache = filter(filt, ds_cache.iteritems())
    prog_meter = bullshit.FlatProgressMeter(len(ds_cache))
    for ds_n, (ds_key, ds) in enumerate(ds_cache): 
        prog_meter.update(ds_n)
        atlfast, fullsim = ami.get_atlfast_fullsim(ds.id)
        vals = {}
        if atlfast: 
            vals['atlfast_name'] = atlfast[0]
            vals['atlfast_stat'] = atlfast[1]
        if fullsim: 
            vals['fullsim_name'] = fullsim[0]
            vals['fullsim_stat'] = fullsim[1]
        if not vals: 
            raise ValueError("couldn't find {} in AMI".format(str(ds)))
        sets[ds.key] = vals

    return sets

def sort_ds_fullsim_atlfast(sets, fullsim_bias=1.2, get_keys=False): 
    atlfast = []
    fullsim = []
    for key, val in sets.iteritems(): 
        atl_ret = key if get_keys else val['atlfast_name'] 
        ful_ret = key if get_keys else val['fullsim_name']
        if 'atlfast_name' in val and 'fullsim_name' not in val: 
            atlfast.append(atl_ret)
        elif 'fullsim_name' in val and 'atlfast_name' not in val: 
            fullsim.append(ful_ret)
        elif not val: 
            print '{}, what the fuck??'.format(key)
        else: 
            atl_bias = float(val['atlfast_stat']) / val['fullsim_stat']
            if atl_bias > fullsim_bias: 
                atlfast.append(atl_ret)
            else: 
                fullsim.append(ful_ret)
    return fullsim, atlfast

    
