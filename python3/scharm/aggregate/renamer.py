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
_no_prefix = ['scharm','stop', 'data']

class Renamer:
    """
    Class to rename physics processes to whatever they will be in the
    fit and plots.
    """
    def __init__(self, no_bundle={}):
        self._pass_through = set(no_bundle)
    def shorten(self, name):
        """
        finds and returns the short name for the argument. Returns
        'None' for signal points. Throw ValueError if no match is
        found.
        """
        if name in self._pass_through:
            return name
        for short, prefi in _prefix_replacements.items():
            for prefix in prefi:
                if name.startswith(prefix):
                    return short
        for prefix in _no_prefix:
            if name.startswith(prefix):
                return None
        raise ValueError(
            "{} doesn't have an identifiable prefix".format(name))
