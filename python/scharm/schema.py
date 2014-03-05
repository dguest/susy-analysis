import re, warnings

_data_stream_schema = { 
    'm':'Muon', 
    'e':'Egamma', 
    'j':'JetTauEtmiss'
    }

stream_schema = { 
    'm': 'muon', 
    'e': 'electron', 
    'j': 'jet', 
    'a': 'atlfast', 
    's': 'fullsim',
    'u': 'unknown'
}

def get_prechar(name):
    """
    map dataset names onto the key charicter
    """
    for char, stream in _data_stream_schema.iteritems(): 
        # data streams have physics_<stream> in the name somewhere
        if '_' + stream in name: 
            return char
    atlfinder = re.compile('(_a([0-9]+))+')
    if atlfinder.search(name): 
        return 'a'
    fullsimfinder = re.compile('(_s([0-9]+))+')
    if fullsimfinder.search(name): 
        return 's'
    warnings.warn(
        "{} lacks atlfast / fullsim info, assume 'a'".format(
            name), stacklevel=2)
    return 'a'
