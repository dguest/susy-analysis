from os.path import dirname

_shift_sf = list('BCUT') + ['EL','MU']
scale_factor_systematics = ['NONE'] 
scale_factor_systematics += [
    part + shift for part in _shift_sf for shift in ['UP','DOWN']
    ]

def get_systematics(self, ntuple): 
    syst = None
    tld = dirname(ntuple).split('/')[-1]
    syst = 'NONE' if tld == 'baseline' else tld.upper()
    if syst is None: 
        raise ValueError("couldn't find syst for {}".format(ntuple))
    if syst == 'NONE': 
        return scale_factor_systematics
    else: 
        return [syst]

