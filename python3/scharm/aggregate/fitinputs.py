from scharm.aggregate.normalizer import Normalizer
from scharm.aggregate import renamer
from scharm.aggregate.histread import get_count
from collections import Counter
import yaml
import numpy as np

_up_var_prefix = 'up'
_down_var_prefix = 'down'

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
                 quiet=False, veto_region_prefix=None):
        self._meta_path = meta_path
        self._variable = variable
        self._quiet = quiet
        self._vrp = veto_region_prefix

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
                if self._vrp and region.startswith(self._vrp):
                    continue
                nom_count = get_count(vargroup[self._variable]) * norm
                counts_dict[region,process,n_key] += nom_count
                wt_var = self._variable + self._wt2_tag
                if wt_var in vargroup:
                    sum_wt2 = get_count(vargroup[wt_var]) * norm**2
                    counts_dict[region,process,err_key] += sum_wt2

        # step two organizes the flat dict as nested dicts
        rpp = set( (r, p) for r,p,s in counts_dict.keys())
        reg_dict = {}
        for reg, proc in rpp:
            proc_dict = reg_dict.get(reg, {})
            proc_dict[proc] = [ float(counts_dict[reg, proc, n_key]) ]
            if (reg, proc, err_key) in counts_dict:
                proc_dict[proc].append(
                    float(counts_dict[reg, proc, err_key]**0.5))
            reg_dict[reg] = proc_dict

        return reg_dict

# __________________________________________________________________________
# functions to translate from {systematic: {region: {process: yield} } }
# to something closer to what scharm analysis uses

def translate_to_fit_inputs(yields_dict, clean=True):
    """
    translates the yields directory structure into the structure used
    as the fitter input
    """
    nom = _nominal_yields(yields_dict['none'])
    syst = _yield_systematics(yields_dict)
    symsys, asymsys = _sort_sym_asym(syst)
    # run through and remove variations that don't vary
    if clean:
        for syst_name in symsys:
            _cleansyst(syst[syst_name], nom)
        for syst_name in asymsys:
            dn_syst = syst_name + _down_var_prefix
            up_syst = syst_name + _up_var_prefix
            _cleansyst(syst[dn_syst], nom, other_sys=syst[up_syst])

    return {
        'nominal_yields': nom,
        'yield_systematics': syst
        }

def _cleansyst(regdict, nom, other_sys=None):
    """
    remove entries that are identical to nominal. If other_sys is given,
    will only remove the entries where _both_ are identical.
    """
    kill_list = []
    for regname, procdict in regdict.items():
        for procname, counts in procdict.items():
            nomcount = nom[regname].get(procname)
            def isnom(ct):
                return (not nomcount) or (nomcount[0] == ct[0])
            if other_sys:
                other_count = other_sys[regname][procname]
                if isnom(other_count) and isnom(counts):
                    kill_list.append((regname, procname))
            else:
                if isnom(counts):
                    kill_list.append((regname, procname))

    def clean(region_dict):
        for reg, proc in kill_list:
            del region_dict[reg][proc]
            if not region_dict[reg]:
                del region_dict[reg]

    clean(regdict)
    if other_sys:
        clean(other_sys)

_preselfix = 'presel'
def _presel_region(reg_name):
    """find preselection regions"""
    return reg_name.startswith(_preselfix)

def _drop_stat_uncert(yields):
    """
    recursively dig through yields, return only the yield, not the
    statistical uncertainty. Assumes everything is a dict except for the
    final yield (which is assumed to be a list).
    """
    ret_dict = {}
    for key, subdict in yields.items():
        try:
            ret_dict[key] = [subdict[0]]
        except KeyError:
            ret_dict[key] = _drop_stat_uncert(subdict)
    return ret_dict

def _nominal_yields(yields):
    """strip out regions with zero yield"""
    trim = {k:v for k, v in yields.items() if not _presel_region(k)}
    out = {}
    for reg_name, procs in trim.items():
        out[reg_name] = {}
        for proc_name, proc_yield in procs.items():
            if proc_yield[0] > 0.0:
                out[reg_name][proc_name] = proc_yield
    return out

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

def _sort_sym_asym(yields):
    """takes output from _yield_systematics, gets sym / asym keys"""
    uvp = _up_var_prefix
    dvp = _down_var_prefix
    udvar = {y[:-len(uvp)] for y in yields if y.endswith(uvp)}
    udvar_dn_check = {y[:-len(dvp)] for y in yields if y.endswith(dvp)}
    if udvar != udvar_dn_check:
        raise ValueError('found unmatched up ({}) or down ({})'.format(
                ', '.join(udvar), ', '.join(udvar_dn_check)))
    sym_keys = {x + y for x in udvar for y in [dvp, uvp]}
    return set(yields.keys()) - sym_keys, udvar

#___________________________________________________________________________
# helpers

