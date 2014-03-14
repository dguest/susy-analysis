"""
Tools to select set of histogram files which can be coherently aggregated.
"""
from scharm.schema import dir_key_order
import os
from os.path import relpath, join, isfile

def get_all_files(root_list, systematic='none'): 
    """
    runs over a list of root dirs (or files), makes a flattened list of
    files.
    """
    all_files = []
    for root in root_list: 
        if isfile(root): 
            all_files.append(root)
        all_files.extend(get_files(root, systematic))
    return all_files

def get_files(root_dir, systematic='none', plot_type=None): 
    all_files = []
    sys_level = dir_key_order.index('systematic')
    # files are stored two directories past the 'replacement' level
    # (the hist type sits in the middle)
    plot_type_level = dir_key_order.index('replacement') + 1
    file_level = plot_type_level + 1
    for root, dirs, files in os.walk(root_dir): 
        rpth = relpath(root, root_dir).split('/')
        levels = len([d for d in rpth if d != '.'])
        if levels == sys_level: 
            if not systematic in dirs: 
                raise FileNotFoundError(
                    "can't find {} in {}".format(systematic, root))
            dirs[:] = [systematic]
        elif levels == plot_type_level:
            if plot_type is not None: 
                if not plot_type in dirs:
                    raise FileNotFoundError(
                        "can't find {} in {}".format(plot_type, root))
                dirs[:] == [plot_type]
        elif levels == file_level: 
            all_files.extend(join(root,f) for f in files)
        elif files: 
            raise RuntimeError(
                "found files: ({}) in {}, where they aren't expected".format(
                    ', '.join(files), root))
    return all_files
