from _hyperstack import _stacksusy
# from _hyperstack import _hypersusy, _cutflow
from os.path import basename, splitext

GeV = 1e3

_config_opts = dict(
    leading_jet = 260*GeV, 
    met         = 180*GeV, 
    j2_anti_b   = -0.2, 
    j2_anti_u   =  2.5, 
    j3_anti_b   = -0.2, 
    j3_anti_u   = -0.5, 
    btag_config = 'LOOSE_TIGHT', 
)
def stacksusy(input_file, region_list, output_file='', flags='', 
              config_opts=_config_opts): 
    """
    Runs the analysis on a distilled ntuple. 

    Flags: 
        v: verbose
        t: fill_truth
        d: is data
    """
    if not region_list: 
        raise ValueError("must give a regions list")
    if isinstance(region_list[0], tuple): 
        newlist = []
        for tup in region_list: 
            try: 
                name, mask, antimask = tup
            except ValueError: 
                name, mask = tup
                antimask = 0
            newdict = {
                'name':name, 
                'required_bits':long(mask), 
                'veto_bits':long(antimask), 
                'jet_tag_requirements':['NOTAG','LOOSE','TIGHT'], 
                }
            newlist.append(newdict)
        region_list = newlist

    if not output_file: 
        output_file = '{}_stack.h5'.format(splitext(input_file)[0])
    _stacksusy(input_file, region_list, output_file, flags, config_opts)
    return output_file

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
