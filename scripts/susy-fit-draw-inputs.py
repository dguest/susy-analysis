#!/usr/bin/env python3.3
"""
Plotting routine for the fit inputs.
"""

import argparse
import sys, os
import yaml
from collections import Counter
from scharm.limits.limitsty import alpha_names, reg_names

_txt_size = 16
_summed_bg = ['Wjets', 'Zjets', 'top', 'other']
_def_regions = ['cr_z', 'signal_mct150', 'cr_t', 'cr_w_mct150_l1pt50']
_def_syst = ['jer','jes','met','metres', 'b', 'c', 'u', 't']
_sys_lists = {
    'default': _def_syst,
    'jesbd': [
        'jcb', 'jicalm', 'jicals', 'jpumu',
        'jpunpv', 'jpupt', 'jpurho', 'jsp'], #, 'jnc'
    'jesnp': ['jenp1', 'jenp2', 'jenp3', 'jenp4', 'jenp5', 'jenp6'],
    'jesflav': ['jbjes', 'jflavcomp', 'jflavresp'],
    }

def _get_args():
    """input parser"""
    d = 'default: %(default)s'
    syst_help = d + ' allowed expansions: {}'.format(
        ', '.join(_sys_lists.keys()))
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('fit_inputs')
    parser.add_argument('-r','--regions', nargs='+', default=_def_regions)
    parser.add_argument(
        '-s','--systematics', nargs='+', default=_def_syst, help=syst_help)
    parser.add_argument('-o','--plot-name', default='test.pdf', help=d)
    return parser.parse_args()

def run():
    """top level routine"""
    args = _get_args()
    with open(args.fit_inputs) as yml:
        inputs_dict = yaml.load(yml)

    # allow adding of systematic groups from _sys_lists
    plot_systs = []
    for syst in args.systematics:
        if syst in _sys_lists:
            plot_systs += _sys_lists[syst]
        else:
            plot_systs.append(syst)

    systs = {}
    for syst in plot_systs:
        regs, nom, down, up, data = _get_counts(
            inputs_dict, args.regions, syst)
        systs[syst] = (regs, nom, down, up, data)

    ofile = args.plot_name
    odir = os.path.dirname(ofile)
    if odir and not os.path.isdir(odir):
        os.mkdir(odir)
    _plot_counts(systs, ofile)

_syst_colors = list('rgbcmk') + ['orange', 'purple', 'brown', 'white']
def _plot_counts(counts, out_file):
    from matplotlib.figure import Figure
    from matplotlib.backends.backend_agg import FigureCanvasAgg as FigCanvas
    from numpy import arange

    fig = Figure(figsize=(8, 3))
    canvas = FigCanvas(fig)
    ax = fig.add_subplot(1,1,1)

    trash, ex_sys = next(iter(counts.items()))
    ex_regs, ex_nom, *nada, ex_data = ex_sys

    # offsetter for systematics
    sysw = 0.5
    syst_increment = sysw / (len(counts) - 1)
    syst_initial = -sysw / 2
    sys_num = {x:n for n, x in enumerate(sorted(counts.keys()))}
    def get_offset(syst):
        return syst_initial + sys_num[syst] * syst_increment

    x_vals_base = arange(len(ex_regs)) + 0.5
    ax.errorbar(
        x_vals_base, ex_data / ex_nom, yerr=ex_data**0.5/ ex_nom,
        fmt='o', color='k', label='data')

    ax.set_xticks(x_vals_base)
    ax.set_xticklabels([reg_names.get(x,x) for x in ex_regs])
    for sysnm, (regions, nom, down, up, data) in sorted(counts.items()):
        x_vals = x_vals_base + get_offset(sysnm)
        color = _syst_colors[sys_num[sysnm]]
        ax.set_xlim(0, len(regions))
        up_vals = up / nom
        ax.plot(x_vals, up_vals , '^', color=color,
                label=alpha_names.get(sysnm, sysnm))
        dn_vals = down / nom
        ax.plot(x_vals, dn_vals, 'v', color=color)

    ax.tick_params(labelsize=_txt_size)
    leg = ax.legend(
        numpoints=1, ncol=5, borderaxespad=0.0, loc='upper left',
        handletextpad=0, columnspacing=1, framealpha=0.5, fontsize=10)

    ax.axhline(1, linestyle='--', color=(0,0,0,0.5))
    ylims = ax.get_ylim()
    # ax.set_ylim(ylims[0], (ylims[1] - ylims[0]) *0.2 + ylims[1])
    ax.set_ylim(0.8, 1.3)
    ax.set_ylabel('Variation / Nominal')
    fig.tight_layout(pad=0.3, h_pad=0.3, w_pad=0.3)
    canvas.print_figure(out_file, bboxinches='tight')

def _get_counts(inputs_dict, regions, syst):
    """returns counts as a tuple of Counters, (nom, down, up, data)"""
    yields = inputs_dict['nominal_yields']
    systs = inputs_dict['yield_systematics']
    if syst in systs:
        return _get_sym_counts(inputs_dict, regions, syst)
    nup = syst + 'up'
    ndn = syst + 'down'
    if any(x not in systs for x in [nup, ndn]):
        raise ValueError("couldn't find up / down systematics for {}".format(
                syst))

    reg_up_counter = Counter()
    reg_dn_counter = Counter()
    reg_nom_counter = Counter()
    data_counter = {}
    for region in regions:
        for bg in _summed_bg:
            def get_yld(dic, reg, bg):
                return dic[reg].get(bg, [0])[0]
            try:
                reg_nom_counter[region] += get_yld(yields,region,bg)
                reg_up_counter[region] += get_yld(systs[nup],region,bg)
                reg_dn_counter[region] += get_yld(systs[ndn],region,bg)
            except KeyError as err:
                raise KeyError("can't find {} {}".format(region, bg))
        data_counter[region] = yields[region].get('data',[float('NaN')])[0]

    return _arrays_from_counters(
        reg_nom_counter, reg_dn_counter, reg_up_counter, data_counter)

def _get_sym_counts(inputs_dict, regions, syst):
    import numpy as np
    yields = inputs_dict['nominal_yields']
    systs = inputs_dict['yield_systematics']
    nom_counter = Counter()
    alt_counter = Counter()
    data_counter = {}
    for region in regions:
        for bg in _summed_bg:
            def get_yld(dic, reg, bg):
                return dic[reg].get(bg, [0])[0]
            nom_counter[region] += get_yld(yields,region,bg)
            alt_counter[region] += get_yld(systs[syst],region,bg)
        data_counter[region] = yields[region].get('data',[float('NaN')])[0]

    sreg = sorted(regions)
    n_reg = len(regions)
    nom_array = np.zeros(n_reg)
    alt_array = np.zeros(n_reg)
    data_array = np.zeros(n_reg)
    for idx, reg in enumerate(sreg):
        nom_array[idx] = nom_counter[reg]
        alt_array[idx] = alt_counter[reg]
        data_array[idx] = data_counter[reg]
    diff = np.abs(nom_array - alt_array)
    down = nom_array - 0.5*diff
    up = nom_array + 0.5*diff
    return sreg, nom_array, down, up, data_array

def _arrays_from_counters(*counters):
    """
    Takes dicts, returns (region_list, nominal, down, up).
    Meant to handle the asym counts only.
    """
    from numpy import zeros
    nom, down, up, data = counters
    regions = list(sorted(nom.keys()))
    n_reg = len(regions)
    dn_array = zeros(n_reg)
    up_array = zeros(n_reg)
    nom_array = zeros(n_reg)
    data_array = zeros(n_reg)
    for idx, reg in enumerate(regions):
        dn_array[idx] = down[reg]
        up_array[idx] = up[reg]
        nom_array[idx] = nom[reg]
        data_array[idx] = data[reg]
    return regions, nom_array, dn_array, up_array, data_array

if __name__ == '__main__':
    run()
