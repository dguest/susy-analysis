import matplotlib.pyplot as plt
from stop.hists import Hist1d
from stop import style, hists
from os.path import isdir, join
from stop.stack.stack import Stack

def make_plots(plots_dict, args): 
    hist1_dict = {}
    for tup, hist in plots_dict.iteritems():
        hist1_dict[tup] = hist1_from_histn(*tup, histn=hist, 
                                            lumi_fb=args.lumi_fb)

    print_plots(args, *sort_data_mc(hist1_dict))

def hist1_from_histn(physics, variable, cut, histn, lumi_fb): 
    if physics not in style.type_dict and not physics.startswith('stop'):
        raise ValueError("what the fuck is {}?".format(pt))

    assert len(histn.axes) == 1
    y_vals, extent = histn.project_1d('x')
    base_var = variable.split('/')[-1]
    units = histn.axes[0].units
    if units == 'MeV': 
        extent = [e / 1000.0 for e in extent]
        units = 'GeV'
    if base_var in style.ax_labels: 
        var_sty = style.ax_labels[base_var]
        var_sty.units = units
    else: 
        var_sty = sytle.VariableStyle(base_var, units)
    x_ax_lab = var_sty.axis_label
        
    x_ax_full_label = r' '.join(variable.split('/')[:-1] + [x_ax_lab])

    hist = Hist1d(y_vals, extent)
    n_center_bins = len(y_vals) - 2 
    if n_center_bins > 50 and n_center_bins % 4 == 0: 
        hist.rebin(4)
    hist.x_label = x_ax_full_label
    hist.y_label = style.event_label(lumi_fb)
    try: 
        hist.color = style.type_dict[physics].color
        hist.title = style.type_dict[physics].tex
    except KeyError: 
        hist.color = 'k'
        hist.title = physics
    hist.title += r': {:.1f} Evt'.format(float(hist))
    return hist

def sort_data_mc(hist1_dict): 
    stack_data = {}
    all_cuts = set(c for p,v,c in hist1_dict.keys())
    variables = set(v for p,v,c in hist1_dict.keys())
    lists = {(v,c):[] for v in variables for c in all_cuts}
    signals = {}
    for threetup, hist in hist1_dict.iteritems(): 
        physics_type, variable, cut = threetup
        tup = (variable, cut)
        if physics_type == 'data': 
            if tup in stack_data: 
                raise ValueError('doubling the data')
            stack_data[(variable, cut)] = hist
        elif physics_type.startswith('stop'): 
            signals[(variable,cut)] = hist
        else: 
            lists[(variable, cut)].append(hist)

    for tup in lists: 
        lists[tup].sort()

    return stack_data, lists, signals


def print_plots(args, stack_data, stack_mc_lists, signal_hists={}): 

    plot_dir = args.outplot_dir
    if not isdir(plot_dir): 
        os.mkdir(plot_dir)
    for id_tup in stack_mc_lists.keys(): 
        variable, cut = id_tup
        stack_name = '_'.join([variable.replace('/','-'), cut])
        stack = Stack(stack_name)
        stack.y_min = 0.1
        stack.ax.set_yscale('log')
        stack.add_backgrounds(stack_mc_lists[id_tup])
        if id_tup in signal_hists: 
            stack.add_signals([signal_hists[id_tup]])
        save_base = join(plot_dir, stack_name)
        if save_base.endswith('_cr'): 
            stack.add_data(stack_data[id_tup])
        else: 
            print '{} is blinded, not showing data'.format(save_base)
        stack.add_legend()
        save_name = save_base + args.output_ext
        stack.save(save_name)
        stack.close()
