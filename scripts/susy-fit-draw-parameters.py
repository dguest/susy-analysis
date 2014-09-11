#!/usr/bin/env python3.3
"""
Plotting routine for the fit results (which are produced as a yaml file).
"""
_fit_parameters_help = "yaml file produced by susy-fit-results.py"
_full_correlation_help = ('plot full correlation matrix (rather than just '
                          'correlations with mu parameters)')

import argparse
import sys, yaml
from os.path import isdir
from os import mkdir
from scharm.limits.fitpars import plot_mu_parameters, plot_alpha_parameters
from scharm.limits.fitpars import plot_corr_matrix

def run():
    """top level routine"""
    args = _get_args()
    pars_dict = _get_fit_pars(args.fit_parameters)
    outinfo = dict(ext=args.ext, outdir=args.plot_directory)
    if not isdir(args.plot_directory):
        mkdir(args.plot_directory)
    plot_mu_parameters(pars_dict, outinfo, lumi=args.lumi)
    plot_alpha_parameters(pars_dict, outinfo)
    plot_corr_matrix(pars_dict, outinfo, crop=args.crop_corr)

def _get_args():
    """input parser"""
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        'fit_parameters', help=_fit_parameters_help, nargs='?')
    parser.add_argument('-o','--plot-directory', default='bg_fit')
    parser.add_argument('-e','--ext', default='.pdf', help='plot type')
    parser.add_argument('-n','--no-lumi', action='store_false', dest='lumi')
    parser.add_argument(
        '-f','--full-correlation', action='store_false', dest='crop_corr',
        help=_full_correlation_help)
    return parser.parse_args()

def _get_fit_pars(fit_parameters):
    if not fit_parameters and not sys.stdin.isatty():
        return yaml.load(sys.stdin)

    with open(fit_parameters) as pars:
        return yaml.load(pars)


if __name__ == '__main__':
    run()
