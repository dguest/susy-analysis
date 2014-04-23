from h5py import File
import numpy as np
from collections import Counter
from os.path import dirname, isdir
import os
from scharm import style

def _is_signal(name):
    return len(name.split('-')) >= 3

def _get_counts(truth_group, region='SR-mll'):
    objects_of_interest = {'taggedJet{}'.format(x) for x in xrange(4)}
    objects_of_interest |= {'jet{}'.format(x) for x in xrange(4)}
    counts = Counter()
    for phys_obj_name in truth_group:
        if phys_obj_name in objects_of_interest:
            for flavor in truth_group[phys_obj_name]:
                regions = truth_group[phys_obj_name][flavor]['pt']
                if region in regions:
                    count = np.array(regions[region]).sum()
                    counts[phys_obj_name, flavor] = count
    return counts

def plot_truth(args):
    region = args.region
    use_bg = args.include_background
    total_counts = Counter()
    with File(args.histmill_file) as hfile:
        for physics in hfile:
            is_bg = not _is_signal(physics) and physics != 'data'
            is_sp = physics == args.signal_point
            include = is_bg if use_bg else is_sp
            if include:
                total_counts += _get_counts(
                    hfile[physics]['truth'], region)

    plot_name = region + '-bg' if args.include_background else region
    _draw_plots(total_counts, 'truthplt/{}{}'.format(plot_name, args.ext))

def _rename_tag(oldname):
    new, num = oldname.replace('taggedJet', 'Tag ').split()
    return ' '.join([new, str(int(num) + 1)])

def _draw_plots(total_counts, out_path):
    # cmap = {'bottom':'r', 'charm':'g', 'light':'b', 'tau':'pink'}
    cmap = style.type_dict
    objects, flavors = zip(*total_counts)
    tagged_obj = sorted({t for t in objects if t.startswith('tagged')})

    from matplotlib.figure import Figure
    from matplotlib.backends.backend_agg import FigureCanvasAgg as FigCanvas

    figure = Figure(figsize=(9,8))
    canvas = FigCanvas(figure)
    ax = figure.add_subplot(1,1,1)
    ax.tick_params(axis='both', which='major', labelsize=18)
    width = 0.8

    n_cols = len(tagged_obj)
    bottom = np.zeros(n_cols)
    x_vals = np.arange(n_cols)
    leg_entries = []
    print tagged_obj
    for flav in sorted({f for f in flavors}):
        color = cmap[flav].color
        y_vals = np.fromiter([total_counts[o,flav] for o in tagged_obj],'d')
        bar = ax.bar(x_vals, y_vals, width, bottom=bottom, color=color)
        bottom += y_vals
        leg_entries.append( (bar[0], flav ))
    ax.set_xticks(x_vals + width/2)
    ax.set_xticklabels([_rename_tag(t) for t in tagged_obj],
                       fontsize=18)
    ax.set_ylabel('$n$ jets', **style.vdict)

    leg = ax.legend(
        *zip(*leg_entries), loc='upper right', framealpha=0.8, fontsize=18)
    if not isdir(dirname(out_path)):
        os.mkdir(dirname(out_path))
    canvas.print_figure(out_path, bbox_inches='tight')

