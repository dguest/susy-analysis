
other_bits = [
    'n_events','GRL','EF_xe80_tclcw_loose','lar_error','core','jet_clean',
    'vxp_good', 'leading_jet_120', 'met_120', 'n_jet_geq_3', 
    'dphi_jetmet_sum', 'lepton_veto', 'ctag_mainz', 
    ]


bits = [
    ("all", 0), 
    ("grl",  1 << 2), 
    ("trigger",  1 << 3), 
    ("lar_error",  1 << 4), 
    ("core",  1 << 5), 
    ("jet_clean",  1 << 6), 
    ("vxp_gt_4trk",  1 << 7), 
    ("leading_jet",  1 << 8), 
    ("met",  1 << 9), 
    ("n_jet",  1 << 10), 
    # ("dphi_jetmet_sum",  1 << 12), 
    ("dphi_jetmet_min",  1 << 11), 
    ("lepton_veto",  1 << 13), 
    ("ctag_mainz",  1 << 14), 
    ]

better_bits = [(new, val) for new, (old, val) in zip(other_bits,bits)]
