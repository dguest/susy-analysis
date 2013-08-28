"""
Code to parse meta files and find overlapping samples. 
"""
import re, collections 

def clear_overlap_info(ds_cache): 
    for key, ds in ds_cache.iteritems(): 
        ds.overlap = {}

class OverlapMetaAdder(object): 
    def __init__(self, sherpa_boson_pt_gev=70): 
        self._sherpa_boson_pt_gev = sherpa_boson_pt_gev

    def add_overlap_info(self, ds_cache): 
        signals_by_qn = collections.defaultdict(list)
        for key, ds in ds_cache.iteritems(): 
            if _needs_sherpa_boson_overlap(ds.full_name): 
                ds.overlap['sherpa_boson'] = self._sherpa_boson_pt_gev
            mquark, mlsp, met_cut = _get_mq_mn_met(ds.full_name)
            if mquark: 
                signals_by_qn[mquark, mlsp].append((met_cut, key))

        for (mquark, mlsp), cut_key_list in signals_by_qn.iteritems(): 
            for key, met_cut in _get_met_cuts_by_key(cut_key_list): 
                ds_cache[key].overlap['truth_met'] = met_cut

def _get_met_cuts_by_key(cut_key_list): 
    met_list = sorted(cut_key_list)
    cut_by_key = []
    for cut_num in xrange(1,len(met_list)): 
        key = met_list[cut_num - 1][1]
        met = met_list[cut_num][0]
        yield key, met

def _get_mq_mn_met(full_ds_name): 
    met_filt_rex = re.compile('LSP_t([0-9]+)_n([0-9]+)_MET([0-9]+)')
    inclusive_rex = re.compile('directCC_([0-9]+)_([0-9]+)')
    met_match = met_filt_rex.search(full_ds_name)
    inclusive_match = inclusive_rex.search(full_ds_name)
    if met_match and inclusive_match: 
        raise ValueError('{} matches too many regexes'.format(full_ds_name))
    if met_match: 
        return tuple(int(n) for n in met_match.groups())
    elif inclusive_match: 
        return tuple(int(n) for n in  inclusive_match.group(1,2)) + (None,)
    else: 
        return None, None, None

def _needs_sherpa_boson_overlap(sample): 
    """
    somewhat hackish in that it relies on hardcoded upper pt limit for 
    the inclusive sample
    """
    overlaping_sherpa_finder = re.compile('MassiveCBPt0_')
    sherpa_pt_range_finder = re.compile('Pt[0-9]+_[0-9]+')
    if overlaping_sherpa_finder.search(sample): 
        if sherpa_pt_range_finder.search(sample): 
            raise IOError( 
                "can't determine if {} needs pt overlap removal".format(
                    sample))
        return True
    return False
