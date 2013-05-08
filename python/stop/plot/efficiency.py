from matplotlib.backends.backend_agg import FigureCanvasAgg as FigureCanvas
from matplotlib.figure import Figure
import numpy as np
from stop.stattest import binomial_interval
from stop.hists import HistToBinsConverter

class EfficiencyPlot(object): 
    """
    Plots the efficiency of several distributions overlayed. 
    """
    def __init__(self, y_range, x_range=None, bins=None, max_bins=150): 
        self.y_range = y_range
        self.fig = Figure(figsize=(8,6))
        self.canvas = FigureCanvas(self.fig)
        self.ax = self.fig.add_subplot(1,1,1)
        self._color_itr = iter('kbgrm')
        self.legends = []
        self.x_range = x_range
        self.max_bins = max_bins

    def _error_from_num_denom(self, num, denom, num_wt2, denom_wt2): 
        """
        Propagate errors assuming num is a subset of denom. 
        """
        p = num 
        f = denom - num 
        f_wt2 = denom_wt2 - num_wt2
        p_wt2 = num_wt2
        error = (p_wt2**0.5 * f + f_wt2**0.5 * p) / (p + f)**2.0
        return error

    def _rebin(self, num): 
        if len(num) > self.max_bins and len(num) % 2 == 0: 
            new_num = num.reshape((-1,2)).sum(1)
            # call self recursively until rebinning is done
            return self._rebin(new_num)
        return num

    def add_efficiency(self, num, denom, extent, name='', color=None, 
                       num_wt2=None, denom_wt2=None): 
        """
        Adds efficiency num / denom. If no wt2 arrays are given, calculates
        binomial errors. Otherwise uses linear error propagation (assuming
        num is a subset of denom). 

        Overflow bins should not be included. 
        """
        num = self._rebin(num)
        denom = self._rebin(denom)
        num_wt2 = self._rebin(num_wt2)
        denom_wt2 = self._rebin(denom_wt2)

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

class BinnedEfficiencyPlot(EfficiencyPlot): 
    """
    Like the EfficiencyPlot, but with custom bins. With no bins should 
    default to the EfficiencyPlot binning (not yet, but someday)
    """
    def __init__(self, y_range, x_range=None, bins=None, **argv): 
        super(BinnedEfficiencyPlot, self).__init__(y_range, x_range, **argv)
        self.bins = bins
        if not self.bins: 
            raise ValueError("we need bins for now, none set")

    def add_efficiency(self, num, denom, extent, name='', color=None, 
                       num_wt2=None, denom_wt2=None): 
        """
        Adds efficiency num / denom. If no wt2 arrays are given, calculates
        binomial errors. Otherwise uses linear error propagation (assuming
        num is a subset of denom). 

        Overflow bins should not be included. 
        """

        binner = HistToBinsConverter(self.bins)
        def get_binned(counts, do_x=False): 
            uf, x, xer, y, of = binner.get_bin_counts(
                counts, extent, overflows_included=False)
            if do_x: 
                return x, xer, y
            else:
                return y

        x_vals, x_err, num = get_binned(num, do_x=True)
        denom = get_binned(denom)
        num_wt2 = get_binned(num_wt2)
        denom_wt2 = get_binned(denom_wt2)

        # remove the points where the denominator is zero
        valid = denom > 0.0 
        x_vals = x_vals[valid]
        x_err = x_err[valid]
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
            yerr=[err_down, err_up], xerr=x_err)
        if name: 
            self.legends.append( (line, name))
        
