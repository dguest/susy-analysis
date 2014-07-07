"""
routines to read hdf5 stored histograms
"""

import numpy as np

# constants from the histogram schema
_sel_str = 'selection_'
_mmax = ['min', 'max']

def get_count(hist):
    """
    Expects 1d n-1 hist as an input (must have 'selection_min' and
    'selection_max' defined).
    """
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

    return float(arr[bindex(smin):bindex(smax)].sum())
