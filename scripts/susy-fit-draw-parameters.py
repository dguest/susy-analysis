#!/usr/bin/env python3.3
"""
Plotting routine for the fit results (which are produced as a yaml file).
"""
_fit_parameters_help = "yaml file produced by susy-fit-results.py"

import argparse, sys, yaml
from os.path import join, isdir
import os

def run():
    """top level routine"""
    args = _get_args()
    pars_dict = _get_fit_pars(args.fit_parameters)
    outinfo = dict(ext=args.ext, outdir=args.plot_directory)
    _plot_mu_parameters(pars_dict, outinfo)
    # _plot_alpha_parameters(pars_dict, outinfo)

def _plot_mu_parameters(pdict, outinfo):
    import numpy as np
    from matplotlib.figure import Figure
    from matplotlib.backends.backend_agg import FigureCanvasAgg as FigCanvas
    pars = {
        x.split('_',1)[1]:y for x,y in pdict.items() if x.startswith('mu_')}
    npars = len(pars)
    xpos = np.arange(0.5, npars + 0.5)
    ypos = np.zeros(npars)
    yerr = np.zeros(npars)
    xlab = ['x'] * npars
    for parnum, (name, parvals) in enumerate(pars.items()):
        xlab[parnum] = name
        after = parvals['after']
        ypos[parnum] = after['value']
        yerr[parnum] = after['error']
    fig = Figure(figsize=(9, 8))
    canvas = FigCanvas(fig)
    ax = fig.add_subplot(1,1,1)
    ax.errorbar(xpos, ypos, yerr=yerr)
    outdir = outinfo['outdir']
    if not isdir(outdir):
        os.mkdir(outdir)
    canvas.print_figure(join(outdir, 'test' + outinfo['ext']))

def _get_args():
    """input parser"""
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        'fit_parameters', help=_fit_parameters_help, nargs='?')
    parser.add_argument('-o','--plot-directory', default='fitplots')
    parser.add_argument('-e','--ext', default='.pdf', help='plot type')
    return parser.parse_args()

def _get_fit_pars(fit_parameters):
    if not fit_parameters and not sys.stdin.isatty():
        return yaml.load(sys.stdin)

    with open(fit_parameters) as pars:
        return yaml.load(pars)

if __name__ == '__main__':
    run()
