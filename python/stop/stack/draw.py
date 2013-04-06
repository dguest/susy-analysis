import matplotlib.pyplot as plt
from matplotlib.colors import LogNorm, Normalize
from matplotlib.gridspec import GridSpec
from matplotlib.ticker import MaxNLocator
import numpy as np
from itertools import chain
from stop import stattest

class Stack(object): 
    """
    This is for drawing. 
    """
    def __init__(self, title, ratio=False): 
        self.title = title
        self.fig = plt.figure(figsize=(8,6))
        if not ratio:
            self.ax = self.fig.add_subplot(1,1,1)
            self.ratio = None
        else: 
            grid = GridSpec(2,1, height_ratios=[3,1])
            self.ax = self.fig.add_subplot(grid[0])
            self.ratio = self.fig.add_subplot(grid[1],sharex=self.ax)
            self.ratio.set_ylabel('Data / Sim', y=0.98, va='top')
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
                self.ax.set_ylabel(hist.y_label, y=0.98, va='top')
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
        self.fig.tight_layout()# pad=0.0, h_pad=0.0
        self.fig.savefig(name, bbox_inches='tight')
    def close(self): 
        plt.close(self.fig)

class Hist2d(object): 
    def __init__(self, imdict, xlabel, ylabel): 
        self.imdict = imdict
        self.x_label = xlabel
        self.y_label = ylabel

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

    def save(self, name, log=False): 
        fig = plt.figure(figsize=(8,6))
        ax = fig.add_subplot(1,1,1)
        if log: 
            norm=LogNorm()
        else: 
            norm=Normalize()
        im = ax.imshow(norm=norm,**self.imdict)
        cb_dict = {}
            
        cb = plt.colorbar(im, **cb_dict)
        ax.set_xlabel(self.x_label, x=0.98, ha='right')        
        ax.set_ylabel(self.y_label, y=0.98, va='top')
        fig.savefig(name, bbox_inches='tight')
        plt.close(fig)

class PlottingError(StandardError): 
    def __init__(self, problem): 
        super(PlottingError,self).__init__(problem)
