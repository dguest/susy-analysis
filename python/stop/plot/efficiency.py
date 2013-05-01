import matplotlib.pyplot as plt
import numpy as np
from stop.stattest import binomial_interval

class EfficiencyPlot(object): 
    """
    Plots the efficiency of several distributions overlayed. 
    """
    def __init__(self, y_min, y_max): 
        self.y_range = (y_min, y_max)
        self.fig = plt.figure(figsize=(8,6))
        self.ax = self.fig.add_subplot(1,1,1)
        self._color_itr = iter('kbgrm')
        self._legs = []
        
    def add_efficiency(self, num, denom, extent, name=''): 
        """
        overflow bins should not be included. 
        """
        tmp_xpts = len(num)*2 + 1
        x_vals = np.linspace(*extent, num=tmp_xpts)[1:-1:2]
        valid = denom > 0.0
        ratio = np.zeros(num.shape)
        ratio[valid] = num[valid] / denom[valid]
        low, high = binomial_interval(num, denom)
        err_down = ratio - low
        err_up = high - ratio
        fmt = '{}.'.format(next(self._color_itr))
        line, cap, notsure = self.ax.errorbar(
            x_vals, ratio, ms=10, fmt=fmt, 
            yerr=[err_down, err_up])
        if name: 
            self._legs.append( (line, name))
        
    def save(self, name): 
        if self._legs: 
            legend = self.ax.legend(*zip(self._legs), numpoints=1)
            legend.get_frame().set_linewidth(0)
            legend.get_frame().set_alpha(0)
        self.fig.savefig(name, bbox_inches='tight')
    def close(self): 
        plt.close(self.fig)
