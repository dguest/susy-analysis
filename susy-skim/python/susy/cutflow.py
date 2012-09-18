import _cutflow 
import warnings
import cPickle
import os
import glob
import collections
import re

run_number = 180614

def cutflow(input_files, run_number, flags): 
    """
    Returns a list of pairs: (cut_name, n_passing)

    Python-level interface for the compiled cutflow routine. 
    Flags: 
        v: verbose
        d: is data
        s: is signal
        p: use low pt jets
    """
    if isinstance(input_files, str): 
        input_files = [input_files]

    if not input_files: 
        raise IOError("can't run cutflow, input files don't exist")

    try: 
        cut_out = _cutflow._cutflow(input_files, run_number, flags)
    except RuntimeError as er: 
        raise RuntimeError('{} in {}'.format(str(er), input_files))

    return cut_out


class NormedCutflow(object): 
    """
    Uses a lookup file to normalize cutflows. 
    """
    
    files_from_key = collections.defaultdict(list)

    def __init__(self, norm_file, raw_counts_cache='raw_counts.pkl', 
                 file_format='official'): 
        self._norm_file_name = norm_file
        self._cache_name = raw_counts_cache

        allowed_formats = {
            'mainz':self._mainz_itr,
            'official':self._susy_itr, 
            }

        if not file_format in allowed_formats: 
            raise ValueError("can't parse file formatted in {}, "
                             "choose from {}".format(file_format, 
                                                     allowed_formats.keys()))

        file_itr = allowed_formats[file_format]

        with open(norm_file) as txt: 
            self._norm_dict = {n: (x,e) for n,x,e in file_itr(txt)}


    def add_ds_lookup(self, ds_key, lookup_location): 
        """
        Builds a mapping between the ds_key and the files that it maps to.
        Tries several things in order, appends if these are true: 
        1) exact file match
        2) fuzzy match according to Mainz naming convention
        3) top-level directory includes ds key
        4) subdirectory of lookup_location contains ds_key

        returns the mapping. 
        """
        
        matches = []
        leaf_dir = os.path.split(lookup_location)[0].split('/')[-1]
        
        fuzzy_mainz = self._mainz_match(ds_key, lookup_location)

        if os.path.isfile(lookup_location): 
            matches = [lookup_location]
        elif fuzzy_mainz: 
            matches = fuzzy_mainz
        elif ds_key in leaf_dir: 
            matches += glob.glob('{loc}/*.root*')
        elif os.path.isdir(lookup_location): 
            globstr = '{loc}/*{key}*/*.root*'.format(
                loc=lookup_location.rstrip('/'), key=ds_key)
            match_files = glob.glob(globstr)
            for f in match_files: 
                if not 'root.tgz' in f: 
                    matches.append(f)
        else: 
            warnings.warn("can't find {} in {}".format(
                    ds_key, lookup_location))
        
        self.files_from_key[ds_key] += matches
        return self.files_from_key[ds_key]

    def _mainz_itr(self,txt_file): 
        for line in txt_file: 
            line = line.split('#')[0].strip()
            if not line: 
                continue
            spl = line.split()
            short_name = spl[0]
            xsec = float(spl[2])
            evts = int(spl[4])
            yield short_name, xsec, evts

    def _mainz_match(self, ds_key, location): 
        """
        fuzzy match function for mainz signal files
        """
        spl = ds_key.split('_')
        if len(spl) != 2: 
            return None
        ds_dir, file_name = spl
        globstr = '{loc}/*{subdir}*/*{fn}*.root'.format(
            loc=location.rstrip('/'), 
            subdir=ds_dir.replace('-','?'), 
            fn=file_name)
        match_files = glob.glob(globstr)

        match_re = re.compile('(?<=[-_.]v)[0-9]+(?=[-_.])')

        versions = collections.defaultdict(list)
        for f in match_files: 
            version_strings = match_re.findall(f)
            if len(version_strings) > 1: 
                warnings.warn(
                    'version finder acting up: {} found in {}'.format(
                        version_strings, f))
            if version_strings:
                versions[int(version_strings[0])].append(f)

        return match_files

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
            short_name = spl[1]
            xsec = float(spl[2])
            k_factor = float(spl[3])
            
            yield short_name, xsec * k_factor, None

    def get_normed_counts(self, ds_key, lumi=4700.0, flags='v'): 
        """
        Gets lumi-normalized cut counts for dataset identified by ds_key. 
        Looks finds the root files in files_from_key, normalization 
        is found in norm_file. 
        """
        
        if not ds_key in self._norm_dict: 
            raise LookupError('no {} found in {}'.format(
                    ds_key, self._norm_file_name))

        cached_info = {}
        if os.path.isfile(self._cache_name): 
            with open(self._cache_name,'rb') as pkl: 
                cached_info = cPickle.load(pkl)

        if not ds_key in cached_info: 
            input_files = self.files_from_key[ds_key]
            cut_counts = cutflow(input_files, run_number, flags=flags)
            
            cached_info[ds_key] = cut_counts

            with open(self._cache_name,'wb') as pkl: 
                cPickle.dump(cached_info, pkl)

        cut_counts = cached_info[ds_key]
        
        # we assume the maximum cut is the number of events
        n_events = max(count for name, count in cut_counts)
        cross_section, xsec_events = self._norm_dict[ds_key]
        try: 
            frac_in_file = float(n_events) / float(xsec_events)
        except TypeError: 
            frac_in_file = 0
            xsec_events = 0
        if xsec_events and n_events != xsec_events: 
            warnings.warn(
                "number of events in {} ({}) "
                "doesn't match expected {} ({:.1%} diff)".format(
                    ds_key, n_events, xsec_events, frac_in_file - 1), 
                stacklevel=2)
        
        int_xsec_per_evt =  cross_section / float(n_events) * lumi

        normed_counts = {n: c * int_xsec_per_evt for n,c in cut_counts}

        return normed_counts
        
                
                              

def print_raw_cutflow(input_file): 
    """
    prints the raw cutflow, descending in n passing cut
    """
    cut_counts = cutflow(input_file, run_number, 'vs')
    sorted_counts = sorted(cut_counts, key=lambda cut: cut[1], reverse=True)
    for name, count in sorted_counts: 
        print '{:>20}: {}'.format(name,count)