from scharm.aggregate.normalizer import Normalizer
from collections import defaultdict
import yaml
import numpy as np

class FitInputMaker:
    """
    Takes some meta data and hists as an input, produces fit inputs file.
    """

    # the sum(weight^2) histograms have a special tag that tells
    # us to multiply by the squared normalization
    _wt2_tag = 'Wt2'

    def __init__(self, meta_path, variable='met'):
        self._meta_path = meta_path
        self._variable = variable

    def make_inputs(self, hfiles):
        """
        Returns a dictionary for one systematic variation
        """
        normalizer = Normalizer(self._meta_path, hfiles)
        counts_dict = defaultdict(dict)
        for physics_type, hfile, norm in normalizer:
            for region, vargroup in hfile.items():
                nom_count = _get_count(vargroup[self._variable]) * norm
                wt_var = self._variable + self._wt2_tag
                sum_wt2 = _get_count(vargroup[wt_var]) * norm**2
                counts_dict[region][physics_type] = {
                    'n': nom_count, 'err': sum_wt2**0.5}
        return yaml.dump(counts_dict)

#___________________________________________________________________________
# helpers

# constants from the histogram schema
_sel_str = 'selection_'
_mmax = ['min', 'max']

def _get_count(hist):
    """
    Expects 1d n-1 hist as an input (must have 'selection_min' and
    'selection_max' defined).
    """
    print(hist)
    try:
        if not all(len(hist.attrs[_sel_str + x]) == 1 for x in _mmax):
            raise ValueError('got multi-dim hist, not supported right now')
        smin, smax = [hist.attrs[_sel_str + x ][0] for x in _mmax]
        emin, emax = [hist.attrs[x][0] for x in _mmax]
    except KeyError:
        raise KeyError('missing attributes. Valid keys: {}'.format(
                ', '.join(hist.attrs)) )
    arr = np.array(hist)
    nxpts = arr.size - 1
    xpts = np.linspace(emin, emax, num=nxpts)

    def bindex(value):
        """get the index of the input `value`"""
        if value == np.inf:
            return arr.size
        elif value == -np.inf:
            return 0
        idx = np.abs(xpts - value).argmin()
        rel_err = abs( (xpts[idx] - value) / (xpts[-1] - xpts[0]))
        if rel_err > 0.001:
            raise ValueError('selection mismatch: {} != {}'.format(
                    value, xpts[idx]))
        return idx + 1

    return arr[bindex(smin):bindex(smax)].sum()
