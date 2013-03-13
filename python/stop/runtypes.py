class DsRange(object): 
    def __init__(self, *range_tuples): 
        self.range_tuples = range_tuples
    def __add__(self, other): 
        all_tuples = list(self.range_tuples)  
        if isinstance(other,list): 
            for v in other: 
                all_tuples.append( (v,v))
        else: 
            all_tuples += list(other.range_tuples)

        return DsRange(*all_tuples)
    def __radd__(self, other): 
        return self.__add__(other)
    def __iter__(self): 
        for rt in self.range_tuples: 
            low, high = rt
            for v in xrange(low, high+1): 
                yield v

marks_types = { 
    'ttbar': [105861], 
    'Wjets': DsRange( (167740,167748), (167761,167796)), 
    'Zjets': DsRange( (167749,167760), (167797,167844)), 
    't': DsRange( (108343,108346), (117360,117362)), 
    'diboson': [143065] + DsRange( (157814,157819)), 
    'signal': DsRange( (157851, 157908) , (176270, 176320) )
    }

def is_wjet(ds): 
    if 117680 <= ds.id <= 117705: 
        ds.physics_type = 'Wjets'
        return True
    return False

def is_zjet(ds): 
    if 117650 <= ds.id <= 117675: 
        ds.physics_type = 'Zll'
        return True
    return False

def is_znujet(ds): 
    if 156803 <= ds.id <= 156828: 
        ds.physics_type = 'Znunujets'
        return True
    return False

def is_ttbar(ds): 
    if ds.id == 105861: 
        ds.physics_type = 'ttbar'
        return True
    return False

def is_diboson(ds): 
    if 105985 <= ds.id <= 105987: 
        ds.physics_type = 'diboson'
        return True
    return False

def is_data(ds): 
    if ds.is_data: 
        ds.physics_type = 'data'
        return True
    return False

def is_signal(ds): 
    import re
    is_old_signal = (157851 <= ds.id <= 157908)
    is_met_filter = (176270 <= ds.id <= 176320)
    if is_old_signal:
        finder = re.compile('directCC_([0-9]+)_([0-9]+)')
        matches = finder.search(ds.name)
        stop_mass = int(matches.group(1))
        lsp_mass = int(matches.group(2))
        ds.physics_type = 'signal'
        ds.generator_info = dict(
            stop_mass_gev = stop_mass, 
            lsp_mass_gev = lsp_mass
            )
        return True
    elif is_met_filter: 
        finder = re.compile(
            'stopToCharmLSP_t([0-9]+)_n([0-9]+)_MET([0-9]+)')
        matches = finder.search(ds.name)
        stop_mass = int(matches.group(1))
        lsp_mass = int(matches.group(2))
        met_filter = float(matches.group(3))
        ds.physics_type = 'signal'
        ds.generator_info = dict(
            stop_mass_gev = stop_mass, 
            lsp_mass_gev = lsp_mass, 
            met_filter_gev = met_filter
            )
        return True
    return False

_all_typers = [ 
    is_data, 
    is_wjet, 
    is_zjet, 
    is_znujet, 
    is_ttbar, 
    is_diboson, 
    is_signal, 
    ]

def set_dataset_physics_type(ds): 
    """
    See if the DS is in the types we want. 
    Changes the ds physics_type as a side effect. 
    """
    nset = 0
    for set_type in _all_typers: 
        is_set = set_type(ds)
        if is_set: 
            nset += 1
    if nset == 0: 
        return False
    elif nset > 1: 
        raise ValueError("dataset {} somehow fell into {} types".format(
                ds, nset))
    return True
        
