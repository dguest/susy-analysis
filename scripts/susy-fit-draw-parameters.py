#!/usr/bin/env python3.3
"""
Plotting routine for the fit results (which are produced as a yaml file).
"""
_fit_parameters_help = "yaml file produced by susy-fit-results.py"

import argparse, sys, yaml
from os.path import join, isdir
import os, itertools

def run():
    """top level routine"""
    args = _get_args()
    pars_dict = _get_fit_pars(args.fit_parameters)
    outinfo = dict(ext=args.ext, outdir=args.plot_directory)
    _plot_mu_parameters(pars_dict, outinfo)
    _plot_alpha_parameters(pars_dict, outinfo)

_eb_style = dict(linewidth=2, color='k', fmt='o')
_hline_style = dict(linestyle='--', color='k')
_txt_size = 16

def _get_lab_x_y_err(pars):
    import numpy as np
    npars = len(pars)
    xpos = np.arange(0.5, npars + 0.5)
    ypos = np.zeros(npars)
    yerr = np.zeros(npars)
    xlab = ['x'] * npars
    for parnum, (name, parvals) in enumerate(pars):
        xlab[parnum] = name
        after = parvals['after']
        ypos[parnum] = after['value']
        yerr[parnum] = after['error']
    return xlab, xpos, ypos, yerr

def _plot_mu_parameters(pdict, outinfo):
    from matplotlib.figure import Figure
    from matplotlib.backends.backend_agg import FigureCanvasAgg as FigCanvas
    pars = []
    for x, y in sorted(pdict.items()):
        if x.startswith('mu_'):
            pars.append( (x.split('_',1)[1], y ))
        elif x == 'Lumi':
            pars.append((x,y))
    xlab, xpos, ypos, yerr = _get_lab_x_y_err(pars)
    fig = Figure(figsize=(4, 4))
    canvas = FigCanvas(fig)
    ax = fig.add_subplot(1,1,1)
    ax.set_xlim(0, len(xlab))
    ax.set_ylim(0, 2)
    ax.errorbar(
        xpos, ypos, yerr=yerr, **_eb_style)
    ax.axhline(1, **_hline_style)
    ax.set_xticks(xpos)
    ax.set_xticklabels(xlab)
    ax.tick_params(labelsize=_txt_size)
    outdir = outinfo['outdir']
    if not isdir(outdir):
        os.mkdir(outdir)
    fig.tight_layout(pad=0.3, h_pad=0.3, w_pad=0.3)
    canvas.print_figure(
        join(outdir, 'mu' + outinfo['ext']))

def _sort_alpha(pdict):
    """return dict sorted by type of systematic, along with division index"""
    tagging = []
    jet = []
    lep = []
    other = []
    for longkey, val in pdict.items():
        if not longkey.startswith('alpha_'):
            continue
        key = longkey.split('_',1)[1]
        kv = (key, val)
        if key in 'bcut':
            tagging.append(kv)
        elif key in ['el', 'mu', 'egzee', 'mscale', 'eglow']:
            lep.append(kv)
        elif key.startswith('je'):
            jet.append(kv)
        else:
            other.append(kv)
    # stick the lists together, sorted
    lists = [sorted(x) for x in [tagging, jet, lep, other]]
    idxs = [len(tagging)]
    for l in lists[1:]:
        idxs.append(idxs[-1] + len(l))
    return list(itertools.chain(*lists)), idxs

def _plot_alpha_parameters(pdict, outinfo):
    from matplotlib.figure import Figure
    from matplotlib.backends.backend_agg import FigureCanvasAgg as FigCanvas
    pars = []
    parlist, div_idxs = _sort_alpha(pdict)
    xlab, xpos, ypos, yerr = _get_lab_x_y_err(parlist)
    fig = Figure(figsize=(8, 4))
    fig.subplots_adjust(bottom=0.2)
    canvas = FigCanvas(fig)
    ax = fig.add_subplot(1,1,1)
    ax.set_xlim(0, len(xlab))
    ax.set_ylim(-1.5, 1.5)
    ax.errorbar(
        xpos, ypos, yerr=yerr, **_eb_style)
    ax.axhline(0, **_hline_style)
    for hline in div_idxs:
        ax.axvline(hline, **_hline_style)
    ax.set_xticks(xpos)
    ax.set_xticklabels(xlab)
    ax.tick_params(labelsize=_txt_size)
    for lab in ax.get_xticklabels():
        lab.set_rotation(30)

    outdir = outinfo['outdir']
    if not isdir(outdir):
        os.mkdir(outdir)

    fig.tight_layout(pad=0.3, h_pad=0.3, w_pad=0.3)
    canvas.print_figure(
        join(outdir, 'alpha' + outinfo['ext']), bboxinches='tight')

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
