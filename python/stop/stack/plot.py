import matplotlib.pyplot as plt
import numpy as np
from stop.hists import Hist1d
from stop import style, hists
from os.path import isdir, join
from stop.stack.draw import Stack, Hist2d
import os
from itertools import chain

def make_plots(plots_dict, misc_info): 
    hist1_dict = {}
    hist2_dict = {}
    converter = HistConverter(misc_info)
    converter.appended_evt_str = None
    for tup, hist in plots_dict.iteritems():
        hist1_dict.update(converter.h1dict_from_histn(tup, histn=hist))
        hist2_dict.update(converter.h2dict_from_histn(tup, histn=hist))

    # printer = StackPlotPrinter(misc_info)
    # printer.print_plots(*sort_data_mc(hist1_dict))
    # printer.log = True
    # printer.print_plots(*sort_data_mc(hist1_dict))
    h2print = H2Printer(misc_info)
    h2print.print_plots(*sort_data_mc(hist2_dict))
    

class HistConverter(object): 
    """
    convert from NdHist to several Hist1d or Hist2d. 
    """
    def __init__(self, misc_info): 
        self.lumi_fb = misc_info['lumi_fb']
        self.appended_evt_str = r': {:.1f} Evt'

    def h1dict_from_histn(self, pvc, histn): 
        physics, variable, cut = pvc
        hdict = {}
        if len(histn.axes) == 1: 
            hdict[pvc] = self.hist1_from_histn(pvc, histn=histn)
            return hdict
        for axis in histn.axes: 
            y_vals, extent = histn.project_1d(axis.name)
            subvar = '-'.join([variable,axis.name])
            hdict[(physics, subvar, cut)] = self._get_hist1(
                y_vals, extent, axis.units , pvc)
        return hdict

    def hist1_from_histn(self, pvc, histn): 
        physics, variable, cut = pvc
        if physics not in style.type_dict and not physics.startswith('stop'):
            raise ValueError("what the fuck is {}?".format(pvc))
    
        assert len(histn.axes) == 1
        y_vals, extent = histn.project_1d('x')
        units = histn.axes[0].units
        return self._get_hist1(y_vals, extent, units, 
                               (physics, variable, cut))

    def h2dict_from_histn(self, pvc, histn): 
        axes = histn.axes
        nax = len(axes)
        h2dict = {}
        physics, var, cut = pvc
        for x in xrange(nax): 
            xname = axes[x].name
            for y in xrange(x): 
                yname = axes[y].name
                varname = '-'.join([var,xname,yname])
                imdict = histn.project_imshow(xname, yname)
                xvar = ' '.join([var, xname])
                xlab, xext = self._get_axislabel_extent(
                    xvar, axes[x].extent, axes[x].units)
                yvar = ' '.join([var, yname])
                ylab, yext = self._get_axislabel_extent(
                    yvar, axes[y].extent, axes[y].units)
                imdict['extent'] = np.fromiter(chain(xext, yext),np.float)
                subvar = '-'.join([var,yname,'vs',xname])
                h2dict[(physics,subvar,cut)] = Hist2d(imdict, xlab, ylab)
                
        return h2dict

    def _get_axislabel_extent(self, variable, extent, units): 
        base_var = variable.split('/')[-1]
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
        return x_ax_full_label, extent

    def _get_hist1(self, y_vals, extent, units, pvc): 
        physics, variable, cut = pvc
        x_ax_full_label, extent = self._get_axislabel_extent(
            variable, extent, units)
    
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

class StackPlotPrinter(object): 
    def __init__(self, options): 
        self.plot_dir = options['base_dir']
        self.ext = options['output_ext']
        self.log = False
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

class H2Printer(object): 
    def __init__(self, options): 
        self.plot_dir = options['base_dir']
        self.ext = options['output_ext']
        self.verbose = True
        
    def print_plots(self, data, mc, signal={}): 
        if not isdir(self.plot_dir): 
            os.mkdir(self.plot_dir)
        for id_tup in mc.keys(): 
            variable, cut = id_tup
            stack_name = '_'.join([
                    variable.replace('/','-'), cut.replace('_','-'), 
                    '{}'])
            save_base = join(self.plot_dir, stack_name) + self.ext
            if self.verbose: 
                print 'making {}'.format(save_base)
            data_name = save_base.format('data')
            if id_tup in data: 
                data[id_tup].save(data_name)
            sig_name = save_base.format('signal')
            mc_name = save_base.format('bg')
            if id_tup in mc: 
                mclist = mc[id_tup]
                bg_total = sum(mclist[1:], mclist[0])
                bg_total.save(mc_name)
