from os.path import dirname
from stop.bullshit import OutputFilter
import os
import errno

def load_susyfit(): 
    """
    Loads the root interfaces with HistFitter
    """
    from distutils import spawn
    
    hf = dirname(spawn.find_executable('HistFitter.py'))
    import ROOT
    with OutputFilter(accept_re='ERROR'): 
        ROOT.gSystem.Load('{}/../lib/libSusyFitter.so'.format(hf))

def make_dir_if_none(hists_dir): 
    """
    Avoids race condition from launching multiple jobs. 
    """
    try: 
        os.makedirs(hists_dir)
    except OSError as err: 
        if err.errno == errno.EEXIST and os.path.isdir(hists_dir): 
            pass
        else: 
            raise
