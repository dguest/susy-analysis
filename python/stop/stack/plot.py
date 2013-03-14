import matplotlib.pyplot as plt
from stop.hists import Hist1d
from stop import style, hists
from os.path import isdir, join
from stop.stack.stack import Stack
import os

def make_plots(plots_dict, misc_info): 
    hist1_dict = {}
    converter = HistConverter(misc_info)
    converter.appended_evt_str = None
    for tup, hist in plots_dict.iteritems():
        hist1_dict[tup] = converter.hist1_from_histn(*tup, histn=hist)

    printer = PlotPrinter(misc_info)
    printer.print_plots(*sort_data_mc(hist1_dict))
    printer.log = False
    printer.print_plots(*sort_data_mc(hist1_dict))
    

class HistConverter(object): 
    """
    convert from NdHist to Hist1d. 
    """
    def __init__(self, misc_info): 
        self.lumi_fb = misc_info['lumi_fb']
        self.appended_evt_str = r': {:.1f} Evt'

    def hist1_from_histn(self, physics, variable, cut, histn): 
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
            var_sty = style.VariableStyle(base_var, units)
        x_ax_lab = var_sty.axis_label
            
        x_ax_full_label = r' '.join(variable.split('/')[:-1] + [x_ax_lab])
    
        hist = Hist1d(y_vals, extent)
        n_center_bins = len(y_vals) - 2 
        if n_center_bins > 50 and n_center_bins % 4 == 0: 
            hist.rebin(4)
        hist.x_label = x_ax_full_label
        hist.y_label = style.event_label(self.lumi_fb)
        try: 
            hist.color = style.type_dict[physics].color
            hist.title = style.type_dict[physics].tex
        except KeyError: 
            hist.color = 'k'
            hist.title = physics
        if self.appended_evt_str: 
            hist.title += self.appended_evt_str.format(float(hist))
        return hist
    
def sort_data_mc(hist1_dict): 
    stack_data = {}
    all_cuts = set(c for p,v,c in hist1_dict.keys())
    variables = set(v for p,v,c in hist1_dict.keys())
    lists = {(v,c):[] for v in variables for c in all_cuts}
    signals = {(v,c):[] for v in variables for c in all_cuts}
    for threetup, hist in hist1_dict.iteritems(): 
        physics_type, variable, cut = threetup
        tup = (variable, cut)
        if physics_type == 'data': 
            if tup in stack_data: 
                raise ValueError('doubling the data')
            stack_data[(variable, cut)] = hist
        elif physics_type.startswith('stop'): 
            signals[(variable,cut)].append(hist)
        else: 
            lists[(variable, cut)].append(hist)

    for tup in lists: 
        lists[tup].sort()

    return stack_data, lists, signals

class PlotPrinter(object): 
    def __init__(self, options): 
        self.plot_dir = options['base_dir']
        self.ext = options['output_ext']
        self.log = True
        self.verbose = True

    def print_plots(self, stack_data, stack_mc_lists, signal_hists={}): 
        plot_dir = self.plot_dir
        if not isdir(plot_dir): 
            os.mkdir(plot_dir)
        for id_tup in stack_mc_lists.keys(): 
            variable, cut = id_tup
            stack_name = '_'.join(
                [variable.replace('/','-'), cut.replace('_','-')])
            if self.log: 
                stack_name += '_log'
            save_base = join(plot_dir, stack_name)
            save_name = save_base + self.ext
            if self.verbose: 
                print 'making {}'.format(save_name)
            stack = Stack(stack_name)
            if self.log: 
                stack.y_min = 0.1
                stack.ax.set_yscale('log')
            stack.add_backgrounds(stack_mc_lists[id_tup])
            if id_tup in signal_hists: 
                stack.add_signals(signal_hists[id_tup])
            if id_tup in stack_data: 
                stack.add_data(stack_data[id_tup])
            stack.add_legend()
            stack.save(save_name)
            stack.close()
