import matplotlib.pyplot as plt
from matplotlib.colors import LogNorm, Normalize
from matplotlib.gridspec import GridSpec
from matplotlib.ticker import MaxNLocator, LogLocator
from matplotlib.ticker import LogFormatterMathtext, LogFormatter
import numpy as np
from itertools import chain
from stop import stattest
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
    def __init__(self, title, ratio=False): 
        self.title = title
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
        self.x_vals = None
        self._y_sum_step = None
        self.y_sum = None
        self.colors = 'mky'
        self.y_min = None
        self._proxy_legs = []
        self._bg_proxy_legs = []
        self.ratio_max = 2.0
        
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
        
    def add_backgrounds(self, hist_list): 
        last_plot = 0
        color_itr = iter(self.colors)
        if self.y_min is not None: 
            last_plot = self.y_min
        for hist in hist_list:
            x_vals, y_vals = hist.get_xy_step_pts()

            if self.x_vals is None: 
                self.x_vals = x_vals
            else: 
                pass            # TODO: add check
            ylabel = self.ax.get_ylabel()
            if not ylabel: 
                self.ax.set_ylabel(hist.y_label, y=0.98,ha='right')
            self._set_xlab(hist.x_label)

            fill_color = hist.color
            if not fill_color: 
                fill_color = next(color_itr)

            if self._y_sum_step is None:
                self._y_sum_step = y_vals
            else: 
                self._y_sum_step = self._y_sum_step + y_vals
                
            if self.y_sum is None: 
                self.y_sum = hist.get_xy_center_pts()[1]
            else: 
                self.y_sum += hist.get_xy_center_pts()[1]

            tmp_sum = np.array(self._y_sum_step[:])
            if self.y_min is not None: 
                tmp_sum[tmp_sum < self.y_min] = self.y_min

            self.ax.fill_between(x_vals, tmp_sum, last_plot, 
                                 facecolor=fill_color)
            proxy = plt.Rectangle((0, 0), 1, 1, fc=fill_color, 
                                  label=hist.title)

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

    def _add_ratio(self, x_vals, y_vals, lows, highs): 
        ratable = (self.y_sum > 0.0) 
        rat_x = x_vals[ratable]
        rat_y = y_vals[ratable]
        rat_y_sum = self.y_sum[ratable]
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
        if self.x_vals is None: 
            self.x_vals = x_vals
        
        lows, highs = stattest.poisson_interval(y_vals)

        plt_y = self._get_min_plotable(y_vals)
        plt_low = self._get_min_plotable(lows)
        plt_err_up = highs - plt_y
        plt_err_down = plt_y - plt_low

        if not np.any(plt_err_up): 
            return 
        line,cap,notsure = self.ax.errorbar(
            x_vals, plt_y, ms=10, fmt='k.', 
            yerr=[plt_err_down,plt_err_up])

        if self.ratio and np.any(self.y_sum): 
            self._add_ratio(x_vals, y_vals, lows, highs)
            
        self._proxy_legs.append( (line, hist.title))
    
    def add_legend(self): 
        all_legs = chain(self._proxy_legs,reversed(self._bg_proxy_legs))
        proxies = zip(*all_legs)
        legend = self.ax.legend(*proxies, numpoints=1)
        legend.get_frame().set_linewidth(0)
        legend.get_frame().set_alpha(0)
            
    def save(self, name):
        if self.ax.get_yscale() != 'log': 
            ymax = self.ax.get_ylim()[1]
            self.ax.set_ylim(0,ymax)
            self.ax.set_xlim(min(self.x_vals), max(self.x_vals))
        self.fig.tight_layout(pad=0.3, h_pad=0.3, w_pad=0.3)

        if self.ratio: 
            self.ratio.set_ylim(0,self.ratio_max)
        self.canvas.print_figure(name)
    def close(self): 
        # should garbage collect on it's own now
        warn('This method is no longer needed',FutureWarning,stacklevel=2)

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
            
        try: 
            cb = plt.colorbar(im, **cb_dict)
        except ValueError: 
            print self.imdict['X'].sum()
            raise
        ax.set_xlabel(self.x_label, x=0.98, ha='right')        
        ax.set_ylabel(self.y_label, y=0.98, ha='right')
        if self.cb_label: 
            cb.set_label(self.cb_label, y=0.98, ha='right')
        canvas.print_figure(name, bbox_inches='tight')

class PlottingError(StandardError): 
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
    for color, physics_types in color_groups.iteritems(): 
        for phys in physics_types: 
            if color_arrays[color] is None: 
                color_arrays[color] = phys_dicts[phys]['X']
            else: 
                color_arrays[color] += phys_dicts[phys]['X']

    stacked_array = np.dstack( 
        (color_arrays[color] for color in ['red','green','blue']))

    stacked_array = np.log(stacked_array + 1)
    for i in xrange(3): 
        the_max = stacked_array[:,:,i].max()
        stacked_array[:,:,i] /= the_max


    chop_factor = 0.5
    new_max = stacked_array.max()*chop_factor
    chop_vals = (stacked_array > new_max)
    stacked_array[chop_vals] = new_max
    stacked_array /= stacked_array.max()

    for color, array in color_arrays.iteritems(): 
        print color, array.sum()

    im_dic = phys_dicts[signal_point].copy()
    im_dic['X'] = stacked_array

    figure = Figure(figsize=(8,8))
    canvas = FigureCanvas(figure)
    ax = figure.add_subplot(1,1,1)
    ax.imshow(**im_dic)
    y_lims = ax.get_ylim()
    x_lims = ax.get_xlim()

    for color, phys_types in color_groups.iteritems(): 
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
