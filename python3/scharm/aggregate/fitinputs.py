from scharm.aggregate.normalizer import Normalizer
from scharm.aggregate import renamer
from collections import Counter
import yaml
import numpy as np

class FitInputMaker:
    """
    Takes some meta data and one systematic as an input.
    Produces fit inputs dictionary.
    """

    # the sum(weight^2) histograms have a special tag that tells
    # us to multiply by the squared normalization
    _wt2_tag = 'Wt2'
    _signal_prefix = 'scharm'

    def __init__(self, meta_path, variable='met', signal_point=None,
                 quiet=False):
        self._meta_path = meta_path
        self._variable = variable
        self._quiet = quiet

        # signal finder (for short test jobs without much signal)
        if signal_point is None:
            def finder(proc):
                return True
        else:
            def finder(proc):
                if proc.startswith(self._signal_prefix):
                    if proc != signal_point:
                        return False
                return True
        self._sig_finder = finder

    def make_inputs(self, hfiles):
        """
        Returns a dictionary for one systematic variation
        """
        # these keys are only used by this function
        n_key = 'n'
        err_key = 'err'

        normalizer = Normalizer(self._meta_path, hfiles, quiet=self._quiet)
        # step one is to load everything into a flat dict
        counts_dict = Counter()
        for full_process, hfile, norm in normalizer:
            if not self._sig_finder(full_process):
                continue
            # rename processes to simplify fit
            process = renamer.shorten(full_process) or full_process
            for region, vargroup in hfile.items():
                nom_count = _get_count(vargroup[self._variable]) * norm
                wt_var = self._variable + self._wt2_tag
                sum_wt2 = _get_count(vargroup[wt_var]) * norm**2
                if nom_count > 0.0:
                    counts_dict[region,process,n_key] += nom_count
                    counts_dict[region,process,err_key] += sum_wt2

        # step two organizes the flat dict as nested dicts
        rpp = set( (r, p) for r,p,s in counts_dict.keys())
        reg_dict = {}
        for reg, proc in rpp:
            proc_dict = reg_dict.get(reg, {})
            proc_dict[proc] = [
                float(counts_dict[reg, proc, n_key]),
                float(counts_dict[reg, proc, err_key]**0.5)
                ]
            reg_dict[reg] = proc_dict

        return reg_dict

# __________________________________________________________________________
# functions to translate from {systematic: {region: {process: yield} } }
# to something closer to what scharm analysis uses

def translate_to_fit_inputs(yields_dict):
    """
    translates the yields directory structure into the structure used
    as the fitter input
    """
    return {
        'nominal_yields': _nominal_yields(yields_dict['none']),
        'yield_systematics': _yield_systematics(yields_dict)
        }

_preselfix = 'presel'
def _presel_region(reg_name):
    """find preselection regions"""
    return reg_name.startswith(_preselfix)

def _drop_stat_uncert(yields):
    """
    recursively dig through yields, return only the yield, not the
    statistical uncertainty. Assumes everything is a dict except for the
    final yield (which is assumed to be a list.
    """
    ret_dict = {}
    for key, subdict in yields.items():
        try:
            ret_dict[key] = [subdict[0]]
        except KeyError:
            ret_dict[key] = _drop_stat_uncert(subdict)
    return ret_dict

def _nominal_yields(yields):
    """Returns more or less a straight copy, remove preselection maybe"""
    return {k:v for k, v in yields.items() if not _presel_region(k)}

def _yield_systematics(yields):
    """returns a reorganized 'yields'"""
    out = {}
    for syst, regions in yields.items():
        if syst == 'none':
            continue
        out[syst] = {}
        for reg_name, processes in regions.items():
            if _presel_region(reg_name):
                continue
            out[syst][reg_name] = _drop_stat_uncert(processes)
    return out

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
