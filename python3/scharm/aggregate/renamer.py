"""
Routines to rename the finely-grained samples to several larger
categories.
"""

# migrate to schema?
_prefix_replacements = {
    'Wjets': ['Wjets'],
    'Zjets': ['Zjets'],
    'other': ['QCD','diboson'],
    'top': ['singleTop', 'ttbar']
    }
_signal_prefix = ['scharm','stop']

def shorten(name):
    """
    finds and returns the short name for the argument. Returns 'None' for
    signal points. Throw ValueError if no match is found.
    """
    for short, prefi in _prefix_replacements.items():
        for prefix in prefi:
            if name.startswith(prefix):
                return short
    for prefix in _signal_prefix:
        if name.startswith(prefix):
            return None
    raise ValueError("{} doesn't have an identifiable prefix".format(name))
