import cutflow
import cPickle
import os
import glob
import collections
import re
import warnings
from stop.meta import Dataset

class NormedCutflow(object): 
    """
    Uses a lookup file to normalize cutflows. 
    """
    
    def __init__(self, mainz_file, susy_file, 
                 raw_counts_cache='raw_counts', 
                 output_ntuples_dir='', 
                 p_tag=''): 
        self._mainz_file_name = mainz_file
        self._susy_file_name = susy_file, 
        self._cache_name = raw_counts_cache
        self._output_ntuples_dir = output_ntuples_dir
        self.p_tag = p_tag
        
        self.n_fail_lookup = 0

        with open(mainz_file) as txt: 
            mainz_dict = {n: d for n,d in self._mainz_itr(txt)}
        with open(susy_file) as txt: 
            susy_dict = {n: d for n,d in self._susy_itr(txt)}

        self._ds_dict = susy_dict
        self._ds_dict.update(mainz_dict)

    def get_dataset(self, ds_key): 
        """
        Try to get cross section for ds_key. Search order: 
        1) exact mainz match 
        2) exact susy match 
        3) ds_key within susy key 
        """
        try: 
            return self._ds_dict[ds_key]
        except KeyError: 
            self.n_fail_lookup += 1

        for key in self._ds_dict: 
            if ds_key in key: 
                return self._ds_dict[key]
    
        raise LookupError('no {} found in {} or {}'.format(
                ds_key, self._mainz_file_name, self._susy_file_name))

    
    def _mainz_itr(self,txt_file): 
        """
        iterator for xsec file in mainz format
        """
        for line in txt_file: 
            line = line.split('#')[0].strip()
            if not line: 
                continue
            spl = line.split()
            ds = Dataset()
            ds.name = spl[0]
            ds.total_xsec = float(spl[2])
            ds.filteff = float(spl[3])
            ds.kfactor = 1
            ds.n_raw_entries = int(spl[4])
            yield ds.name, ds

    def _susy_itr(self,txt_file): 
        id_line = ''
        for line in txt_file: 

            # first line of the susy textfile is some kind of 
            # field description         
            if not id_line: 
                id_line = line
                continue

            line = line.split('#')[0].strip()
            if not line: 
                continue
            spl = line.split()
            ds = Dataset()
            ds.id = int(spl[0])
            ds.name = spl[1]
            ds.total_xsec = float(spl[2])
            ds.kfactor = float(spl[3])
            ds.filteff = float(spl[4])
            
            yield ds.name, ds

    def add_ds_lookup(self, ds_key, lookup_location): 
        """
        Builds a mapping between the ds_key and the files that it maps to.
        Tries several things in order, appends if these are true: 
        1) exact file match
        2) find ds_key within sub-directories of lookup_location

        returns the mapping. 
        """
        
        matches = []

        if os.path.isfile(lookup_location): 
            matches = [lookup_location]
        else: 
            matches = self._ds_match(ds_key, lookup_location)
        
        self.get_dataset(ds_key).d3pds += matches
        return self.get_dataset(ds_key).d3pds

    def _ds_match(self, ds_key, location): 
        """
        does the globbing for files matching ds_key in location 
        """
        
        globstr = '{location}/*{ds_key}*{tag}*/*.root*'.format(
            location=location, ds_key=ds_key, tag=self.p_tag)
            
        match_files = glob.glob(globstr)
        
        if not match_files: 
            warnings.warn("can't find key {} in {} with {}".format(
                    ds_key, location, globstr))

        return match_files


    def get_normed_counts(self, ds_key, lumi=1000.0, flags='v'): 
        """
        Gets lumi-normalized cut counts for dataset identified by ds_key. 
        Looks finds the root files in files_from_key, normalization 
        is found in norm_file. 
        """
        
        cached_info = {}
        full_cache_name = os.path.join(self._cache_name, ds_key + '.pkl')
        if not os.path.isdir(self._cache_name): 
            os.makedirs(self._cache_name)
        if os.path.isfile(full_cache_name): 
            with open(full_cache_name,'rb') as pkl: 
                cached_info = cPickle.load(pkl)

        if self._output_ntuples_dir: 
            if not os.path.isdir(self._output_ntuples_dir): 
                os.makedirs(self._output_ntuples_dir)
            output_ntuple = os.path.join(self._output_ntuples_dir,ds_key)
            output_ntuple += '.root'
        else: 
            output_ntuple = ''

        rebuild_conditions = [
            ds_key not in cached_info, 
            output_ntuple and not os.path.isfile(output_ntuple), 
            ]

        need_rebuild = any(rebuild_conditions)

        run_number = self.get_dataset(ds_key).id
        if not run_number: 
            run_number = 180614 # FIXME: obviously not right...

        if need_rebuild: 
            input_files = self.get_dataset(ds_key).d3pds
            if 'v' in flags: 
                print '--- distilling {} ---'.format(ds_key)
            cut_counts = cutflow.cutflow(input_files, run_number, 
                                         flags=flags, 
                                         output_ntuple=output_ntuple)
            
            cached_info[ds_key] = cut_counts

            with open(full_cache_name,'wb') as pkl: 
                cPickle.dump(cached_info, pkl)

        cut_counts = cached_info[ds_key]
        
        # we assume the maximum cut is the number of events
        n_events = max(count for name, count in cut_counts)
        dataset = self.get_dataset(ds_key)

        if dataset.n_raw_entries and n_events != dataset.n_raw_entries: 
            frac_in_file = float(n_events) / float(dataset.n_raw_entries)
            warnings.warn(
                "number of events in {} ({}) "
                "doesn't match expected {} ({:.1%} diff)".format(
                    ds_key, n_events, dataset.n_raw_entries, 
                    frac_in_file - 1), 
                stacklevel=2)
        
        dataset.n_raw_entries = n_events
        
        int_xsec_per_evt = dataset.corrected_xsec() / float(n_events) * lumi

        self._save_xsec_info(ds_key, int_xsec_per_evt)
        dataset = self.get_dataset(ds_key)
        self._save_dataset_meta(dataset)

        normed_counts = [(n, c * int_xsec_per_evt) for n,c in cut_counts]

        return normed_counts

    def _save_xsec_info(self,ds_key, int_xsec_per_evt): 
        if self._output_ntuples_dir: 
            per_evt_pkl = 'x_sec_per_event.pkl'
            x_sec_pkl = os.path.join(self._output_ntuples_dir, per_evt_pkl) 
            if os.path.isfile(x_sec_pkl): 
                with open(x_sec_pkl) as pkl: 
                    per_evt_dic = cPickle.load(pkl)
                if not isinstance(per_evt_dic, dict): 
                    warnings.warn(
                        '{} is corrupted, removing'.format(x_sec_pkl))
                    os.remove(x_sec_pkl)
                    per_evt_dic = {}
            else:
                per_evt_dic = {}
            per_evt_dic[ds_key] = int_xsec_per_evt
            with open(x_sec_pkl,'w') as pkl: 
                cPickle.dump(per_evt_dic, pkl)

    def _save_dataset_meta(self,dataset): 
        if self._output_ntuples_dir: 
            meta_pkl = 'meta.pkl'
            meta_path = os.path.join(self._output_ntuples_dir, meta_pkl)
            if os.path.isfile(meta_path): 
                with open(meta_path) as pkl: 
                    meta_dict = cPickle.load(pkl)
                if not isinstance(meta_dict, dict): 
                    warnings.warn(
                        '{} is corrupted, removing'.format(meta_dict))
                    os.remove(meta_dict)
                    meta_dict = {}
            else: 
                meta_dict = {}
            
            meta_dict[dataset.name] = dataset
            with open(meta_path,'w') as pkl: 
                cPickle.dump(meta_dict, pkl)

