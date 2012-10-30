from _hyperstack import _stacksusy
from os.path import basename, splitext

def stacksusy(input_file, mask_list, output_file='', flags=''): 
    """
    Runs the analysis on a distilled ntuple. 

    Flags: 
        v: verbose
    """
    if not output_file: 
        output_file = '{}_stack.h5'.format(splitext(input_file)[0])
    _stacksusy(input_file, mask_list, output_file, flags)
    return output_file
