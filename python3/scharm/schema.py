"""
misc file naming / path interpenetration stuff
"""

from os.path import join, isdir
from os import listdir
import re, warnings

# extension for sum(wt^2) plots
wt2_ext = 'Wt2'
# extension for systematic error^2 plots
sys2_ext = 'Syst2'

nominal_syst = 'none'

# __________________________________________________________________________
# systematic definitions

def remove_minor_systematics(systematics):
    """
    return iterator that only uses 'major' systematics, i.e. no JES
    breakdown. Works by removing the 'breakdown' systematics.
    """
    # TODO: unify this list with the one in susy-setup-distill.py
    filt_regexes = [
        re.compile('jenp[0-9](up|down)'),
        re.compile('jpu(mu|npv|pt|rho)(up|down)'),
        re.compile('jical[ms](up|down)'),
        re.compile('j(sp|nc|cp|flavcomp|flavresp|bjes)(up|down)'),
        ]
    for syst in systematics:
        if not any(x.search(syst) for x in filt_regexes):
            yield syst

# __________________________________________________________________________
# path / systematic / replacement mappings

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
    try:
        subdirs = listdir(root_path)
    except TypeError:
        # also works with lists / iterable things
        for path in root_path:
            all_syst |= get_all_systematics(path)
        return all_syst
    for stream_dir in subdirs:
        syst_dirs = listdir(join(root_path,stream_dir))
        if not nominal_syst in syst_dirs:
            raise ValueError('no nominal syst found, probably an error')
        all_syst.update(syst_dirs)
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
