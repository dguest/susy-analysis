"""
Naming conventions and tools for interacting with official datasets.
"""

import re

scharm_re = 'Scharm_dir_([0-9]+)_([0-9]+)_MET([0-9]+)'
stop_re = 'stopToCharmLSP_t(\d+)_n(\d+)_MET(\d+)'
stop2_re = 'directCC_(\d+)_(\d+)'

def get_mass_pair(dataset_name):
    """
    checks to see if the dataset_name matches any known formats,
    returns (msusy, mlsp) pair as `int`s
    """
    finders = [re.compile(n) for n in [scharm_re, stop_re, stop2_re]]
    for finder in finders:
        match = finder.search(dataset_name)
        if match:
            found = match.group
            return int(found(1)), int(found(2))
    return None
