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

class Region(dict): 
    """
    Python wrapper for info to define a signal region. 
    """
    def __init__(self, init_tuple=None, tag_requirements=[]): 
        self.jet_tag_requirements = tag_requirements
        if not init_tuple: 
            return None
        try: 
            name, required, veto = init_tuple
        except ValueError: 
            name, required = init_tuple
            veto = 0
        self.name = name
        self.required_bits = required
        self.veto_bits = veto
        self.output_name = 'stupid.h5'
    def get_dict(self): 
        """
        Produces the configuration info needed for _stacksusy
        """
        out_dict = {}
        for name, value in self.__dict__.items(): 
            if isinstance(value, int): 
                out_dict[name] = long(value)
            else: 
                out_dict[name] = value
        return out_dict

def stacksusy(input_file, region_list, flags=''): 
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
            newlist.append(Region(tup, ['NOTAG']).get_dict())
        region_list = newlist

    _stacksusy(input_file, region_list, flags)

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
