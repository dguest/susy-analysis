from os.path import join, isdir
import os, itertools
from scharm.limits.limitsty import alpha_names, reg_names
from scharm.schema import get_jes_variations
import numpy as np

_eb_style = dict(linewidth=2, color='k', fmt='o')
_hline_style = dict(linestyle='--', color='k')
_txt_size = 16

def _get_lab_x_y_err(pars):
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

def _sort_labels(labels):
    jes_variations = [x.lower() for x in get_jes_variations()]
    jes_variations += ['jes', 'jer']
    tagging = []
    jet = []
    lep = []
    other = []
    mu = []
    for longkey in labels:
        if not longkey.startswith('alpha_'):
            mu.append(longkey)
            continue
        key = longkey.split('_',1)[1]
        if key in 'bcut':
            tagging.append(longkey)
        elif key in ['el', 'mu', 'egzee', 'mscale', 'eglow', 'leptrig']:
            lep.append(longkey)
        elif key in jes_variations:
            jet.append(longkey)
        else:
            other.append(longkey)
    # stick the lists together, sorted
    return list(itertools.chain.from_iterable(
            [sorted(x) for x in [tagging, jet, lep, other, mu]]))

# ___________________________________________________________________________
# for mu parameters

def plot_mu_parameters(pdict, outinfo, lumi=False):
    from matplotlib.figure import Figure
    from matplotlib.backends.backend_agg import FigureCanvasAgg as FigCanvas
    pars = []
    for x, y in sorted(pdict.items()):
        if x.startswith('mu_'):
            pars.append( (x.split('_',1)[1], y ))
        elif x == 'Lumi' and lumi:
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

# __________________________________________________________________________
# for alpha parameters

def _sort_alpha(pdict):
    """return dict sorted by type of systematic, along with division index"""
    jes_variations = [x.lower() for x in get_jes_variations()]
    jes_variations += ['jes', 'jer']

    tagging = []
    jet = []
    lep = []
    other = []
    for longkey, val in pdict.items():
        if not longkey.startswith('alpha_'):
            continue
        key = longkey.split('_',1)[1]
        kv = (alpha_names.get(key,key), val)
        if key in 'bcut':
            tagging.append(kv)
        elif key in ['el', 'mu', 'egzee', 'mscale', 'eglow', 'leptrig']:
            lep.append(kv)
        elif key in jes_variations:
            jet.append(kv)
        else:
            other.append(kv)
    # stick the lists together, sorted
    lists = [sorted(x) for x in [tagging, jet, lep, other]]
    idxs = [len(tagging)]
    for l in lists[1:]:
        idxs.append(idxs[-1] + len(l))
    return list(itertools.chain(*lists)), idxs

def plot_alpha_parameters(pdict, outinfo):
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
        lab.set_rotation(60 if len(xlab) < 10 else 90)

    outdir = outinfo['outdir']
    if not isdir(outdir):
        os.mkdir(outdir)

    fig.tight_layout(pad=0.3, h_pad=0.3, w_pad=0.3)
    canvas.print_figure(
        join(outdir, 'alpha' + outinfo['ext']), bboxinches='tight')

# __________________________________________________________________________
# correlation matrix

def _rename_corr_var(name):
    alp = 'alpha_'
    if name.startswith(alp):
        trunc = name[len(alp):]
        return alpha_names.get(trunc, trunc)
    gstat = 'gamma_stat_'
    stattail = '_bin_0'
    if name.startswith(gstat):
        trunc = name[len(gstat):-len(stattail)]
        name = reg_names.get(trunc, trunc)
        return name + " Stat"
    mu = 'mu_'
    if name.startswith(mu):
        return name[len(mu):]
    return name

def _xyiter(matrix):
    xyitr = [range(x) for x in matrix.shape]
    for binx, biny in itertools.product(*xyitr):
        yield binx, biny

def _sort_matrix(labels, matrix):
    """Reorders a correlation matrix"""
    lsort = _sort_labels(labels)

    # build new matrix
    new_idx = [lsort.index(x) for x in labels]
    new_mat = np.zeros(matrix.shape)
    for binx, biny in _xyiter(matrix):
        newx = new_idx[binx]
        newy = new_idx[biny]
        new_mat[newx, newy] = matrix[binx, biny]
    return lsort, new_mat

def _add_numbers(ax, matrix):
    maxval = np.max(matrix[matrix < 1.0])
    minval = np.min(matrix)
    valrg = maxval - minval
    text_args = dict(fontsize=7, ha='center', va='center')
    for binx, biny in _xyiter(matrix):
        val = matrix[binx, biny]
        # hackish way to make some values different colors
        if (val - minval) / valrg > 0.3:
            col = 'k'
        else:
            col = 'w'
        ax.text(binx, biny, '{:.2f}'.format(val), color=col, **text_args)

def plot_corr_matrix(pars, outinfo):
    """correlation matrix plotter"""
    from matplotlib.figure import Figure
    from matplotlib.backends.backend_agg import FigureCanvasAgg as FigCanvas
    from mpl_toolkits.axes_grid1 import make_axes_locatable
    from matplotlib.colorbar import Colorbar
    from matplotlib.pyplot import get_cmap

    labels = pars['correlation_matrix']['parameters']
    matrix = np.array(pars['correlation_matrix']['matrix'])
    labels, matrix = _sort_matrix(labels, matrix)
    short_labels = [_rename_corr_var(x) for x in labels]
    maxval = np.max(matrix[matrix < 1.0])

    fig = Figure(figsize=(8, 8))
    fig.subplots_adjust(bottom=0.2)
    canvas = FigCanvas(fig)
    ax = fig.add_subplot(1,1,1)
    im = ax.matshow(matrix, vmax=maxval, cmap=get_cmap('hot'))
    im.get_cmap().set_over('grey', 1.0)

    tickpos = np.linspace(0, len(labels) - 1, len(labels))
    ax.set_xticks(tickpos)
    ax.set_xticklabels(short_labels)
    ax.xaxis.tick_bottom()
    ax.set_yticks(tickpos)
    ax.set_yticklabels(short_labels)
    divider = make_axes_locatable(ax)
    cax = divider.append_axes("right", size="5%", pad=0.05)
    cb = Colorbar(ax=cax, mappable=im)
    _add_numbers(ax, matrix)

    for lab in ax.get_xticklabels():
        lab.set_rotation(90)
    fig.tight_layout(pad=1.0, h_pad=0.3, w_pad=1.0)
    canvas.print_figure(
        join(outinfo['outdir'], 'cor_matrix' + outinfo['ext']),
        bboxinches='tight')

