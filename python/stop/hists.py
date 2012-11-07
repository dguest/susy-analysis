import matplotlib.pyplot as plt
import numpy as np
from itertools import chain
from collections import defaultdict
import copy

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

class HistNd(object): 
    """
    Wrapper for multidimensional array. This class exists to simplify 
    conversion from HDF5 array to a 1d or 2d hist which can be plotted. 
    """
    class Axis(object): 
        def __init__(self): 
            self.name = None
            self.bins = None
            self.min = None
            self.max = None
            self.number = None
            self.type = 'bare'
        def __eq__(self, other): 
            span = self.max - self.min
            span_diff = (self.max - other.max)**2 + (self.min - other.min)**2
            conditions = [ 
                self.name == other.name, 
                self.bins == other.bins, 
                self.number == other.number, 
                span_diff / span**2 < 1e-6, 
                self.type == other.type, 
                ]
            return all(conditions)
        def __ne__(self, other): 
            not self == other

        @property
        def valid(self): 
            conditions = [ 
                self.name, 
                self.bins, 
                self.min is not None, 
                self.max is not None, 
                self.number is not None, 
                ]
            return all(conditions)
        @property
        def axis(self): 
            return self.number
        @axis.setter
        def axis(self, num): 
            self.number = num

    def __init__(self,array=None): 
        self._axes = defaultdict(HistNd.Axis)
        if array: 
            self.__from_hdf(array)

    def __from_hdf(self, hdf_array): 
        self._array = np.array(hdf_array)
        for name, atr in hdf_array.attrs.items(): 
            ax_name, part, ax_prop = name.rpartition('_')
            the_axis = self._axes[ax_name]
            setattr(the_axis, ax_prop, atr)
            the_axis.name = ax_name
        for name, axis in self._axes.items(): 
            if not axis.valid: 
                raise IOError("{} isn't well defined".format(name))

    def __to_hdf(self, target, name): 
        ds = target.create_dataset(name, data=self._array, 
                                   compression='gzip')
        ax_list = sorted([(ax.number, ax) for ax in self._axes.values()])
        for num, ax in ax_list: 
            assert ax.type == 'bare'
            ds.attrs['{}_axis'.format(ax.name)] = num
            ds.attrs['{}_bins'.format(ax.name)] = ax.bins
            ds.attrs['{}_min'.format(ax.name)] = ax.min
            ds.attrs['{}_max'.format(ax.name)] = ax.max

    def __check_consistency(self, other): 
        for axis in self._axes: 
            if not self._axes[axis] == other._axes[axis]: 
                raise ValueError("tried to add non-equal hists")

    def __add__(self, other): 
        self.__check_consistency(other)
        new = HistNd()
        new._axes = copy.deepcopy(self._axes)
        new._array = self._array + other._array
        return new

    def __mul__(self, value): 
        new = HistNd()
        new._axes = copy.deepcopy(self._axes)
        if isinstance(value,HistNd): 
            self.__check_consistency(value)
            used = np.isfinite(self._array) * np.isfinite(value._array)
            new._array = np.zeros(self._array.shape)
            new._array[used] = self._array[used] * value._array[used]
        else: 
            new._array = self._array * value
        return new

    def __rmul__(self, value): 
        return self * value

    def __div__(self, value): 
        old_err = np.seterr(divide='ignore')
        denom = value**(-1)
        np.seterr(**old_err)
        return self * denom

    def __pow__(self, value): 
        new = HistNd()
        new._axes = copy.deepcopy(self._axes)
        new._array = self._array**value
        return new

    @property
    def axes(self): 
        return copy.deepcopy(self._axes).values()

    def integrate(self, axis=None, reverse=False): 
        if axis is None: 
            for axis in self._axes: 
                self.integrate(axis, reverse)
        else: 
            if isinstance(axis, int): 
                ax_number = axis
            else: 
                if not axis in self._axes: 
                    raise ValueError('axis {} not defined'.format(axis))
                ax_number = self._axes[axis].number
            a = self._array
            if reverse: 
                a = a.swapaxes(0,ax_number)[::-1,...].swapaxes(0,ax_number)
            a = np.cumsum(a, axis=ax_number)
            if reverse: 
                a = a.swapaxes(0,ax_number)[::-1,...].swapaxes(0,ax_number)
            self._array = a
            self._axes[axis].type = 'integral'


    def reduce(self, axis): 
        """
        Remove an axis. If the axis has been integrated replace with the 
        maximum bin, otherwise replace with the sum. 

        TODO: consider making this return reduced HistNd (rather than 
        modifying in place)
        """
        ax = self._axes.pop(axis)
        if ax.type == 'bare': 
            self._array = np.sum(self._array, axis=ax.number)
        elif ax.type == 'integral': 
            self._array = np.amax(self._array, axis=ax.number)
        else: 
            raise ValueError(
                'an axis somehow got undefined type: {}'.format(ax.type))
        for ax_name in self._axes: 
            if self._axes[ax_name].number > ax.number: 
                self._axes[ax_name].number -= 1

    def project_1d(self, axis): 
        """
        get (array, extent) tuple for axis
        """
        todo = [a.name for a in self.axes]
        victim = copy.deepcopy(self)
        for red_ax in todo: 
            if red_ax == axis: continue
            victim.reduce(red_ax)
        
        assert len(victim.axes) == 1
        the_ax = victim.axes[0]
        return victim._array, (the_ax.min, the_ax.max)

    def write_to(self, hdf_fg, name): 
        self.__to_hdf(hdf_fg, name)

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
