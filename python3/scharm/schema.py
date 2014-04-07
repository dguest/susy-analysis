"""
misc file naming / path interpenetration stuff
"""

from os.path import join, isdir
from os import listdir
import re, warnings

dir_key_order = ['stream', 'systematic', 'replacement']

def distiller_settings_from_dir(path):
    return dict(zip(dir_key_order, path.split('/')[-3:]))

def dir_from_distiller_settings(settings, base=''):
    return join(base, *(settings[key].lower() for key in dir_key_order))

def test(test_path='this/is/a/path'):
    settings = distiller_settings_from_dir(test_path)
    repath = dir_from_distiller_settings(settings, 'this')
    if test_path != repath:
        raise Exception("path schema not working: {} => {}".format(
                test_path, repath))
    return repath

def get_all_systematics(root_path):
    """get all the systematics as visible from the directory structure"""
    all_syst = set()
    for stream_dir in listdir(root_path):
        syst_dirs = listdir(stream_dir)
        if not 'none' in syst_dirs:
            raise ValueError('no nominal syst found, probably an error')
        all_syst |= syst_dirs
    return all_syst

# _______________________________________________________________________
# data type charicter codes

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
    for char, stream in _data_stream_schema.items():
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
