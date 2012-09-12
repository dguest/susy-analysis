import _cutflow 
import warnings
import cPickle
import os

run_number = 180614

def cutflow(input_file, run_number, flags): 
    """
    Python-level interface for the compiled cutflow routine. 
    Flags: 
        v: verbose
        d: is data
        s: is signal
    """
    return _cutflow._cutflow(input_file, run_number, flags)


class NormedCutflow(object): 
    """
    Uses a lookup file to normalize cutflows. 
    """
    def __init__(self, norm_file, raw_counts_cache = 'raw_counts.pkl'): 
        self._norm_file_name = norm_file
        self._cache_name = raw_counts_cache
        with open(norm_file) as txt: 
            self._norm_dict = {n: (x,e) for n,x,e in self._tup_itr(txt)}

    def _tup_itr(self,txt_file): 
        for line in txt_file: 
            line = line.split('#')[0].strip()
            if not line: 
                continue
            spl = line.split()
            short_name = spl[0]
            xsec = float(spl[2])
            evts = int(spl[4])
            yield short_name, xsec, evts

    def get_normed_counts(self, ds_key): 
        
        if not ds_key in self._norm_dict: 
            raise LookupError('no {} found in {}'.format(
                    file_name, self._norm_file_name))

        cached_info = {}
        if os.path.isfile(self._cache_name): 
            with open(self._cache_name,'rb') as pkl: 
                cached_info = cPickle.load(pkl)

        if not ds_key in cached_info: 
            input_file = ds_key + '.root'
            cut_counts = cutflow(input_file, run_number, 'vs')
            
            cached_info[ds_key] = cut_counts

            with open(self._cache_name,'wb') as pkl: 
                cPickle.dump(cached_info, pkl)

        cut_counts = cached_info[ds_key]
        
        # we assume the maximum cut is the number of events
        n_events = max(count for name, count in cut_counts)
        cross_section, xsec_events = self._norm_dict[ds_key]
        frac_in_file = float(n_events) / float(xsec_events)
        if n_events != xsec_events: 
            warnings.warn(
                "number of events in {} ({}) "
                "doesn't match expected {} ({:.1%} diff)".format(
                    ds_key, n_events, xsec_events, frac_in_file - 1), 
                stacklevel=2)
        
        robust_xsec =  cross_section / frac_in_file

        normed_counts = {n: c * robust_xsec for n,c in cut_counts}

        return normed_counts
        
                
                              

def print_raw_cutflow(input_file): 
    """
    prints the raw cutflow, descending in n passing cut
    """
    cut_counts = cutflow(input_file, run_number, 'vs')
    sorted_counts = sorted(cut_counts, key=lambda cut: cut[1], reverse=True)
    for name, count in sorted_counts: 
        print '{:>20}: {}'.format(name,count)
