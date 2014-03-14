"""
Top level drawing routines for stack plots.
"""

import numpy as np
from scharm import style, hists
from os.path import isdir, join
from scharm.aggregate.draw import Stack, Hist2d, Hist1d
import os
from itertools import chain

def make_plots(plots_dict, misc_info, log=False): 
    hist1_dict = {}
    hist2_dict = {}
    converter = HistConverter(misc_info)
    converter.appended_evt_str = None
    for tup, hist in plots_dict.items():
        hist1_dict.update(converter.h1dict_from_histn(tup, histn=hist))
        hist2_dict.update(converter.h2dict_from_histn(tup, histn=hist))

    printer = StackPlotPrinter(misc_info)
    printer.log = log
    printer.print_plots(*sort_data_mc(hist1_dict))
    h2print = H2Printer(misc_info)
    h2print.log = log
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
        for ax_name, axis in histn.axes.items(): 
            xy_tup = histn.project_1d(axis.name)
            subvar = '-'.join([variable,axis.name])
            hdict[physics, subvar, cut] = self._get_hist1(
                xy_tup, axis.units , pvc)
        return hdict

    def hist1_from_histn(self, pvc, histn): 
        physics, variable, cut = pvc
        is_signal = physics.startswith('scharm')
        if physics not in style.type_dict and not is_signal:
            raise ValueError("what the fuck is {}?".format(pvc))
    
        assert len(histn.axes) == 1
        xy_tup = histn.project_1d()
        ax = histn[0]
        pars = ax.parameters
        if 'selection_min' in pars: 
            sel = tuple(pars['selection_' + m] for m in ['min','max'])
        return self._get_hist1(xy_tup, ax.units, pvc, sel)

    def h2dict_from_histn(self, pvc, histn): 
        axes = list(histn.axes.values())
        nax = len(axes)
        h2dict = {}
        physics, var, cut = pvc
        for x_n in range(nax): 
            x_axis = axes[x_n]
            xname = x_axis.name
            for y_n in range(x_n): 
                y_axis = axes[y_n]
                yname = y_axis.name
                varname = '-'.join([var,xname,yname])
                imdict = histn.project_imshow(xname, yname)
                xvar = ' '.join([var, xname])
                xlab, xext, xunit = self._get_axislabel_extent(
                    xvar, x_axis.extent, x_axis.units)
                yvar = ' '.join([var, yname])
                ylab, yext, yunit = self._get_axislabel_extent(
                    yvar, y_axis.extent, y_axis.units)
                imdict['extent'] = np.fromiter(chain(xext, yext),np.float)
                subvar = '-'.join([var,yname,'vs',xname])
                hist2 = Hist2d(imdict, xlab, ylab)
                hist2.title = physics # <- bit of a monkey patch, should fix
                h2dict[physics,subvar,cut] = hist2
                
        return h2dict

    def _get_axislabel_extent(self, variable, extent, units): 
        base_var = variable.split('/')[-1]
        if units == 'MeV': 
            extent = [e / 1000.0 for e in extent]
            units = 'GeV'
        var_sty = style.get_variable_style(base_var, units)
        x_ax_lab = var_sty.tex_name
        return x_ax_lab, extent, var_sty.units

    def _get_hist1(self, xy_tup, units, pvc, selection=None): 
        y_vals, extent = xy_tup
        physics, variable, cut = pvc
        x_label, extent, x_units = self._get_axislabel_extent(
            variable, extent, units)
        if selection: 
            nada, selection, zilch = self._get_axislabel_extent(
                variable, selection, units)
    
        hist = Hist1d(y_vals, extent, x_label=x_label, x_units=x_units,
                      y_label='Events')
        hist.selection = selection
        n_center_bins = len(y_vals) - 2 
        if n_center_bins > 50 and n_center_bins % 4 == 0: 
            hist.rebin(4)
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
    for threetup, hist in hist1_dict.items(): 
        physics_type, variable, cut = threetup
        tup = (variable, cut)
        if physics_type == 'data': 
            if tup in stack_data: 
                raise ValueError('doubling the data')
            stack_data[variable, cut] = hist
        elif physics_type.startswith('scharm'): 
            signals[variable,cut].append(hist)
        else: 
            lists[variable, cut].append(hist)

    for tup in lists: 
        try: 
            lists[tup].sort(key=float)
        except TypeError: 
            pass

    return stack_data, lists, signals

class StackPlotPrinter(object): 
    def __init__(self, options): 
        self.plot_dir = options['base_dir']
        self.ext = options['output_ext']
        self.lumi = options['lumi_fb']
        self.log = False
        self.verbose = True

    def print_plots(self, stack_data, stack_mc_lists, signal_hists={}): 
        plot_dir = self.plot_dir
        for id_tup in stack_mc_lists.keys(): 
            variable, cut = id_tup
            save_dir = join(plot_dir, cut)
            save_base = join(save_dir, variable)
            if self.log: 
                save_base += '_log'
            save_name = save_base + self.ext
            has_ratio = id_tup in stack_data
            if self.verbose: 
                if has_ratio: 
                    print('making ratio {}'.format(save_name))
                else: 
                    print('making {}'.format(save_name))
            stack = Stack(ratio=has_ratio)
            stack.lumi = self.lumi
            if self.log: 
                stack.y_min = 0.1
                stack.ax.set_yscale('log')
            stack.add_backgrounds(stack_mc_lists[id_tup])
            if id_tup in signal_hists: 
                stack.add_signals(signal_hists[id_tup])
            if id_tup in stack_data: 
                stack.add_data(stack_data[id_tup])
            stack.add_legend()
            if not isdir(save_dir): 
                os.makedirs(save_dir)
            stack.save(save_name)

class H2Printer(object): 
    def __init__(self, options): 
        self.plot_dir = options['base_dir']
        self.ext = options['output_ext']
        self.verbose = True
        self.log = False
        
    def print_plots(self, data, mc, signal={}): 
        if not isdir(self.plot_dir): 
            os.mkdir(self.plot_dir)
        for id_tup in mc.keys(): 
            variable, cut = id_tup
            stack_name = '_'.join([
                    variable.replace('/','-'), cut.replace('_','-'), 
                    '{}'])
            save_base = join(self.plot_dir, stack_name) + self.ext
            if id_tup in data: 
                data_name = save_base.format('data')
                if self.verbose: 
                    print('making {}'.format(data_name))
                data[id_tup].save(data_name)
            if id_tup in signal: 
                for hist in signal[id_tup]: 
                    # NOTE: the name is monkey patched in above.. 
                    # should fix this 
                    sig_name = save_base.format(hist.title)
                    if self.verbose: 
                        print('making {}'.format(sig_name))
                    hist.save(sig_name)
            mc_name = save_base.format('bg')
            if id_tup in mc: 
                mclist = mc[id_tup]
                bg_total = sum(mclist[1:], mclist[0])
                if self.verbose: 
                    print('making {}'.format(mc_name))
                bg_total.save(mc_name, log=self.log)

        
    
