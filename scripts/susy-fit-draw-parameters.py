#!/usr/bin/env python3.3
"""
Plotting routine for the fit results (which are produced as a yaml file).
"""
_fit_parameters_help = "yaml file produced by susy-fit-results.py"

import argparse
import sys, yaml
from scharm.limits.fitpars import plot_mu_parameters, plot_alpha_parameters

def run():
    """top level routine"""
    args = _get_args()
    pars_dict = _get_fit_pars(args.fit_parameters)
    outinfo = dict(ext=args.ext, outdir=args.plot_directory)
    plot_mu_parameters(pars_dict, outinfo, lumi=args.lumi)
    plot_alpha_parameters(pars_dict, outinfo)


def _get_args():
    """input parser"""
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        'fit_parameters', help=_fit_parameters_help, nargs='?')
    parser.add_argument('-o','--plot-directory', default='bg_fit')
    parser.add_argument('-e','--ext', default='.pdf', help='plot type')
    parser.add_argument('-l','--lumi', action='store_true')
    return parser.parse_args()

def _get_fit_pars(fit_parameters):
    if not fit_parameters and not sys.stdin.isatty():
        return yaml.load(sys.stdin)

    with open(fit_parameters) as pars:
        return yaml.load(pars)


if __name__ == '__main__':
    run()
