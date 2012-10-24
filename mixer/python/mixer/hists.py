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

            if self._y_sum is None:
                self._y_sum = y_vals
            else: 
                self._y_sum = self._y_sum + y_vals

            tmp_sum = self._y_sum[:]
            if self.y_min is not None: 
                tmp_sum[tmp_sum < self.y_min] = self.y_min
            self.ax.fill_between(x_vals, self._y_sum, last_plot, 
                                 facecolor=color)
            last_plot = self._y_sum
            
    def save(self, name): 
        self.fig.savefig(name, bbox_inches='tight')
