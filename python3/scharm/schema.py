"""
misc file naming / path interpenetration stuff
"""

from os.path import join

dir_key_order = ['stream', 'systematic', 'replacement']

def distiller_settings_from_dir(path): 
    return dict(zip(dir_key_order, path.split('/')[-3:]))

def dir_from_distiller_settings(settings, base=''): 
    return join(base, *(settings[key] for key in dir_key_order))

def test(test_path='this/is/a/path'): 
    settings = distiller_settings_from_dir(test_path)
    repath = dir_from_distiller_settings(settings, 'this')
    if test_path != repath: 
        raise Exception("path schema not working: {} => {}".format(
                test_path, repath))
    return repath
