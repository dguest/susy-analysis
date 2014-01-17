from os.path import dirname

_shift_sf = list('BCUT') + ['EL','MU','MET']
scale_factor_systematics = ['NONE', 'METRES'] 
scale_factor_systematics += [
    part + shift for part in _shift_sf for shift in ['UP','DOWN']
    ]

def get_systematics(ntuple): 
    tld = dirname(ntuple).split('/')[-1]
    syst = 'NONE' if tld == 'baseline' else tld.upper()
    if syst == 'NONE': 
        return scale_factor_systematics
    else: 
        return [syst]

