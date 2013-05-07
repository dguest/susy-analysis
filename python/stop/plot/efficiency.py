from matplotlib.backends.backend_agg import FigureCanvasAgg as FigureCanvas
from matplotlib.figure import Figure
import numpy as np
from stop.stattest import binomial_interval

class EfficiencyPlot(object): 
    """
    Plots the efficiency of several distributions overlayed. 
    """
    def __init__(self, y_range, x_range=None): 
        self.y_range = y_range
        self.fig = Figure(figsize=(8,6))
        self.canvas = FigureCanvas(self.fig)
        self.ax = self.fig.add_subplot(1,1,1)
        self._color_itr = iter('kbgrm')
        self.legends = []
        self.x_range = x_range

    def _error_from_num_denom(self, num, denom, num_wt2, denom_wt2): 
        p = num 
        f = denom - num 
        f_wt2 = denom_wt2 - num_wt2
        p_wt2 = num_wt2
        error = (p_wt2**0.5 * f + f_wt2**0.5 * p) / (p + f)**2.0
        return error

    def add_efficiency(self, num, denom, extent, name='', color=None, 
                       num_wt2=None, denom_wt2=None): 
        """
        Adds efficiency num / denom. If no wt2 arrays are given, calculates
        binomial errors. Otherwise uses linear error propagation (assuming
        num is a subset of denom). 

        Overflow bins should not be included. 
        """
        tmp_xpts = len(num)*2 + 1
        x_vals = np.linspace(*extent, num=tmp_xpts)[1:-1:2]

        # remove the points where the denominator is zero
        valid = denom > 0.0
        # also remove points outside x-range
        if self.x_range: 
            valid &= x_vals > self.x_range[0]
            valid &= x_vals < self.x_range[1]
        x_vals = x_vals[valid]
        num = num[valid]
        denom = denom[valid]
        ratio = num / denom
        if num_wt2 is not None and denom_wt2 is not None: 
            num_wt2 = num_wt2[valid]
            denom_wt2 = denom_wt2[valid]
            err = self._error_from_num_denom(num, denom, num_wt2, denom_wt2)
            low = ratio - err
            high = ratio + err
        else: 
            low, high = binomial_interval(num, denom) / denom
        
        # force within window
        ratio[ratio > self.y_range[1]] = self.y_range[1]
        high[high > self.y_range[1]] = self.y_range[1]
        low[low <= self.y_range[0]] = self.y_range[0] 
        low[np.isnan(low)] = 0.0

        err_down = ratio - low
        err_up = high - ratio
        if not color: 
            color = next(self._color_itr)
        line, cap, notsure = self.ax.errorbar(
            x_vals, ratio, ms=10, fmt='.', mfc=color, mec=color, 
            ecolor=color, 
            yerr=[err_down, err_up])
        if name: 
            self.legends.append( (line, name))
        
    def save(self, name): 
        if self.legends: 
            legend = self.ax.legend(*zip(*self.legends), numpoints=1)
            legend.get_frame().set_linewidth(0)
            legend.get_frame().set_alpha(0)
        self.ax.set_ylim(*self.y_range)
        self.fig.savefig(name, bbox_inches='tight')
