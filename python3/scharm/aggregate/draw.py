import matplotlib.pyplot as plt
from matplotlib.colors import LogNorm, Normalize
from matplotlib.gridspec import GridSpec
from matplotlib.ticker import MaxNLocator, LogLocator
from matplotlib.ticker import LogFormatterMathtext, LogFormatter
import numpy as np
from itertools import chain
from scharm import errorbars
from warnings import warn
from matplotlib.backends.backend_agg import FigureCanvasAgg as FigureCanvas
from matplotlib.figure import Figure
from mpl_toolkits.axes_grid1 import make_axes_locatable
from os.path import join, isdir, dirname
import os

class Stack(object): 
    """
    This is for drawing. 
    """
    def __init__(self, ratio=False, exclude_zeros=True): 
        self._exclude_zeros = exclude_zeros
        self.fig = Figure(figsize=(8,6))
        self.canvas = FigureCanvas(self.fig)
        if not ratio:
            self.ax = self.fig.add_subplot(1,1,1)
            self.ratio = None
        else: 
            grid = GridSpec(2,1, height_ratios=[3,1])
            self.ax = self.fig.add_subplot(grid[0])
            self.ratio = self.fig.add_subplot(grid[1],sharex=self.ax)
            self.ratio.set_ylabel('Data / SM')# y=0.98, va='top')
            locator = MaxNLocator(5, prune='upper') 
            self.ratio.get_yaxis().set_major_locator(locator)
        self._selection = None
        self._y_sum_step = 0.0
        self._y_sum = 0.0
        self.colors = 'mky'
        self.y_min = None
        self._proxy_legs = []
        self._bg_proxy_legs = []
        self.ratio_max = 2.0
        self._x_limits = None
        
    def _set_xlab(self, name): 
        if self.ratio: 
            axes = self.ratio
            plt.setp(self.ax.get_xticklabels(), visible=False)
        else: 
            axes = self.ax
        axes = self.ax if not self.ratio else self.ratio
        xlabel = axes.get_xlabel()
        if xlabel and xlabel != name: 
            raise PlottingError(
                "tried to plot xaxis {} on {}, "
                "probably don't want that".format(name, xlabel))

        if not xlabel: 
            axes.set_xlabel(name, x=0.98, ha='right')

    def _set_xlims(self, hist): 
        xval = hist.get_xy_step_pts()[0]
        xmin = xval.min()
        xmax = xval.max()
        if not self._x_limits: 
            self._x_limits = (xmin,xmax)
        else: 
            pass                # add check!
        
    def add_backgrounds(self, hist_list): 
        last_plot = 0
        color_itr = iter(self.colors)
        if self.y_min is not None: 
            last_plot = self.y_min
        for hist in hist_list:
            x_vals, y_vals = hist.get_xy_step_pts()

            self._set_xlims(hist)

            ylabel = self.ax.get_ylabel()
            if not ylabel: 
                self.ax.set_ylabel(hist.y_label, y=0.98,ha='right')
            self._set_xlab(hist.x_label)

            fill_color = hist.color
            if not fill_color: 
                fill_color = next(color_itr)

            self._y_sum_step += y_vals
            self._y_sum += hist.get_xy_center_pts()[1]

            tmp_sum = np.array(self._y_sum_step[:])
            if self.y_min is not None: 
                tmp_sum[tmp_sum < self.y_min] = self.y_min

            self.ax.fill_between(x_vals, tmp_sum, last_plot, 
                                 facecolor=fill_color)
            proxy = plt.Rectangle((0, 0), 1, 1, fc=fill_color, 
                                  label=hist.title)
            if hist.selection: 
                self._add_selection(*hist.selection)
            self._bg_proxy_legs.append( (proxy,hist.title)) 

            last_plot = tmp_sum

    def add_signals(self, hist_list): 
        color_itr = iter(self.colors)
        for hist in hist_list: 
            x_vals, y_vals = hist.get_xy_step_pts()
            if self.y_min is not None: 
                y_vals[y_vals < self.y_min] = self.y_min

            if hist.color: 
                color = hist.color
            else: 
                color = next(color_itr)
            style = color + '--'
            plt_handle, = self.ax.plot(x_vals,y_vals,style, linewidth=2.0)
            self._proxy_legs.append( (plt_handle, hist.title))

    def _get_min_plotable(self, y_vals): 
        plot_vals = np.array(y_vals)
        if self.y_min is not None: 
            bad_y_vals = y_vals <= self.y_min
            plot_vals[bad_y_vals] = self.y_min*1.001
        return plot_vals

    def _add_selection(self, low, high): 
        if self._selection: 
            if low != self._selection[0] or high != self._selection[1]: 
                raise ValueError('multiple incompatible selections')
        self._selection = (low, high)

    def _draw_selection(self): 
        if not self._selection: 
            return 
        low, high = self._selection
        fill_args = dict(facecolor=(0.9, 0, 0, 0.2), linestyle=None)
        line_args = dict(color='r', linewidth=2)
        inf = float('inf')
        xlow, xhigh = self._x_limits
        if low != -inf:
            self.ax.axvspan(xlow, low, **fill_args)
            self.ax.axvline(low, **line_args)
        if high != inf: 
            self.ax.axvspan(high, xhigh, **fill_args)
            self.ax.axvline(high, **line_args)
        
    def _add_ratio(self, x_vals, y_vals, lows, highs): 
        """
        Adds a ratio plot to the bottom axes, showing data / simulated
        """
        ratable = (self._y_sum > 0.0)
        if self._exclude_zeros: 
            ratable &= (y_vals != 0.0)
        rat_x = x_vals[ratable]
        rat_y = y_vals[ratable]
        rat_y_sum = self._y_sum[ratable]
        y_ratios = rat_y / rat_y_sum

        # all ratio points must fall within the upper limit, if not move them
        y_ratios_high = np.minimum(
            self.ratio_max,highs[ratable] / rat_y_sum)
        y_ratios_low = lows[ratable] / rat_y_sum
        y_ratio_err_up = y_ratios_high - y_ratios
        y_ratio_err_down = y_ratios - y_ratios_low

        out_of_bounds = y_ratios > self.ratio_max
        in_bounds = ~out_of_bounds

        # plot the well behaved data first
        if np.any(in_bounds): 
            self.ratio.errorbar(
                rat_x[in_bounds], y_ratios[in_bounds], ms=10, fmt='k.', 
                yerr=[y_ratio_err_down[in_bounds], y_ratio_err_up[in_bounds]])

        # the points outside the ratio max are red 
        bound_y = np.minimum(self.ratio_max, y_ratios[out_of_bounds])
        bound_y_low = np.minimum(self.ratio_max, y_ratios_low[out_of_bounds])
        bound_up = y_ratios_high[out_of_bounds] - bound_y
        bound_down = bound_y - bound_y_low
        if np.any(out_of_bounds): 
            self.ratio.errorbar(
                rat_x[out_of_bounds], 
                bound_y, ms=10, fmt='r.', 
                yerr=[bound_down, bound_up])
            
            
        self.ratio.axhline(y=1, linestyle='--', color='k')
        self.ratio.axhline(y=0.5, linestyle=':', color='k')
        self.ratio.axhline(y=1.5, linestyle=':', color='k')

    def add_data(self, hist): 
        x_vals, y_vals = hist.get_xy_center_pts()
        lows, highs = errorbars.poisson_interval(y_vals)

        if self.ratio and np.any(self._y_sum): 
            self._add_ratio(x_vals, y_vals, lows, highs)

        self._set_xlims(hist)

        plt_x = x_vals
        plt_y = y_vals
        if self._exclude_zeros: 
            nonzero = np.nonzero(y_vals)
            plt_x = plt_x[nonzero]
            plt_y = plt_y[nonzero]
            lows = lows[nonzero]
            highs = highs[nonzero]
        
        plt_y = self._get_min_plotable(plt_y)
        plt_low = self._get_min_plotable(lows)
        plt_err_up = highs - plt_y
        plt_err_down = plt_y - plt_low

        if not np.any(plt_err_up): 
            return 

        line,cap,notsure = self.ax.errorbar(
            plt_x, plt_y, ms=10, fmt='k.', 
            yerr=[plt_err_down,plt_err_up])
            
        self._proxy_legs.append( (line, hist.title))
    
    def add_legend(self): 
        all_legs = chain(self._proxy_legs,reversed(self._bg_proxy_legs))
        proxies = zip(*all_legs)
        legend = self.ax.legend(*proxies, numpoints=1, ncol=2)
        if legend: 
            legend.get_frame().set_linewidth(0)
            legend.get_frame().set_alpha(0)
            
    def save(self, name):
        if self._x_limits is None: 
            return 

        self._draw_selection()
        self.ax.set_xlim(*self._x_limits)
        if self.ax.get_yscale() != 'log': 
            ymax = self.ax.get_ylim()[1]
            self.ax.set_ylim(0,ymax)

        self.fig.tight_layout(pad=0.3, h_pad=0.3, w_pad=0.3)

        if self.ratio: 
            self.ratio.set_ylim(0,self.ratio_max)
        self.canvas.print_figure(name)

class Hist1d(object): 
    """
    class to organize 1d hists from np arrays
    """
    
    def __init__(self, array, extent, 
                 x_label = '', y_label = '', x_units='', title=''): 
        self._array = array
        self.extent = extent
        self._x_label = x_label
        self._x_units = x_units
        self._y_label = y_label
        self.title = title
        self.color = ''
        self.selection = None

    def __float__(self): 
        return self._array.sum()
    def __cmp__(self, other): 
        this_val = float(self)
        other_val = float(other)
        return this_val - other_val
    def __str__(self): 
        return self.title
    def __add__(self, other): 
        diffs = [(x - y)**2 for x, y in zip(self.extent, other.extent)]
        if sum(diffs) / sum(x**2 for x in self.extent) > 1e-4: 
            raise ValueError(
                'tried to add a hist with extent {} to one with'
                ' extent {}'.format(self.extent, other.extent) )
        sum_array = self._array + other._array
        new_y = self._zipstring(self._y_label, other.y_label)
        new_x = self._zipstring(self._x_label, other.x_label)
        new_title = self._zipstring(self.title, other.title)
        new_hist = Hist1d(sum_array, self.extent, 
                          x_label=new_x, y_label=new_y, title=new_title)
        new_hist.color = self.color if self.color else other.color
        return new_hist
    def _zipstring(self, one, two): 
        return ''.join(c for c, o in zip(one, two) if c == o)
    
    @property
    def x_label(self): 
        if self._x_units: 
            return '{} [{}]'.format(self._x_label, self._x_units)
        return self._x_label

    @property
    def y_label(self): 
        bins = len(self._array) - 2
        x_per_bin = (self.extent[1] - self.extent[0]) / bins
        fm_string = '{} / {:.1f}'
        if self._x_units: 
            fm_string += ' {units}'
        return fm_string.format(
            self._y_label, x_per_bin, units=self._x_units)

    def scale(self, factor): 
        self._array = self._array * factor
    def rebin(self, n_bins): 
        center_bins = self._array[1:-1]
        n_old = len(center_bins)
        if not n_old % n_bins == 0: 
            raise ValueError("rebinning: {} doesn't go into {}".format(
                    n_bins, n_old))
        newcenter = center_bins.reshape((-1, n_bins)).sum(axis=1)
        newhist = np.zeros(n_old // n_bins + 2)
        newhist[0] = self._array[0]
        newhist[1:-1] = newcenter
        newhist[-1] = self._array[-1]
        self._array = newhist

    def crop(self, low=None, high=None): 
        warn("this doesn't do anything right now...", stacklevel=2)
        
    def average_bins(self, bins): 
        """
        either average bin range or all bins. 
        
        TODO: rework to average all bins between successive list entries. 
        """
        if isinstance(bins, int): 
            lbin = 1
            for hbin in range(bins + 1, self._array.size, bins): 
                ave_bins = np.arange(lbin, hbin)
                lbin = hbin
                self.average_bins(ave_bins)
        ave = np.average(self._array[bins])
        self._array[bins] = ave
    def get_xy_step_pts(self): 
        """
        returns xy pairs which can be fed to plt.plot() to generate a 
        step plot
        """
        y_vals = self._array[1:-1] # no overflow
        double_y_vals = np.dstack((y_vals,y_vals)).flatten()
        x_vals = np.linspace(*self.extent, num=len(y_vals) + 1)
        double_x_vals = np.dstack((x_vals,x_vals)).flatten()[1:-1]

        return double_x_vals, double_y_vals

    def get_xy_center_pts(self, exclude_zeros=True): 
        """
        returns points at the bin centers
        """
        y_vals = self._array[1:-1]
        n_pts = len(y_vals)*2 + 1
        x_vals = np.linspace(*self.extent, num=n_pts)[1:-1:2]
        return x_vals, y_vals


def h2_from_hn(in_hist): 
    if not len(in_hist) == 2: 
        raise PlottingError(
            "can't get a 2d hist from an {}d hist".format(len(in_hist)))

    def new_lim_unit(ax): 
        lims = [ax.min, ax.max]
        if ax.units == 'MeV': 
            return [lim / 1000.0 for lim in lims], 'GeV'
        else: 
            return lims, ax.units

    x_name, y_name = [ax.name for ax in in_hist.axlist()]
    x_lims, x_unit = new_lim_unit(in_hist[x_name])
    y_lims, y_unit = new_lim_unit(in_hist[y_name])
    array = in_hist.array[1:-1,1:-1]

    extent = list(x_lims) + list(y_lims)
    im_dict =  {
        'X': array.T, 
        'extent': extent, 
        'aspect': 'auto', 
        'interpolation':'nearest', 
        'origin':'lower', 
        }
    def mklabel(base, unit): 
        if unit: 
            return '{} [{}]'.format(base, unit)
        return base
    return Hist2d(im_dict, mklabel(x_name, x_unit), mklabel(y_name, y_unit))

class Hist2d(object): 
    def __init__(self, imdict, xlabel, ylabel): 
        self.imdict = imdict
        self.x_label = xlabel
        self.y_label = ylabel
        self.cb_label = ''

    def __add__(self, other): 
        assert self.x_label == other.x_label
        assert self.y_label == other.y_label
        xext = self.imdict['extent']
        yext = other.imdict['extent']
        rel_diff = ((xext - yext)**2).sum() / ((xext + yext)**2).sum()
        if rel_diff > 0.001: 
            raise ValueError('hist extent mismatch')
        out_im = { 
            'X': self.imdict['X'] + other.imdict['X'], 
            'extent': xext}
        out_im.update(
            {k:v for k,v in self.imdict.items() if k not in out_im})
        return Hist2d(out_im, self.x_label, self.y_label)

    def save(self, name, log=False, vrange=None): 
        if self.imdict['X'].sum() == 0.0 and log: 
            warn("can't plot {}, in log mode".format(name), RuntimeWarning, 
                 stacklevel=2)
            return 
        fig = Figure(figsize=(8,6))
        canvas = FigureCanvas(fig)
        ax = fig.add_subplot(1,1,1)
        divider = make_axes_locatable(ax)
        cax = divider.append_axes("right", "5%", pad="1.5%")
        if log: 
            norm=LogNorm()
        else: 
            norm=Normalize()
        if vrange: 
            self.imdict['vmin'], self.imdict['vmax'] = vrange
        im = ax.imshow(norm=norm,**self.imdict)
        cb_dict = {'cax':cax}
        if log: 
            cb_dict['ticks'] = LogLocator(10, np.arange(0.1,1,0.1))
            cb_dict['format'] = LogFormatterMathtext(10)
            
        cb = plt.colorbar(im, **cb_dict)

        ax.set_xlabel(self.x_label, x=0.98, ha='right')        
        ax.set_ylabel(self.y_label, y=0.98, ha='right')
        if self.cb_label: 
            cb.set_label(self.cb_label, y=0.98, ha='right')
        canvas.print_figure(name, bbox_inches='tight')

class PlottingError(Exception): 
    def __init__(self, problem): 
        super(PlottingError,self).__init__(problem)


# ============================================
# this should be made into a class
# ============================================


def tagger_plot_for_jet_number(plots_dict, jetn, signal_point, ext, 
                               out_dir): 

    hist_name = 'jet{}/taggerWeights'.format(jetn)
    key = (signal_point, hist_name, 'preselection')
    hist = plots_dict[key]
    x_name, y_name = (ax for ax in hist)
    hist2d = Hist2d(hist.project_imshow(x_name, y_name), x_name, y_name)
    if not isdir(out_dir):
        os.mkdir(out_dir)
    out_title = 'jet{}-wt-{}{}'.format(jetn, signal_point, ext)
    hist2d.save(join(out_dir, out_title), log=True)

def tagger_overlay_plot_for_jet_number(plots_dict, jetn, signal_point, 
                                       ext, out_dir): 
    color_groups = { 
        'red': ['ttbar', 't'], 
        'green': [signal_point], 
        'blue': ['Wjets','Zjets'], 
        }

    def get_im_dict(physics, jet_number): 
        hist_name = 'jet{}/taggerWeights'.format(jet_number)
        key = (physics, hist_name, 'preselection')
        hist = plots_dict[key]
        x_name, y_name = (ax for ax in hist)
        return hist.project_imshow(x_name, y_name)

    phys_dicts = {}
    for phys in chain(*color_groups.values()): 
        phys_dicts[phys] = get_im_dict(phys, jetn)
    
    color_arrays = {k:None for k in color_groups}
    for color, physics_types in color_groups.items(): 
        for phys in physics_types: 
            if color_arrays[color] is None: 
                color_arrays[color] = phys_dicts[phys]['X']
            else: 
                color_arrays[color] += phys_dicts[phys]['X']

    stacked_array = np.dstack( 
        (color_arrays[color] for color in ['red','green','blue']))

    stacked_array = np.log(stacked_array + 1)
    for i in range(3): 
        the_max = stacked_array[:,:,i].max()
        stacked_array[:,:,i] /= the_max


    chop_factor = 0.5
    new_max = stacked_array.max()*chop_factor
    chop_vals = (stacked_array > new_max)
    stacked_array[chop_vals] = new_max
    stacked_array /= stacked_array.max()

    for color, array in color_arrays.items(): 
        print(color, array.sum())

    im_dic = phys_dicts[signal_point].copy()
    im_dic['X'] = stacked_array

    figure = Figure(figsize=(8,8))
    canvas = FigureCanvas(figure)
    ax = figure.add_subplot(1,1,1)
    ax.imshow(**im_dic)
    y_lims = ax.get_ylim()
    x_lims = ax.get_xlim()

    for color, phys_types in color_groups.items(): 
        label = ', '.join(phys_types)
        ax.plot(np.zeros(0), color[0] + '-', lw=10, label=label)
    ax.legend(loc='lower right')
    ax.set_ylim(*y_lims)
    ax.set_xlim(*x_lims)
    ax.set_xlabel(r'$\log(P_c/P_u)$', x=0.98, ha='right', fontsize=18)
    ax.set_ylabel(r'$\log(P_c/P_b)$', y=0.98, ha='right', fontsize=18)
    ax.axvline(-0.82, linestyle='--', color='red')
    ax.axhline(-1.0, linestyle='--', color='red')

    if not isdir(out_dir):
        os.mkdir(out_dir)
    out_title = 'jet{}-wtOverlay-{}{}'.format(jetn, signal_point, ext)
    canvas.print_figure(join(out_dir, out_title), bbox_inches='tight')

    # print stacked_array.max(0).max(0)
