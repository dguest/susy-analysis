from scipy.stats import chi2
from math import erf

def poisson_interval(points, alpha=erf(1/2**0.5)): 
    a = alpha
    k = points
    low, high = (chi2.ppf(a/2, 2*k) / 2, chi2.ppf(1-a/2, 2*(k + 1)) / 2)
    low[k == 0] = 0.0
    return low, high
