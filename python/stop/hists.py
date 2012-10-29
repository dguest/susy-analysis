import matplotlib.pyplot as plt
import numpy as np
from itertools import chain

class Hist1d(object): 
    """
    class to organize 1d hists from np arrays
    """
    
    def __init__(self, array, extent, 
                 x_label = '', y_label = 'Jets', title='', group = ''): 
        self._array = array
        self.extent = extent
        self.x_label = x_label
        self.y_label = y_label
        self.title = title
        self.color = ''
        self.group = group

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
        if self.group != other.group: 
            error = "tried to add {} in group {} to {} in group {}."
            raise ValueError(error.format(self.title, self.group, 
                                          other.title, other.group))
        sum_array = self._array + other._array
        new_y = self._zipstring(self.y_label, other.y_label)
        new_x = self._zipstring(self.x_label, other.x_label)
        new_title = self._zipstring(self.title, other.title)
        new_hist = Hist1d(sum_array, self.extent, 
                          x_label=new_x, y_label=new_y, title=new_title)
        new_hist.color = self.color if self.color else other.color
        new_hist.group = self.group
        return new_hist
    def _zipstring(self, one, two): 
        return ''.join(c for c, o in zip(one, two) if c == o)
    
    def scale(self, factor): 
        self._array = self._array * factor
    def average_bins(self, bins): 
        ave = np.average(self._array[bins])
        self._array[bins] = ave
    def get_xy_pts(self): 
        """
        returns slightly hacked xy pairs which can be fed to plt.plot()
        """
        y_vals = self._array[1:-1] # no overflow
        double_y_vals = np.dstack((y_vals,y_vals)).flatten()
        x_vals = np.linspace(*self.extent, num=len(y_vals) + 1)
        double_x_vals = np.dstack((x_vals,x_vals)).flatten()[1:-1]

        return double_x_vals, double_y_vals

class Stack(object): 
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
                self.ax.set_ylabel(hist.y_label)
            xlabel = self.ax.get_xlabel()
            if not xlabel: 
                self.ax.set_xlabel(hist.x_label)

            if self._y_sum is None:
                self._y_sum = y_vals
            else: 
                self._y_sum = self._y_sum + y_vals

            tmp_sum = self._y_sum[:]
            if self.y_min is not None: 
                tmp_sum[tmp_sum < self.y_min] = self.y_min
            
            fill_color = hist.color
            if not fill_color: 
                fill_color = next(color_itr)

            self.ax.fill_between(x_vals, self._y_sum, last_plot, 
                                 facecolor=fill_color)
            proxy = plt.Rectangle((0, 0), 1, 1, fc=fill_color, 
                                  label=hist.title)

            self._bg_proxy_legs.append( (proxy,hist.title)) 

            last_plot = self._y_sum

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
    
    def add_legend(self): 
        all_legs = chain(self._proxy_legs,reversed(self._bg_proxy_legs))
        proxies = zip(*all_legs)
        legend = self.ax.legend(*proxies)
        legend.get_frame().set_linewidth(0)
        legend.get_frame().set_alpha(0)
            
    def save(self, name): 
        self.fig.savefig(name, bbox_inches='tight')
    def close(self): 
        plt.close(self.fig)
