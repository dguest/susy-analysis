"""
Histograms from whiskey
"""

from _hfw import _stacksusy
from os.path import basename, splitext

def stacksusy(input_file, region_list, flags=''): 
    """
    Runs the analysis on a distilled ntuple. The region_list contains 
    dictionaries, formatted as follows. 

    Required: 
        - name: name of region 
        - output_name: file to save region in
        - type: {CONTROL, SIGNAL, VALIDATION}

    Optional: 
        - required_bits: bitmask 
        - veto_bits: bitmask
        - systematic: enum, default NONE
        - leading_jet_pt: cut on leading jet pt
        - met: cut on met
        - jet_tag_requirements: list of enums
        - hists: {HISTMILL, KINEMATIC_STAT, TAG_EFFICIENCY}
        - mc_mc_jet_reweight_file: path to textfile that does reweighting
        - tagger: {JFC, CNN}

    Flags: 
        v: verbose
        t: fill_truth
        d: is data
    """
    if not region_list: 
        raise ValueError("must give a regions list")

    try: 
        _stacksusy(input_file, region_list, flags)
    except ValueError as e: 
        verb_error = (
            '{}, see stacksusy function help for details'.format(str(e)))
        raise ValueError(verb_error)

def hypersusy(input_file, mask_list, output_file='', flags='i', 
              limit_dict={}): 
    """
    Runs the hyperstacker on a distilled ntuple. 

    Flags: 
        v: verbose
        i: optimize b-tag on leading (isr) jet (default true)
        m: cut on mttop
        b: disable c-tagging (Barcelona style optimization)
    """
    mask_list = list(mask_list)
    if not output_file: 
        output_file = '{}_hyper.h5'.format(splitext(input_file)[0])
    _hypersusy(input_file, mask_list, output_file, flags, limit_dict)
    return output_file
    
def cutflow(input_file, mask, flags=''): 
    """
    runs very quick cutflow based on bits

    Flags: 
        v: verbose
    """
    
    return _cutflow(input_file, [mask], '', flags)
