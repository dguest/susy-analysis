from os.path import dirname
from stop.bullshit import OutputFilter
def load_susyfit(): 
    """
    Loads the root interfaces with HistFitter
    """
    from distutils import spawn
    
    hf = dirname(spawn.find_executable('HistFitter.py'))
    import ROOT
    with OutputFilter(): 
        ROOT.gSystem.Load('{}/../lib/libSusyFitter.so'.format(hf))
