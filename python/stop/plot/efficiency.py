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
        self._legs = []
        self.x_range = x_range
        
    def add_efficiency(self, num, denom, extent, name='', color=None): 
        """
        overflow bins should not be included. 
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
        fmt = '{}.'.format(color)
        line, cap, notsure = self.ax.errorbar(
            x_vals, ratio, ms=10, fmt=fmt, 
            yerr=[err_down, err_up])
        if name: 
            self._legs.append( (line, name))
        
    def save(self, name): 
        if self._legs: 
            legend = self.ax.legend(*zip(*self._legs), numpoints=1)
            legend.get_frame().set_linewidth(0)
            legend.get_frame().set_alpha(0)
        self.ax.set_ylim(*self.y_range)
        self.fig.savefig(name, bbox_inches='tight')
