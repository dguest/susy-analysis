"""
Top level drawing routines for stack plots.
"""

from scharm import style, hists
from scharm.schema import wt2_ext, sys2_ext
from scharm.limits.limitsty import reg_names
from os.path import isdir, join
from scharm.aggregate.draw import Stack, Hist2d, Hist1d
import os
import numpy as np
from itertools import chain
from concurrent.futures import ProcessPoolExecutor as Executor

def make_plots(plots_dict, misc_info, log=False, mu_dict={}, blind=False):
    hist1_dict = {}
    hist2_dict = {}
    converter = HistConverter(misc_info)
    for tup, hist in plots_dict.items():
        proc, var, reg = tup
        if proc == 'data' and blind:
            continue
        if var.endswith(wt2_ext) or var.endswith(sys2_ext):
            continue
        sys_tup = (proc, var + sys2_ext, reg)
        stat_tup = (proc, var + wt2_ext, reg)
        for addr in [tup, sys_tup, stat_tup]:
            if addr in plots_dict:
                hist1_dict.update(converter.h1dict_from_histn(
                        addr, histn=plots_dict[addr]))
        hist2_dict.update(converter.h2dict_from_histn(tup, histn=hist))

    printer = StackPlotPrinter(misc_info, log=log, mu_dict=mu_dict)
    printer.print_plots(*sort_data_mc(hist1_dict))
    h2print = H2Printer(misc_info)
    h2print.print_plots(*sort_data_mc(hist2_dict))


class HistConverter:
    """
    convert from NdHist to several Hist1d or Hist2d.
    """
    def __init__(self, misc_info):
        self.lumi_fb = misc_info['lumi_fb']
        show_events = misc_info.get('show_events', True)
        theme = misc_info['theme']
        self._style = style.get_type_dict(theme)

    def h1dict_from_histn(self, pvc, histn):
        """
        for hists with any number of dims.
        If n_dim > 1, project along each axis.
        """
        physics, variable, cut = pvc
        hdict = {}
        if len(histn.axes) == 1:
            # for the simple case call the simple 1d formatter
            hdict[pvc] = self._hist1_from_histn(pvc, histn=histn)
            return hdict
        for ax_name, axis in histn.axes.items():
            xy_tup = histn.project_1d(axis.name)
            subvar = '-'.join([variable,axis.name])
            hdict[physics, subvar, cut] = self._get_hist1(
                xy_tup, axis.units , pvc)
        return hdict

    def _hist1_from_histn(self, pvc, histn):
        physics, variable, cut = pvc
        is_signal = physics.startswith('scharm')
        if physics not in self._style and not is_signal:
            raise ValueError("what the fuck is {}?".format(pvc))

        assert len(histn.axes) == 1
        xy_tup = histn.project_1d()
        ax = histn[0]
        pars = ax.parameters
        sel = None
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
        """does all the formatting / axis scaling"""
        y_vals, extent = xy_tup
        physics, variable, cut = pvc
        x_label, extent, x_units = self._get_axislabel_extent(
            variable, extent, units)
        if selection:
            nada, selection, zilch = self._get_axislabel_extent(
                variable, selection, units)

        hist = Hist1d(y_vals, extent, x_label=x_label, x_units=x_units,
                      y_label='Events')
        crop_var = variable
        for ext in wt2_ext, sys2_ext:
            if variable.endswith(ext):
                crop_var = variable[:-len(ext)]
        if (cut, crop_var) in style.crop_region_vars:
            hist.crop(*style.crop_region_vars[cut, crop_var])
        elif crop_var in style.crop_vars:
            hist.crop(*style.crop_vars[crop_var])
        hist.selection = selection
        max_bins = style.rebinning.get(cut,30)
        hist.rebin(max_bins=max_bins)
        hist.process = physics
        try:
            hist.color = self._style[physics].color
            hist.title = self._style[physics].tex
        except KeyError:
            hist.color = None
            hist.title = physics
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
        lists[tup].sort(key=float)

    for tup in signals:
        signals[tup].sort(key=str)

    return stack_data, lists, signals

def _get_lightness(hist1d):
    """returns the sum of the of the histogram RGB, useful for sorting"""
    from matplotlib.colors import colorConverter
    return sum(colorConverter.to_rgb(hist1d.color))

# __________________________________________________________________________
# 1d plot printer

class StackPlotPrinter:
    """
    Makes all the stack plots. Uses an Executor to multiprocess.
    """
    def __init__(self, options, log=False, mu_dict={}):
        self.plot_dir = options['base_dir']
        self.ext = options['output_ext']
        self.lumi = options['lumi_fb']
        self.serial = options.get('serial', False)
        self._show_counts = options['show_event_counts']
        self.log = log
        self.verbose = True
        theme = options['theme']
        self._selection_colors = style.get_selection_color(theme)
        self._signal_colors = style.get_signal_colors(theme)
        self._mu_dict = mu_dict

    def _info_generator(self, data, mc, signal):
        keys = []
        for key in mc.keys():
            bads = [key[0].endswith(wt2_ext), key[0].endswith(sys2_ext)]
            if not any(bads):
                keys.append(key)

        for id_tup in keys:
            def ap_var(suffix):
                return id_tup[0] + suffix, id_tup[1]
            obj = StackInfo()

            obj.variable, obj.cut = id_tup
            obj.bgs = mc[id_tup]
            obj.wt2 = mc.get(ap_var(wt2_ext), {})
            obj.syst2 = mc[ap_var(sys2_ext)]
            obj.signals = signal.get(id_tup)
            obj.data = data.get(id_tup)

            obj.plot_dir = self.plot_dir
            obj.log = self.log
            obj.ext = self.ext
            obj.verbose = self.verbose
            obj.signal_colors = self._signal_colors
            obj.lumi = self.lumi
            obj.selection_colors = self._selection_colors
            obj.mu_dict = self._mu_dict
            obj.show_counts = self._show_counts
            save_dir = join(self.plot_dir, obj.cut)
            if not isdir(save_dir):
                os.makedirs(save_dir)
            yield obj

    def print_plots(self, stack_data, stack_mc_lists, signal_hists={}):
        """
        All inputs should be dicts keyed by (variable, selection).

        signal_hists and stack_mc_lists are lists of Hist1d, stack_data
        is one Hist1d.
        """
        gen = self._info_generator(stack_data, stack_mc_lists, signal_hists)
        if not self.serial:
            with Executor() as exe:
                exe.map(_print_plot, gen)
        else:
            for obj in gen:
                _print_plot(obj)

class StackInfo:
    """Minimal class, just holds info for a plot"""
    def scale(self):
        """dummy scale function does nothing, can be monkey patched"""
        pass

    def get_save_name(self):
        variable, cut = self.variable, self.cut
        save_dir = join(self.plot_dir, cut)
        save_base = join(save_dir, variable)
        if self.log:
            save_base += '_log'
        if self.mu_dict:
            save_base += '_afterFit'
        return save_base + self.ext

def _scale_stack_info(info):
    """Scales the StackInfo object in place"""
    new = info
    if not new.mu_dict:
        return
    def scale(hist, sumsq=False):
        mu_scale, mu_err = new.mu_dict.get(hist.process, (None, None))
        if mu_scale is not None:
            if sumsq:
                mu_scale **= 2
            hist.scale(mu_scale)
    for bg in new.bgs:
        scale(bg)
    for wt2 in new.wt2:
        scale(wt2, True)
    for syst2 in new.syst2:
        scale(syst2, True)

StackInfo.scale = _scale_stack_info

# __________________________________________________________________________
# printing function
def _print_plot(obj):
    """the actual printing function for StackPlotPrinter"""
    obj.scale()
    save_name = obj.get_save_name()
    has_ratio = bool(obj.data)
    if obj.verbose:
        if has_ratio:
            print('making ratio {}'.format(save_name))
        else:
            print('making {}'.format(save_name))
    stack = Stack(ratio=has_ratio, selection_colors=obj.selection_colors)
    stack.colors = obj.signal_colors
    stack.lumi = obj.lumi
    stack.region_name = reg_names.get(obj.cut)
    stack.show_counts = obj.show_counts
    if obj.log:
        stack.y_min = 0.1
        stack.ax.set_yscale('log')
    stack.add_backgrounds(obj.bgs)
    if obj.signals:
        stack.add_signals(obj.signals)
    if obj.data:
        stack.add_data(obj.data)
    # add the mc error bars
    if obj.syst2:
        stack.add_syst2(obj.syst2)
    if obj.wt2:
        stack.add_wt2(obj.wt2)
        if obj.syst2:
            stack.add_total2(obj.syst2 + obj.wt2)
    stack.add_legend()
    if not isdir(obj.plot_dir):
        os.makedirs(obj.plot_dir)
    style.customize_stack(stack, obj.variable)
    stack.save(save_name)

# __________________________________________________________________________
# 2d plot printer

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



