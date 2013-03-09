"""
Here be bitdefs. 

 TODO: this should be replaced with something more automated
       probably best to write the bits to a tree when running the 
       cutflow. 
"""
bits = [
    ("grl",                  1 <<  0),
    ("trigger",              1 <<  1),
    ("lar_error",            1 <<  2),
    ("core",                 1 <<  3),
    ("jet_clean",            1 <<  4),
    ("vxp_gt_4trk",          1 <<  5),
    ("leading_jet",          1 <<  6),
    ("met",                  1 <<  7),
    ("n_jet",                1 <<  8),
    ("dphi_jetmet_min",      1 <<  9),
    ("lepton_veto",          1 << 10),
    ("ctag_mainz",           1 << 11),

    ('mttop'     ,           1 << 21), 
    ('j2_anti_b' ,           1 << 22), 
    ('j2_anti_u' ,           1 << 23), 
    ('j3_anti_b' ,           1 << 24), 
    ('j3_anti_u' ,           1 << 25), 
    ]

_bdict = dict(bits)

composite_bits = [ 
    ('ctag_opt', 
     _bdict['j2_anti_b'] | _bdict['j2_anti_u'] | 
     _bdict['j3_anti_b'] | _bdict['j3_anti_u'] ), 
    ('kinematic', 
     _bdict['leading_jet'] | _bdict['met']), 
    ('preselection', 
     _bdict['grl'] | _bdict['trigger'] | _bdict['lar_error'] | 
     _bdict['core'] | #this was set wrong in the distiller
     _bdict['jet_clean'] | _bdict['vxp_gt_4trk'] | 
     _bdict['n_jet'] | _bdict['dphi_jetmet_min'] )
    ]
     
_compdict = dict(composite_bits)

pretag_bits = (
    'pretag', _compdict['kinematic'] | _compdict['preselection'] | 
    _bdict['lepton_veto'])
    
final_dict = {
    'pretag':pretag_bits, 
    }
