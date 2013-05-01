from scipy.stats import chi2, beta
from math import erf

def poisson_interval(points, alpha=erf(1/2**0.5)): 
    a = alpha
    k = points
    low, high = (chi2.ppf(a/2, 2*k) / 2, chi2.ppf(1-a/2, 2*(k + 1)) / 2)
    low[k == 0] = 0.0
    return low, high

def binomial_interval(x, n, alpha=(1-erf(1/2**0.5))): 
    low = beta.ppf(alpha/2, x, n - x - 1) * n
    high = beta.ppf(alpha/2, x + 1, n - x) * n
    return low, high
