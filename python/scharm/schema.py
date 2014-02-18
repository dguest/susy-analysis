import re

_stream_schema = { 
    'm':'Muon', 
    'e':'Egamma', 
    'j':'JetTauEtmiss'
    }

dir_schema = { 
    'm': 'muon', 
    'e': 'electron', 
    'j': 'jet', 
    'a': 'atlfast', 
    's': 'fullsim'
}

def get_prechar(name):
    """
    map dataset names onto the key charicter
    """
    for char, stream in _stream_schema.iteritems(): 
        if '_' + stream in name: 
            return char
    atlfinder = re.compile('(_a([0-9]+))+')
    if atlfinder.search(name): 
        return 'a'
    fullsimfinder = re.compile('(_s([0-9]+))+')
    if fullsimfinder.search(name): 
        return 's'
    raise ValueError("can't classify {}".format(name))
