"""
Wrapper for the matrix that takes care of calculating uncertainties.
"""
import numpy as np

_cormat_key = 'correlation_matrix'
_par_key = 'parameters'
_mat_key = 'matrix'

class CorrelationMatrix:
    """Wrapper to do correlation stuff"""
    def __init__(self, varlist, array_like):
        self._matrix = np.matrix(array_like)
        self._varpos = {y:x for x, y in enumerate(varlist)}

    @classmethod
    def from_dict(cls, matdic):
        """Expects `correlation_matrix` from the fit parameters"""
        parameters = matdic[_par_key]
        array = matdic[_mat_key]
        return cls(parameters, array)

    def get_error2(self, error_dict):
        err_vector = np.zeros(len(error_dict))
        for name, err in error_dict.items():
            err_vector[self._varpos[name]] = err
        tot = err_vector * self._matrix * err_vector[:, np.newaxis]
        return float(tot)

def test():
    testdic = {
        'parameters': ['p1', 'p2'],
        'matrix': [ [1.0, -0.0], [-0.0, 1.0] ] }
    cmat = CorrelationMatrix.from_dict(testdic)
    err = cmat.get_error2({'p1': 0.1, 'p2': 0.1})
    print(err)
    return err

def get_sample_rel_error(nom, systs, rels, region, process, systs):
    """
    retruns a dic of relative errors
    """
    nom_yield = nom[region][process][0]

    def get_or_nom(syst):
        try:
            return systs[syst][region][process][0]
        except KeyError:
            return nom_yield

    def get_asymmetric_error(syst):
        """return average relative error"""
        y_up = get_or_nom(syst + 'up')
        y_dn = get_or_nom(syst + 'down')
        return (y_up - y_dn) / 2 / nom_yield

    def get_symmetric_error(syst):
        """return one half the relative excess in the varaition"""
        return (get_or_nom(syst) - nom_yield) / 2 / nom_yield

    def get_rel_error(syst):
        """return the relative error, properly"""
        try:
            bg_rels = rels[syst][region]
            # try to get the sample specific one
            try:
                bg_rels = bg_rels[process]
            except: pass
            return (bg_rels[1] - bg_rels[0]) / 2
        except KeyError:
            return 0

    rel_in = yields.get(rel_key,{})
    rel_systs = {}

    # the actual filling starts here
    yld_syst_names = set(systs)
    for syst in systs:
        if syst in yld_syst_names:
            rel_systs[syst] = get_symmetric_error(syst)
        elif any(x + ext in yld_syst_names for ext in ['up','down']):
            rel_systs[syst] = get_asymmetric_error(syst)
        elif syst in rel_in:
            rel_systs[syst] = get_rel_error(syst)
        else:
            raise ValueError('{} not found'.format(syst))
    return rel_systs
