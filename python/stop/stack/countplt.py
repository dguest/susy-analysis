import yaml
from stop.stack.aggregator import HistDict
from stop.bullshit import make_dir_if_none
import os

def plot_counts(args): 
    if args.fits_file: 
        with open(args.fits_file) as fits: 
            all_configs = yaml.load(fits)
    else: 
        from stop.runtypes import marks_types
        all_configs = {
            'blork': { 
                'backgrounds':list(marks_types), 
                'control_regions':['Znunu0', 'Wenu0', 'Wmunu0', 'ttbar0'], 
                'signal_region': 'SR0'
                }
            }
    phys_set = set(['data'])
    for config in all_configs.values(): 
        phys_set |= set(config['backgrounds'])

    if args.signal_point: 
        phys_set.add(args.signal_point)

    cuts = None
    if args.cuts_file: 
        with open(args.cuts_file) as cuts_yml: 
            cuts = {c['region_key']:c for c in yaml.load(cuts_yml)}

    hists = HistDict(args.aggregate, 
                     filt='kinematic', 
                     physics_set=phys_set, 
                     )

    for config_name, config in all_configs.iteritems(): 
        if not config_name in cuts: 
            continue
        sr_cuts = cuts[config_name]
        if args.which == 'counts': 
            save_dir = 'reg-counts'
            plot_func = _plot_config
        elif args.which == 'comp': 
            save_dir = 'reg-comp'
            plot_func = _plot_comp
        make_dir_if_none(save_dir)
        save_name = os.path.join(save_dir, config_name + args.ext)
        plot_func(config, hists, sr_cuts, args.signal_point, save_name, 
                  args.max_val)

def _plot_comp(config, hists, cuts, sig_pt, save_name, max_val): 
    
    from matplotlib.figure import Figure
    from matplotlib.backends.backend_agg import FigureCanvasAgg as FigCanvas
    import numpy as np

    from stop.style import type_dict
    from stop.stattest import poisson_interval

    backgrounds = config['backgrounds']
    signal_region = config['signal_region']
    variable = 'kinematics'
    cr_cuts = dict(met=150, leading_jet_pt=150)

    def get_count(hist, cuts): 
        inf = float('inf')
        met_cut = cuts['met']*1e3
        ljpt_cut = cuts['leading_jet_pt']*1e3
        return hist['leadingJetPt'].slice(ljpt_cut, inf)['met'].slice(
            met_cut, inf)

    figure = Figure(figsize=(9,8))
    canvas = FigCanvas(figure)
    ax = figure.add_subplot(1,1,1)
    control_regions = config['control_regions']
    n = len(control_regions) + 1
    ind = np.arange(n)    # the x locations for the groups
    width = 0.80

    region_sums = np.zeros(n)
    for bg in config['backgrounds']: 

        sr_count = get_count(hists[bg, variable, signal_region], cuts)
        region_sums[0] += sr_count
        for reg_n, cr in enumerate(control_regions,1): 
            region_sums[reg_n] += get_count(hists[bg, variable, cr], cr_cuts)

    bottoms = np.zeros(n)
    leg_entries = []
    for bg in config['backgrounds']: 
        y_vals = np.zeros(n)
        sr_count = get_count(hists[bg, variable, signal_region], cuts)
        y_vals[0] = sr_count / region_sums[0]
        this_color = type_dict[bg].color
        for reg_n, cr in enumerate(control_regions,1): 
            bg_count = get_count(hists[bg, variable, cr], cr_cuts)
            y_vals[reg_n] = bg_count / region_sums[reg_n]
        bar = ax.bar(ind, y_vals, width, bottom=bottoms, color=this_color)
        bottoms += y_vals
        leg_entries.append( (bar[0], bg))

    if sig_pt: 
        y_vals = np.zeros(n)
        sr_count = get_count(hists[sig_pt, variable, signal_region], cuts)
        y_vals[0] = sr_count / region_sums[0]
        color = 'black'
        for reg_n, cr in enumerate(control_regions,1): 
            sig_count = get_count(hists[sig_pt, variable, cr], cr_cuts)
            y_vals[reg_n] = sig_count / region_sums[reg_n]
        bar = ax.bar(ind, y_vals, width, color='black', fill=False, 
                     linestyle='dashed', linewidth=2)
        leg_entries.append( (bar[0], sig_pt))

    ax.set_xticks(ind + width/2)
    ax.set_xticklabels([signal_region] + control_regions)
    ax.set_ylim(0, 1)
    ax.set_ylabel('events / total SM bg')
    leg = ax.legend(*zip(*reversed(leg_entries)), 
                     loc='upper right', framealpha=0.8)
    canvas.print_figure(save_name, bbox_inches='tight')


def _plot_config(config, hists, cuts, sig_pt, save_name, max_val): 
    
    from matplotlib.figure import Figure
    from matplotlib.backends.backend_agg import FigureCanvasAgg as FigCanvas
    import numpy as np

    from stop.style import type_dict
    from stop.stattest import poisson_interval

    backgrounds = config['backgrounds']
    signal_region = config['signal_region']
    variable = 'kinematics'
    cr_cuts = dict(met=150, leading_jet_pt=150)

    def get_count(hist, cuts): 
        inf = float('inf')
        met_cut = cuts['met']*1e3
        ljpt_cut = cuts['leading_jet_pt']*1e3
        return hist['leadingJetPt'].slice(ljpt_cut, inf)['met'].slice(
            met_cut, inf)

    figure = Figure(figsize=(9,8))
    canvas = FigCanvas(figure)
    ax = figure.add_subplot(1,1,1)
    control_regions = config['control_regions']
    n = len(control_regions) + 1
    ind = np.arange(n)    # the x locations for the groups
    width = 0.80
    bottoms = np.zeros(n)
    leg_entries = []
    for bg in config['backgrounds']: 
        y_vals = np.zeros(n)
        sr_count = get_count(hists[bg, variable, signal_region], cuts)
        y_vals[0] = sr_count
        this_color = type_dict[bg].color
        for reg_n, cr in enumerate(control_regions,1): 
            y_vals[reg_n] = get_count(hists[bg, variable, cr], cr_cuts)
        bar = ax.bar(ind, y_vals, width, bottom=bottoms, color=this_color)
        bottoms += y_vals
        leg_entries.append( (bar[0], bg))

    if sig_pt: 
        y_vals = np.zeros(n)
        sr_count = get_count(hists[sig_pt, variable, signal_region], cuts)
        y_vals[0] = sr_count
        color = 'black'
        for reg_n, cr in enumerate(control_regions,1): 
            y_vals[reg_n] = get_count(hists[sig_pt, variable, cr], cr_cuts)
        bar = ax.bar(ind, y_vals, width, color='black', fill=False, 
                     linestyle='dashed', linewidth=2)
        leg_entries.append( (bar[0], sig_pt))
    
    d_vals = np.zeros(n - 1)
    d_x = np.arange(1,n)
    for reg_n, cr in enumerate(control_regions): 
        d_vals[reg_n] = get_count(hists['data',variable,cr], cr_cuts)
    low, high = poisson_interval(d_vals)
    y_errors = [high - d_vals, d_vals - low]
    ax.errorbar(d_x + width/2, d_vals, fmt='ok', xerr=width/2, yerr=y_errors)

    ax.set_xticks(ind + width/2)
    ax.set_xticklabels([signal_region] + control_regions)
    ax.set_ylabel('$n$ events')
    if max_val: 
        ax.set_ylim(0, max_val)
    leg = ax.legend(*zip(*reversed(leg_entries)), 
                     loc='upper left', framealpha=0.0)
    canvas.print_figure(save_name, bbox_inches='tight')

