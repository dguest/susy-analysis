from os.path import dirname

_shift_sf = list('BCUT') + ['EL','MU']
scale_factor_systematics = ['NONE'] + [
    part + shift for part in _shift_sf for shift in ['UP','DOWN']
    ]

def get_systematics(ntuple): 
    tld = dirname(ntuple).split('/')[-1]
    syst = tld.upper()
    if syst == 'NONE': 
        return scale_factor_systematics
    else: 
        return [syst]

