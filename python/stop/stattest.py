from scipy.stats import chi2, beta
from math import erf
import numpy as np

def poisson_interval(points, alpha=erf(1/2**0.5)): 
    a = alpha
    k = points
    low, high = (chi2.ppf(a/2, 2*k) / 2, chi2.ppf(1-a/2, 2*(k + 1)) / 2)
    low[k == 0] = 0.0
    return low, high

def binomial_interval(x, n, alpha=(1-erf(1/2**0.5))): 
    low = beta.ppf(alpha/2, x, n - x - 1) * n
    high = beta.ppf(1-alpha/2, x + 1, n - x) * n
    return low, high

def _valid_delta(n, wt2): 
    out = np.empty(n.shape)
    out[:] = float('nan')
    mask = wt2 > 0.0
    out[mask] = n[mask] / wt2[mask]**0.5
    return out

def efficiency_error(p, f, p_wt2, f_wt2): 
    delta_p = _valid_delta(p, p_wt2)
    delta_f = _valid_delta(f, f_wt2)
    error = ( (delta_p * f)**2 + (delta_f * p)**2 )**0.5 / (p + f)**2.0
    return error
