import numpy as np
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
    def rebin(self, n_bins): 
        center_bins = self._array[1:-1]
        n_old = len(center_bins)
        if not n_old % n_bins == 0: 
            raise ValueError("rebinning: {} doesn't go into {}".format(
                    n_bins, n_old))
        newcenter = center_bins.reshape((-1, n_bins)).sum(axis=1)
        newhist = np.zeros(n_old / n_bins + 2)
        newhist[0] = self._array[0]
        newhist[1:-1] = newcenter
        newhist[-1] = self._array[-1]
        self._array = newhist
        
    def average_bins(self, bins): 
        """
        either average bin range or all bins. 
        
        TODO: rework to average all bins between successive list entries. 
        """
        if isinstance(bins, int): 
            lbin = 1
            for hbin in xrange(bins + 1, self._array.size, bins): 
                ave_bins = np.arange(lbin, hbin)
                lbin = hbin
                self.average_bins(ave_bins)
        ave = np.average(self._array[bins])
        self._array[bins] = ave
    def get_xy_pts(self): 
        """
        returns xy pairs which can be fed to plt.plot() to generate a 
        step plot
        """
        y_vals = self._array[1:-1] # no overflow
        double_y_vals = np.dstack((y_vals,y_vals)).flatten()
        x_vals = np.linspace(*self.extent, num=len(y_vals) + 1)
        double_x_vals = np.dstack((x_vals,x_vals)).flatten()[1:-1]

        return double_x_vals, double_y_vals

    def get_data_xy(self): 
        """
        returns points at the bin centers
        """
        y_vals = self._array[1:-1]
        n_pts = len(y_vals)*2 + 1
        x_vals = np.linspace(*self.extent, num=n_pts)[1:-1:2]
        return x_vals, y_vals

class HistNd(object): 
    """
    Wrapper for multidimensional array. This class exists to simplify 
    conversion from HDF5 array to a 1d or 2d hist which can be plotted. 
    """
    class Axis(object): 
        def __init__(self): 
            self._name = None
            self._bins = None
            self._min = None
            self._max = None
            self._number = None
            self._type = 'bare'
            self._units = ''
        def __eq__(self, other): 
            span = self.max - self.min
            span_diff = (self.max - other.max)**2 + (self.min - other.min)**2
            conditions = [ 
                self.name == other.name, 
                self.bins == other.bins, 
                self.number == other.number, 
                span_diff / span**2 < 1e-6, 
                self.type == other.type, 
                self.units == other.units, 
                ]
            return all(conditions)
        def __ne__(self, other): 
            not self == other

        @property
        def name(self): 
            return self._name
        @property
        def bins(self): 
            return self._bins
        @property
        def min(self): 
            return self._min
        @property
        def max(self): 
            return self._max
        @property
        def number(self): 
            return self._number
        @property
        def type(self): 
            return self._type
        @property
        def units(self): 
            return self._units

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

        @property
        def extent(self): 
            return self.min, self.max

        def get_bin_extent(self, num): 
            bin_bounds = np.linspace(self.min,self.max,self.bins + 1)
            if num == 0: 
                return -np.inf, bin_bounds[0]
            elif num == self.bins + 1: 
                return bin_bounds[self.bins], np.inf
            else: 
                return bin_bounds[num - 1], bin_bounds[num]

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
            ds.attrs['{}_axis'.format(ax.name)] = num
            ds.attrs['{}_bins'.format(ax.name)] = ax.bins
            ds.attrs['{}_min'.format(ax.name)] = ax.min
            ds.attrs['{}_max'.format(ax.name)] = ax.max
            ds.attrs['{}_type'.format(ax.name)] = ax.type
            ds.attrs['{}_units'.format(ax.name)] = ax.units

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
            try: 
                new._array[used] = self._array[used] * value._array[used]
            except IndexError: 
                new._array = self._array * value._array
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
    def array(self): 
        """
        probably never should have made _array private
        """
        return self._array

    @property
    def axes(self): 
        """
        Return a sorted list of the axis objects. 
        """
        return sorted(self._axes.values(), key=lambda x: x.number)

    def sum(self): 
        return self._array.sum()

    def integrate(self, axis=None, reverse=False): 
        # this should be made an axis method
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

            if self._axes[axis].type == 'integral': 
                return None
                
            a = self._array
            if reverse: 
                a = a.swapaxes(0,ax_number)[::-1,...].swapaxes(0,ax_number)
            a = np.cumsum(a, axis=ax_number)
            if reverse: 
                a = a.swapaxes(0,ax_number)[::-1,...].swapaxes(0,ax_number)
            self._array = a
            self._axes[axis].type = 'integral'


    def cut(self, axis, value, reverse=False): 
        """
        cut axis at (approximately) value, return value where cut is 
        actually made. 

        The hist must be integrated and looses a dimension.
        """
        assert not reverse
        ax = self._axes.pop(axis)
        assert ax.type == 'integral'

        bin_bounds = np.linspace(ax.min,ax.max,ax.bins + 1)
        bin_n = np.digitize([value], bin_bounds)[0]
        
        a = self._array.swapaxes(0, ax.number)
        a = np.rollaxis(a, ax.number, 1)
        a = a[bin_n,...] 
        self._array = a
        for ax_name in self._axes: 
            if self._axes[ax_name].number > ax.number: 
                self._axes[ax_name].number -= 1

        if bin_n == 0: 
            return float('-inf')
        else: 
            return bin_bounds[bin_n - 1]
        
    def reduce(self, axis): 
        """
        Remove an axis. If the axis has been integrated replace with the 
        maximum bin, otherwise replace with the sum. 

        TODO: consider making this return reduced HistNd (rather than 
        modifying in place)

        TODO: replace with a del method in the axes
        
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
                self._axes[ax_name]._number -= 1

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

    def project_imshow(self, xaxis, yaxis): 
        """
        returns a dict that works with imshow
        """

        todo = [a.name for a in self.axes]
        victim = copy.deepcopy(self)
        for red_ax in todo: 
            if red_ax in [xaxis, yaxis]: continue
            victim.reduce(red_ax) 
        
        array = victim._array[1:-1,1:-1]

        ax_dic = dict((a.name, a) for a in self.axes)
        x_ax = ax_dic[xaxis]
        y_ax = ax_dic[yaxis]

        if x_ax.number > y_ax.number: 
            array = array.T

        extent = [
            x_ax.min, x_ax.max, 
            y_ax.min, y_ax.max, 
            ]
        
        return {
            'X': array.T, 
            'extent': extent, 
            'aspect': 'auto', 
            'interpolation':'nearest', 
            'origin':'lower', 
            }
    
    def max_bin_dict(self): 
        global_max = np.argmax(self._array)
        max_bin_tuple = np.unravel_index(global_max,self._array.shape)
        max_bins = {}
        return {a.name: t for a,t in zip(self.axes, max_bin_tuple)}

    def max_bins(self): 
        """
        someday this should procduce tuples of (name, extent)
        """
        raise NotImplementedError("you got to write this")

    def write_to(self, hdf_fg, name): 
        self.__to_hdf(hdf_fg, name)

