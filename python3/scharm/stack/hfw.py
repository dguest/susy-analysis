"""
Histograms from whiskey
"""

from ._hfw import _stacksusy
from os.path import basename, splitext

def stacksusy(input_file, region_list, flags=''): 
    """
    Runs the analysis on a distilled ntuple. The region_list contains 
    dictionaries, formatted as follows:
    
    (commented out because it's out of date)
    """

    # Required: 
    #     - name: name of region 
    #     - output_name: file to save region in
    #     - type: {CONTROL, SIGNAL, VALIDATION}

    # Optional: 
    #     - required_bits: bitmask 
    #     - veto_bits: bitmask
    #     - systematic: enum, default NONE
    #     - leading_jet_pt: cut on leading jet pt
    #     - met: cut on met
    #     - jet_tag_requirements: list of enums
    #     - hists: {HISTMILL, KINEMATIC_STAT, TAG_EFFICIENCY}
    #     - mc_mc_jet_reweight_file: path to textfile that does reweighting
    #     - tagger: {JFC, CNN}
    #     - jet_tag_assignment: {PT_ORDERED, TAG_ORDERED}

    # Flags: 
    #     v: verbose
    #     t: fill_truth
    #     d: is data

    if not region_list: 
        raise ValueError("must give a regions list")

    try: 
        _stacksusy(input_file, region_list, flags)
    except ValueError as e: 
        verb_error = (
            '{}, see stacksusy function help for details'.format(str(e)))
        raise ValueError(verb_error)

