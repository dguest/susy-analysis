from _hyperstack import _stacksusy, _hypersusy, _cutflow
from os.path import basename, splitext

def stacksusy(input_file, mask_list, output_file='', flags=''): 
    """
    Runs the analysis on a distilled ntuple. 

    Flags: 
        v: verbose
        t: fill_truth
    """
    mask_list = list(mask_list)
    if not output_file: 
        output_file = '{}_stack.h5'.format(splitext(input_file)[0])
    _stacksusy(input_file, mask_list, output_file, flags, {})
    return output_file

def hypersusy(input_file, mask_list, output_file='', flags='', 
              limit_dict={}): 
    """
    Runs the hyperstacker on a distilled ntuple. 

    Flags: 
        v: verbose
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
