"""
Code to parse meta files and find overlapping samples. 
"""
import re

def clear_overlap_info(ds_cache): 
    for key, ds in ds_cache.iteritems(): 
        ds.overlap = {}

class OverlapMetaAdder(object): 
    def __init__(self, sherpa_boson_pt_gev=70): 
        self._sherpa_boson_pt_gev = sherpa_boson_pt_gev

    def add_overlap_info(self, ds_cache): 
        for key, ds in ds_cache.iteritems(): 
            if _needs_sherpa_boson_overlap(ds.full_name): 
                ds.overlap['sherpa_boson'] = self._sherpa_boson_pt_gev

def _needs_sherpa_boson_overlap(sample): 
    overlaping_sherpa_finder = re.compile('MassiveCBPt0_')
    sherpa_pt_range_finder = re.compile('Pt[0-9]+_[0-9]+')
    if overlaping_sherpa_finder.search(sample): 
        if sherpa_pt_range_finder.search(sample): 
            raise IOError( 
                "can't determine if {} needs pt overlap removal".format(
                    sample))
        return True
    return False
