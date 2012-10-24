import matplotlib.pyplot as plt
import numpy as np

class Hist1d(object): 
    """
    class to organize 1d hists from np arrays
    """
    
    def __init__(self, array, extent, 
                 x_label = '', y_label = 'Jets', title=''): 
        self._array = array
        self._extent = extent
        self.x_label = x_label
        self.y_label = y_label
        self.title = title

    def __float__(self): 
        return self._array[1:-1].sum()
    def __cmp__(self, other): 
        this_val = float(self)
        other_val = float(other)
        return this_val - other_val
    def __str__(self): 
        return self.title
    def __add__(self, other): 
        diffs = [(x - y)**2 for x, y in zip(self._extent, other._extent)]
        if sum(diffs) / sum(x**2 for x in self._extent) > 1e-4: 
            raise ValueError(
                'tried to add a hist with extent {} to one with'
                ' extent {}'.format(self._extent, other._extent) )
        sum_array = self._array + other._array
        new_y = self._zipstring(self.y_label, other.y_label)
        new_x = self._zipstring(self.x_label, other.x_label)
        new_title = self._zipstring(self.title, other.title)
        new_hist = Hist1d(sum_array, self._extent, 
                          x_label=new_x, y_label=new_y, title=new_title)
        return new_hist
    def _zipstring(self, one, two): 
        return str(c for c, o in zip(one, two) if c == o)
    
    def scale(self, factor): 
        self._array = self._array * factor
    def get_xy_pts(self): 
        """
        returns slightly hacked xy pairs which can be fed to plt.plot()
        """
        y_vals = self._array[1:-1] # no overflow
        double_y_vals = np.dstack((y_vals,y_vals)).flatten()
        x_vals = np.linspace(*self._extent, num=len(y_vals) + 1, endpoint=True)
        double_x_vals = np.dstack((x_vals,x_vals)).flatten()[1:-1]

        return double_x_vals, double_y_vals

class Stack(object): 
    def __init__(self, title): 
        self.title = title
        self.fig = plt.figure(figsize=(8,6))
        self.ax = self.fig.add_subplot(1,1,1)
        self.x_vals = None
        self._y_sum = None
        self.colors = 'bgrcmyk'
        self.y_min = None
        self._proxy_legs = []
    def add_backgrounds(self, hist_list): 
        last_plot = 0
        if self.y_min is not None: 
            last_plot = self.y_min
        for hist, color in zip(hist_list, self.colors):
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
            self.ax.fill_between(x_vals, self._y_sum, last_plot, 
                                 facecolor=color)
            proxy = plt.Rectangle((0, 0), 1, 1, fc=color, 
                                  label=str(hist))
            self._proxy_legs.append( (proxy,str(hist)) )

            last_plot = self._y_sum

    def add_signals(self, hist_list): 
        styles = [''.join([x,'--']) for x in self.colors]
        for hist, style in zip(hist_list, styles): 
            x_vals, y_vals = hist.get_xy_pts()
            if self.y_min is not None: 
                y_vals[y_vals < self.y_min] = self.y_min
            plt_handle, = self.ax.plot(x_vals,y_vals,style)
            self._proxy_legs.append( (plt_handle, str(hist)))
    
    def add_legend(self): 
        proxies = zip(*self._proxy_legs)
        legend = self.ax.legend(*proxies)
        legend.get_frame().set_linewidth(0)
            
    def save(self, name): 
        self.fig.savefig(name, bbox_inches='tight')
