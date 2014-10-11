from os.path import join
import os, itertools
import colorsys                 # to convert to YIQ
from scharm.limits.limitsty import alpha_names, reg_names, mu_names
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
    """return sorted lables, together with the division indexes"""
    jes_variations = [x.lower() for x in get_jes_variations()]
    jes_variations += ['jes', 'jer', 'jvf']
    tagging = []
    jet = []
    lep = []
    met = []
    other = []
    mu = []
    theory = []
    gamma = []
    for longkey in labels:

        if any(longkey.startswith(x) for x in ['gamma_', 'mu_', 'Lumi']):
            if longkey.startswith('gamma_'):
                gamma.append(longkey)
            elif longkey.startswith('mu_'):
                mu.append(longkey)
            else:
                other.append(longkey)
            continue

        key = longkey.split('_',1)[1]
        if key in 'bcut':
            tagging.append(longkey)
        elif key in ['el', 'mu', 'egzee', 'mscale', 'eglow', 'leptrig']:
            lep.append(longkey)
        elif key in jes_variations:
            jet.append(longkey)
        elif key.startswith('theory_') or key in ['signal_isr']:
            theory.append(longkey)
        elif key.startswith('met'):
            met.append(longkey)
        else:
            other.append(longkey)
    return _flattenate(tagging, jet, lep, met, theory, other, gamma, mu)

def _flattenate(*list_of_lists):
    """stick the lists together, sorted, return (list, division index)"""
    lists = filter(None, list_of_lists)
    idxs = []
    full_list = []
    idx = 0
    for l in lists:
        idx += len(l)
        idxs.append(idx)
        full_list += sorted(l)

    # we don't care about the last index, it's just the length of the list
    return full_list, idxs[:-1]

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
    fig.tight_layout(pad=0.3, h_pad=0.3, w_pad=0.3)
    canvas.print_figure(
        join(outdir, 'mu' + outinfo['ext']))

# __________________________________________________________________________
# for alpha parameters

def _sort_alpha(pdict):
    """return dict sorted by type of systematic, along with division index"""
    slab, idxs = _sort_labels(pdict.keys())
    alphas = []
    for longkey in slab:
        if not longkey.startswith('alpha_'):
            continue
        key = longkey.split('_',1)[1]
        val = pdict[longkey]
        alphas.append((alpha_names.get(key,key), val))
    return alphas, idxs

def plot_alpha_parameters(pdict, outinfo):
    from matplotlib.figure import Figure
    from matplotlib.backends.backend_agg import FigureCanvasAgg as FigCanvas
    pars = []
    parlist, div_idxs = _sort_alpha(pdict)
    # in some fits we don't include systematics, don't draw anything
    if not parlist:
        return
    xlab, xpos, ypos, yerr = _get_lab_x_y_err(parlist)
    fig = Figure(figsize=(8, 4))
    fig.subplots_adjust(bottom=0.2)
    canvas = FigCanvas(fig)
    ax = fig.add_subplot(1,1,1)
    ax.set_xlim(0, len(xlab))
    ax.set_ylim(-2.5, 2.5)
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
        return r'$\gamma$ ' + name
    mu = 'mu_'
    if name.startswith(mu):
        trunc = name[len(mu):]
        return r'$\mu$ ' + mu_names.get(trunc, trunc)
    return name

def _xyiter(matrix):
    """iterate through all the bins of an nd array"""
    xyitr = [range(x) for x in matrix.shape]
    yield from itertools.product(*xyitr)

def _sort_matrix(labels, matrix):
    """Reorders a correlation matrix"""
    lsort, idx = _sort_labels(labels)

    # build new matrix
    new_idx = [lsort.index(x) for x in labels]
    new_mat = np.zeros(matrix.shape)
    for binx, biny in _xyiter(matrix):
        newx = new_idx[binx]
        newy = new_idx[biny]
        new_mat[newx, newy] = matrix[binx, biny]
    return lsort, new_mat, idx

def _add_numbers(ax, matrix, image, threshold=0.25):
    """
    Add the value of the correlation in each cell as text.
    Boxes with overall brightness lower than `threshold` are colored
    white.
    """
    maxval = np.max(matrix[matrix < 1.0])
    minval = np.min(matrix)
    valrg = maxval - minval
    text_args = dict(fontsize=6, ha='center', va='center')
    for binx, biny in _xyiter(matrix):
        val = matrix[binx, biny]
        # use white in the dark squares
        rgb = image.to_rgba(val)[:3]
        greyval = colorsys.rgb_to_yiq(*rgb)[0]
        col = 'w' if greyval < threshold else 'k'
        ax.text(binx, biny, '{:.0f}'.format(val*100), color=col, **text_args)

def _set_corr_ticks(axis, labels):
    """set ticks for correlation matrix"""
    tickpos = np.linspace(0, len(labels) - 1, len(labels))
    axis.set_ticks(tickpos)
    axis.set_ticklabels(labels)

def plot_corr_matrix(pars, outinfo, crop=False):
    """correlation matrix plotter"""
    from matplotlib.figure import Figure
    from matplotlib.backends.backend_agg import FigureCanvasAgg as FigCanvas
    from mpl_toolkits.axes_grid1 import make_axes_locatable
    from matplotlib.colorbar import Colorbar
    from matplotlib.pyplot import get_cmap

    labels = pars['correlation_matrix']['parameters']
    matrix = np.array(pars['correlation_matrix']['matrix'])
    labels, matrix, lab_groups = _sort_matrix(labels, matrix)
    short_xlabels = short_ylabels = [_rename_corr_var(x) for x in labels]
    if crop and len(lab_groups) > 0: # chop off all but the mu on the y axis
        short_ylabels = short_ylabels[lab_groups[-1]:]
        matrix = matrix[:,lab_groups[-1]:]
    maxval = np.max(matrix[matrix < 1.0])

    fig = Figure(figsize=(8, 3 if crop else 8))
    fig.subplots_adjust(bottom=0.2)
    canvas = FigCanvas(fig)
    ax = fig.add_subplot(1,1,1)
    im = ax.matshow(matrix.T, vmax=maxval, cmap=get_cmap('hot'))
    im.get_cmap().set_over('grey', 1.0)

    _set_corr_ticks(ax.xaxis, short_xlabels)
    _set_corr_ticks(ax.yaxis, short_ylabels)
    for line in lab_groups:
        ax.axvline(line - 0.5, color='k')
        if not crop:
            ax.axhline(line - 0.5, color='k')
    divider = make_axes_locatable(ax)
    if crop:
        cax = divider.append_axes("bottom", size="30%", pad=0.05)
        cb = Colorbar(ax=cax, mappable=im, orientation='horizontal')
    else:
        ax.xaxis.tick_bottom()
        cax = divider.append_axes("right", size="5%", pad=0.05)
        cb = Colorbar(ax=cax, mappable=im)
    _add_numbers(ax, matrix, im)

    for lab in ax.get_xticklabels():
        lab.set_rotation(90)
    fig.tight_layout(pad=1.0, h_pad=0.3, w_pad=1.0)
    canvas.print_figure(
        join(outinfo['outdir'], 'cor_matrix' + outinfo['ext']),
        bboxinches='tight')

