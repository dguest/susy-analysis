"""
Lower level drawing routines for stack plots
"""

import matplotlib.pyplot as plt
from matplotlib.colors import LogNorm, Normalize
from matplotlib.gridspec import GridSpec
from matplotlib.ticker import MaxNLocator, LogLocator
from matplotlib.ticker import LogFormatterMathtext, LogFormatter
from matplotlib.lines import Line2D
from matplotlib.patches import Rectangle
import numpy as np
from itertools import chain
from scharm import errorbars
from warnings import warn
from matplotlib.backends.backend_agg import FigureCanvasAgg as FigureCanvas
from matplotlib.figure import Figure
from mpl_toolkits.axes_grid1 import make_axes_locatable
from os.path import join, isdir, dirname
import os, math

class Stack:
    """
    This is for drawing.
    """
    lumi_str = '$\int\ \mathcal{{L}}\ dt\ =\ ${:.1f} fb$^{{-1}}$'
    syserr_name = 'systematic'
    staterr_name = 'statistical'
    toterr_name = 'total'
    ratio_grid_color = (0,0,0,0.2)
    label_font_size = 16
    # confidence interval for data
    data_ci = 1 - math.erf( (1/2)**0.5) # 1 sigma
    def __init__(self, ratio=False, exclude_zeros=True,
                 selection_colors=('r',(0.9, 0, 0, 0.2))):
        self._exclude_zeros = exclude_zeros
        self.fig = Figure(figsize=(8,6))
        self.canvas = FigureCanvas(self.fig)
        self.lumi = None
        self.ratio_max = 2.0
        self.ratio_font_size = 10
        self.colors = list('kc') + ['purple', 'orange']
        self.y_min = None
        if not ratio:
            self.ax = self.fig.add_subplot(1,1,1)
            self.ratio = None
        else:
            grid = GridSpec(2,1, height_ratios=[3,1])
            self.ax = self.fig.add_subplot(grid[0])
            self.ratio = self.fig.add_subplot(grid[1],sharex=self.ax)
            self.ratio.set_ylabel('Data / SM', fontsize=self.label_font_size)
            locator = MaxNLocator(5, prune='upper')
            self.ratio.get_yaxis().set_major_locator(locator)
            self.ratio.tick_params(
                labelsize=self.label_font_size, which='both')
        self.ax.tick_params(labelsize=self.label_font_size, which='both')

        self._selection = None
        self._y_sum_step = 0.0
        self._y_sum = 0.0
        self._proxy_legs = []
        self._bg_proxy_legs = []
        self._rat_legs = []     # for ratio plot
        self._x_limits = None
        self._sm_total = 0.0
        # for setting plot upper limit with selection applied
        self._upper_limit = 0.0
        # scale up y axis when the legends get big
        self._scaleup = 1.0
        self._set_selection_colors(*selection_colors)

    def _set_selection_colors(self, line, face):
        self._cut_color = line
        self._cut_fill = face

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
            axes.set_xlabel(name, x=0.98, ha='right',
                            fontsize=self.label_font_size)

    def _get_legstr(self, hist):
        title = hist.title.replace('scharm-','')
        hval = float(hist)
        return '{}: {}'.format(title, _legstr(hval))

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
                self.ax.set_ylabel(hist.y_label, y=0.98, ha='right',
                                   fontsize=self.label_font_size)
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
            self._bg_proxy_legs.append( (proxy,self._get_legstr(hist)))

            last_plot = tmp_sum
            self._sm_total += float(hist)

    def add_syst2(self, hist_list):
        x_vals, rel_sys_err = _get_mc_error_bands(
            hist_list, self._y_sum_step)
        if self.ratio:
            self.ratio.fill_between(
                x_vals, 1 - rel_sys_err, 1 + rel_sys_err,
                color=self._cut_fill, linewidth=0.0)
            proxy = Rectangle(
                (0, 0), 1, 1, fc=self._cut_fill, ec='none')
            self._rat_legs.append((proxy, self.syserr_name))

    def add_wt2(self, hist_list):
        x_vals, rel_stat_err = _get_mc_error_bands(
            hist_list, self._y_sum_step)
        if self.ratio:
            self.ratio.plot(x_vals, 1 - rel_stat_err, 'k:')
            ln, = self.ratio.plot(x_vals, 1 + rel_stat_err, 'k:')
            self._rat_legs.append( (ln, self.staterr_name) )

    def add_total2(self, hist_list):
        x_vals, rel_err = _get_mc_error_bands(
            hist_list, self._y_sum_step)
        if self.ratio:
            self.ratio.plot(x_vals, 1 - rel_err, color=self._cut_color)
            ln, = self.ratio.plot(x_vals, 1 + rel_err, color=self._cut_color)
            self._rat_legs.append( (ln, self.toterr_name) )

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
            style = '--'
            plt_handle, = self.ax.plot(
                x_vals,y_vals,style, linewidth=3.0, color=color)
            self._proxy_legs.append( (plt_handle, self._get_legstr(hist)))

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
        fill_args = dict(facecolor=self._cut_fill, linestyle=None)
        line_args = dict(color=self._cut_color, linewidth=2)
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

        # add lines showing 1, 0.5, 1.5
        self.ratio.axhline(y=1, linestyle='-', color=self.ratio_grid_color)
        self.ratio.axhline(y=0.5, linestyle='--', color=self.ratio_grid_color)
        self.ratio.axhline(y=1.5, linestyle='--', color=self.ratio_grid_color)

    def add_data(self, hist):
        x_vals, y_vals = hist.get_xy_center_pts()
        lows, highs = errorbars.poisson_interval(y_vals, self.data_ci)

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

        self._proxy_legs.append( (line, self._get_legstr(hist)))
        # TODO: fix this, the dims don't match the backgrounds
        # self._upper_limit = np.maximum(self._upper_limit, highs)

    def _add_pr_crap(self):
        if self.lumi:
            self.ax.text(
                0.02, 0.85, self.lumi_str.format(self.lumi),
                transform=self.ax.transAxes, size=self.label_font_size)
            self.ax.text(
                0.05, 0.75, r'$\sqrt{s} = $ 8 TeV',
                transform=self.ax.transAxes, size=self.label_font_size)

        vert = 0.56 if self.ratio else 0.66
        atl_lable_args = dict(
            # x=0.15, y=0.7,
            x=0.7, y=vert,
            style='italic', transform=self.ax.transAxes,
            size=self.label_font_size + 4)
        self.ax.text(s='ATLAS', weight='bold',
                     horizontalalignment='right', **atl_lable_args)
        self.ax.text(s=' INTERNAL',
                     horizontalalignment='left', **atl_lable_args)

    def add_legend(self):
        tstring = 'SM total: {}'.format(_legstr(self._sm_total))
        tartist = Line2D((0,1),(0,0), color='k')
        bg_legs = self._bg_proxy_legs[::-1]
        all_legs = self._proxy_legs + [(tartist, tstring)] + bg_legs
        proxies = zip(*all_legs)
        legend = self.ax.legend(*proxies, numpoints=1, ncol=2)
        if legend:
            legend.get_frame().set_linewidth(0)
            legend.get_frame().set_alpha(0)

        # crude guess for how many legends fit
        max_fitting = 24 if self.ratio else 34
        n_leg = len(all_legs)
        frac_consumed = min(n_leg / max_fitting, 0.9)
        self._scaleup = 1 / (1 - frac_consumed)

        if self._rat_legs:
            lg = self.ratio.legend(
                *zip(*self._rat_legs), fontsize=self.ratio_font_size,
                 ncol=3, loc='lower right', borderaxespad=0.0)
            # lg.get_frame().set_linewidth(0)
            # lg.get_frame().set_alpha(0)

        self._add_pr_crap()

    def save(self, name):
        if self._x_limits is None:
            return

        self._draw_selection()
        self.ax.set_xlim(*self._x_limits)
        if self.ax.get_yscale() != 'log':
            ymax = self.ax.get_ylim()[1]
            self.ax.set_ylim(0,ymax * self._scaleup)
        else:
            ymin, ymax = self.ax.get_ylim()
            self.ax.set_ylim(ymin, ymax**self._scaleup)

        self.fig.tight_layout(pad=0.3, h_pad=0.3, w_pad=0.3)

        if self.ratio:
            self.ratio.set_ylim(0,self.ratio_max)
        self.canvas.print_figure(name)

def _legstr(hval):
    """
    format legend string
    """
    if hval < 10:
        return '{:.1f}'.format(hval)
    exp = int(math.log10(hval * 2) / 3)
    char = {1:'k', 2:'M', 3:'G'}.get(exp,'')
    coif = hval / 10**(exp*3)
    prec = 1 if coif < 10 else 0
    return '{:.{p}f}{}'.format(coif, char, p=prec)

def _get_mc_error_bands(hist_list, y_sum_step, nan_proxy_val=100.0):
    """internal function in Stack, gets the error bars for ratio plots"""
    sum_syst2 = None
    for hist in hist_list:
        if sum_syst2 is None:
            sum_syst2 = hist
        else:
            sum_syst2 += hist
    x_vals, y_vals = sum_syst2.get_xy_step_pts()
    rel_sys_err = y_vals**0.5 / y_sum_step
    inf_vals = np.isinf(rel_sys_err) | np.isnan(rel_sys_err)
    rel_sys_err[inf_vals] = nan_proxy_val
    return x_vals, rel_sys_err

class Hist1d(object):
    """
    class to organize 1d hists from np arrays
    """

    def __init__(self, array, extent,
                 x_label = '', y_label = '', x_units='', title=''):
        self._array = array
        self._prebin_array = None
        self.extent = extent
        self._x_label = x_label
        self._x_units = x_units
        self._y_label = y_label
        self.title = title
        self.color = ''
        self.selection = None

    def _get_bindexer(self, warn_threshold=0.001, err_threshold=0.001):
        """
        returns a function that finds bin index for a given x value
        """
        if self._prebin_array is None:
            arr = self._array
        else:
            arr = self._prebin_array
        npts = arr.size - 1 # n non-overflow bins + 1
        xpts = np.linspace(*self.extent, num=npts)

        def bindex(value):
            if value == np.inf:
                return arr.size
            if value == -np.inf:
                return 0
            idx = np.abs(xpts - value).argmin()
            rel_err = abs( (xpts[idx] - value) / (xpts[-1] - xpts[0]))
            if rel_err > warn_threshold or warn_threshold > err_threshold:
                wstr = "bad binning in {}: target {} != {}".format(
                    self.title, value, xpts[idx])
                if rel_err > err_threshold:
                    raise ValueError(wstr)
                else:
                    warn(wstr)

            return idx + 1
        return bindex

    def __float__(self):
        # if there's a selection, we only return the sum of the selected
        if self.selection:
            if self._prebin_array is None:
                arr = self._array
            else:
                arr = self._prebin_array

            slow, shigh = self.selection
            bindex = self._get_bindexer()
            return arr[bindex(slow):bindex(shigh)].sum()

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

    def rebin(self, max_bins):
        n_bins = self._array.size - 2
        if n_bins < max_bins:
            return

        # grab the prebin array if we've already rebinned before
        if self._prebin_array is not None:
            self._array = self._prebin_array
        else:
            self._prebin_array = self._array
        binbase = np.arange(2,16)
        rebin_numbers = np.concatenate([binbase*10**x for x in (0,1,2,3)])
        # find the maximum numbers by which the current number is divisible
        int_rebin = (n_bins % rebin_numbers == 0)
        # and the numbers which give less than the max number of bins
        under_max = (n_bins // rebin_numbers < max_bins)
        valid_rebins = int_rebin & under_max
        if not valid_rebins.any():
            return
        n_bins = rebin_numbers[int_rebin & under_max].min()
        self._rebin(n_bins)

    def _rebin(self, n_bins):
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

    def crop(self, low=-np.inf, high=np.inf):
        bindex = self._get_bindexer()
        if self._prebin_array is not None:
            warn("throwing away binning by cropping, fixme", stacklevel=2)
            self._array = self._prebin_array
        uf, center, of = np.split(self._array, [bindex(low), bindex(high)])
        # the under and overflows may be included in 'center', in which
        # case we don't want to add them to the new array. So we use crunch.
        def crunch(arr):
            """like sum, but keeps zero-sized arrays zero-sized"""
            if arr.size > 0:
                return np.array([arr.sum()])
            return np.array([])
        new_arr = np.concatenate([crunch(uf), center, crunch(of)])
        self._array = self._prebin_array = new_arr
        self.extent = (low, high)

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
