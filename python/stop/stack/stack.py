import matplotlib.pyplot as plt
import numpy as np
from itertools import chain
from stop import stattest
from math import sqrt, erf

class Stack(object): 
    """
    This is for drawing. 
    """
    def __init__(self, title): 
        self.title = title
        self.fig = plt.figure(figsize=(8,6))
        self.ax = self.fig.add_subplot(1,1,1)
        self.x_vals = None
        self._y_sum = None
        self.colors = 'mky'
        self.y_min = None
        self._proxy_legs = []
        self._bg_proxy_legs = []
    def add_backgrounds(self, hist_list): 
        last_plot = 0
        color_itr = iter(self.colors)
        if self.y_min is not None: 
            last_plot = self.y_min
        for hist in hist_list:
            x_vals, y_vals = hist.get_xy_pts()

            if self.x_vals is None: 
                self.x_vals = x_vals
            else: 
                pass            # TODO: add check
            ylabel = self.ax.get_ylabel()
            if not ylabel: 
                self.ax.set_ylabel(hist.y_label, y=0.98, va='top')
            xlabel = self.ax.get_xlabel()
            if not xlabel: 
                self.ax.set_xlabel(hist.x_label, x=0.98, ha='right')

            fill_color = hist.color
            if not fill_color: 
                fill_color = next(color_itr)

            if self._y_sum is None:
                self._y_sum = y_vals
            else: 
                self._y_sum = self._y_sum + y_vals

            tmp_sum = np.array(self._y_sum[:])
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
            x_vals, y_vals = hist.get_xy_pts()
            if self.y_min is not None: 
                y_vals[y_vals < self.y_min] = self.y_min

            if hist.color: 
                color = hist.color
            else: 
                color = next(color_itr)
            style = color + '--'
            plt_handle, = self.ax.plot(x_vals,y_vals,style, linewidth=2.0)
            self._proxy_legs.append( (plt_handle, hist.title))

    def add_data(self, hist): 
        x_vals, y_vals = hist.get_data_xy()
        if self.x_vals is None: 
            self.x_vals = x_vals
        
        lows = np.zeros(y_vals.shape)
        highs = np.zeros(y_vals.shape)
        tail_prob = 1.0 - erf(1/sqrt(2)) # one sigma
        lows, highs = stattest.poisson_interval(y_vals, tail_prob)
        if self.y_min is not None:
            bad_yvals = y_vals <= self.y_min
            y_vals[bad_yvals] = self.y_min
            bad_lows = lows <= self.y_min
            lows[bad_lows] = self.y_min

        y_err_up = highs - y_vals
        y_err_down = y_vals - lows

        if not np.any(y_err_up): 
            return 
        line,cap,notsure = self.ax.errorbar(x_vals, y_vals, 
                                            ms=10, fmt='k.', 
                                            yerr=[y_err_down, y_err_up])
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
        self.fig.savefig(name, bbox_inches='tight')
    def close(self): 
        plt.close(self.fig)
