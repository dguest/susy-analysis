"""
Here be bitdefs. 

 TODO: is it a good idea to generate the bits with each import? Not sure.
"""
import yaml, sys, stop
from os.path import dirname, join

def get_bits(bits_file=None): 
    if not bits_file: 
        bits_yaml = join(dirname(stop.__file__),'bits.yml')
    with open(bits_yaml) as bt: 
        bt_dict = yaml.load(bt)
    bt_list = [(n, 1 << s) for n,s in bt_dict.iteritems()]
    def lkey(tup): 
        return tup[1]
    return sorted(bt_list, key=lkey)

bits = get_bits()

_bdict = dict(bits)

composite_bits = [ 
    ('ctag_opt', 
     _bdict['j2_anti_b_opt'] | _bdict['j2_anti_u_opt'] | 
     _bdict['j3_anti_b_opt'] | _bdict['j3_anti_u_opt'] ), 
    ('kinematic', 
     _bdict['leading_jet'] | _bdict['met']), 
    ('preselection', 
     _bdict['grl'] | _bdict['trigger'] | _bdict['lar_error'] | 
     _bdict['core'] | #this was set wrong in the distiller
     _bdict['jet_clean'] | _bdict['vxp_gt_4trk'] | 
     _bdict['dphi_jetmet_min'] ), 
    ('lepton_veto', _bdict['electron_veto'] | _bdict['muon_veto'])
    ]
     
_compdict = dict(composite_bits)

pretag_bits = (
    'pretag', _compdict['kinematic'] | _compdict['preselection'] | 
    _compdict['lepton_veto'])
    
final_dict = {
    'pretag':pretag_bits, 
    }

region_event_filter_bits = { 
    'no_extra_jets': 1 << 0, 
    'throw_for_fun': 1 << 1, 
    }
