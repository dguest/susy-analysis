import numpy as np
from collections import defaultdict
import copy
import bisect
import weakref, warnings
from h5py import Group, Dataset

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
    def get_xy_step_pts(self): 
        """
        returns xy pairs which can be fed to plt.plot() to generate a 
        step plot
        """
        y_vals = self._array[1:-1] # no overflow
        double_y_vals = np.dstack((y_vals,y_vals)).flatten()
        x_vals = np.linspace(*self.extent, num=len(y_vals) + 1)
        double_x_vals = np.dstack((x_vals,x_vals)).flatten()[1:-1]

        return double_x_vals, double_y_vals

    def get_xy_center_pts(self): 
        """
        returns points at the bin centers
        """
        y_vals = self._array[1:-1]
        n_pts = len(y_vals)*2 + 1
        x_vals = np.linspace(*self.extent, num=n_pts)[1:-1:2]
        return x_vals, y_vals

class Axis(object): 
    """
    Intended as an interface to any given axis. 

    Has a reference to the Histogram that owns it, so operations 
    like 'axis.remove()' will alter the parent histogram. 
    """
    def __init__(self): 
        self._name = None
        self._bins = None
        self._min = None
        self._max = None
        self._axis = None
        self._type = 'bare'
        self._units = ''
        self._hist = None
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

    def __deepcopy__(self, memo): 
        new = type(self)()
        for name, val in self.__dict__.iteritems(): 
            new.__dict__[name] = copy.deepcopy(val, memo)
        new._hist = None
        return new

    def remove(self): 
        hist = self._hist
        if hist: 
            hist._reduce(self.name)

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
        return self._axis
    @property
    def type(self): 
        return self._type
    @property
    def units(self): 
        return self._units

    @property
    def valid(self): 
        if not self._hist: 
            return False
        hist = self._hist
        if not hist: 
            return False
        conditions = [ 
            self.name, 
            self.bins, 
            self.min is not None, 
            self.max is not None, 
            self.number is not None, 
            hist._array.shape[self.number] == self.bins + 2, 
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

    def slice(self, low, high, tolerance=1e-5): 
        """
        Finds closest bins, slices those (or throws error). 
        Intended to be a replacement for the get_slice() method below. 
        """
        bin_bounds = np.linspace(self.min,self.max,self.bins + 1)
        def get_delta2_and_bin_n(value): 
            if value == float('-inf'): 
                return 0.0, 0
            elif value == float('inf'): 
                return 0.0, self.bins + 2
            deltas = (bin_bounds - value)**2 
            bin_n = deltas.argmin() + 1
            delta2 = deltas.min()
            return delta2, bin_n
            
        low_delta, low_bin = get_delta2_and_bin_n(low)
        high_delta, high_bin = get_delta2_and_bin_n(high)
        bin_width = (self.max - self.min) / (self.bins + 1)
        total_rel_delta = low_delta + high_delta / bin_width**2

        if abs(total_rel_delta > tolerance): 
            raise ValueError(
                'in bounds {} too far from bin bounds {}'.format(
                    (low,high), (bin_bounds[low_bin], bin_bounds[high_bin])))

        return self._hist._get_slice(self.name, low_bin, high_bin)

    def get_slice(self, val, upper_val=None): 
        """
        Get sub-histogram by slicing the axis at this value. 

        returns a tuple (histogram, (low, high)) where low and high 
        are the bounds of the slice taken. 
        """
        bin_bounds = np.linspace(self.min,self.max,self.bins + 1)
        bin_n = bisect.bisect(bin_bounds, val)
        extent = self._get_bounds(bin_bounds, bin_n)
        if upper_val is not None: 
            if not upper_val > val: 
                raise ValueError('upper_val must be > val')
            # should add 1 to ensure that the upper bin is counted
            upper_bin = bisect.bisect(bin_bounds, upper_val) + 1
            upper_extent = self._get_bounds(bin_bounds, upper_bin)
            extent = (extent[0], upper_extent[1])
        else: 
            upper_bin = None

        # _get_slice should take care of copying
        subhist = self._hist._get_slice(self.name, bin_n, upper_bin)
        return subhist, extent
        
    def _get_bounds(self, bin_bounds, num): 
        if num == 0: 
            return -np.inf, bin_bounds[0]
        elif num == self.bins + 1: 
            return bin_bounds[self.bins], np.inf
        else: 
            return bin_bounds[num - 1], bin_bounds[num]

    def get_bin_extent(self, num): 
        bin_bounds = np.linspace(self.min,self.max,self.bins + 1)
        return self._get_bounds(bin_bounds, num)
    
    def integrate(self, reverse=False, in_place=False): 
        """
        NOTE: should replace with something that returns a new hist. 
        """
        if not in_place: 
            warnings.warn(
                "The default behavior of this function will change, "
                "set 'in_place' to True to preserve", FutureWarning, 
                stacklevel=2)
        self._hist._integrate(self.name, reverse)


class HistNd(object): 
    """
    Wrapper for multidimensional array. This class exists to simplify 
    conversion from HDF5 array to a 1d or 2d hist which can be plotted. 
    """

    def __init__(self,array=None): 
        self._axes = {}
        if array == 'example': 
            self._init_example()
        elif array: 
            self.__from_hdf(array)

    def __copy__(self): 
        new = type(self)()
        new.__dict__.update(self.__dict__)
        new._update_axes()
        return new
    def __deepcopy__(self, memo): 
        new = type(self)()
        for name, val in self.__dict__.iteritems(): 
            new.__dict__[name] = copy.deepcopy(val, memo)
        new._update_axes()
        return new
 
    def _update_axes(self): 
        for ax in self._axes.itervalues(): 
            ax._hist = self

    def __from_hdf(self, hdf_array): 
        self._array = np.array(hdf_array)
        for name, atr in hdf_array.attrs.items(): 
            ax_name, part, ax_prop = name.rpartition('_')
            the_axis = self._axes.get(ax_name, Axis())
            setattr(the_axis, '_' + ax_prop, atr)
            the_axis._name = ax_name
            the_axis._hist = self
            self._axes[ax_name] = the_axis

        for name, axis in self._axes.items(): 
            if not axis.valid: 
                atrs = ', '.join(
                    ['{}:{}'.format(k,v) for k,v in axis.__dict__.items()])
                raise IOError("{} isn't well defined: {}".format(name, atrs))

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

    def _init_example(self): 
        self._array = np.arange(-1,11) + np.arange(-10,110,10).reshape((-1,1))
        for ax_n, ax_name in enumerate('xy'): 
            axis = Axis()
            axis._axis = ax_n
            axis._name = ax_name
            axis._bins = 10
            axis._min = 0.0
            axis._max = 1.0
            axis._untis = ''
            axis._type = 'bare'
            axis._hist = self
            self._axes[ax_name] = axis
            assert axis.valid

    def __check_consistency(self, other): 
        for axis in self._axes: 
            if not self._axes[axis] == other._axes[axis]: 
                raise ValueError("tried to add non-equal hists")

    def __add__(self, other): 
        self.__check_consistency(other)
        new = HistNd()
        new._axes = copy.deepcopy(self._axes)
        new._update_axes()
        new._array = self._array + other._array
        return new

    def __mul__(self, value): 
        new = HistNd()
        new._axes = copy.deepcopy(self._axes)
        new._update_axes()
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
        new._update_axes()
        new._array = self._array**value
        return new

    def __getitem__(self, ax_key): 
        return self.axes[ax_key]
    
    def __delitem__(self, ax_key): 
        self._reduce(ax_key)

    @property
    def array(self): 
        """
        probably never should have made _array private
        """
        return self._array

    def axlist(self): 
        return sorted(self._axes.values(), key=lambda x: x.number)
    
    @property 
    def axes(self): 
        return self._axes

    def sum(self): 
        return self._array.sum()

    def integrate(self, axis=None, reverse=False): 
        warnings.warn("integrate will be made an axis method", FutureWarning, 
                      stacklevel=2)
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
            self._integrate(ax_number, reverse)

    def _integrate(self, ax_name, reverse=False): 
        if self.axes[ax_name].type == 'integral': 
            raise ArithmeticError("tried to integrate axis {} twice".format(
                    ax_name))
        ax_number = self._axes[ax_name].number
        a = self._array
        if reverse: 
            a = a.swapaxes(0,ax_number)[::-1,...].swapaxes(0,ax_number)
        a = np.cumsum(a, axis=ax_number)
        if reverse: 
            a = a.swapaxes(0,ax_number)[::-1,...].swapaxes(0,ax_number)
        self._array = a
        self.axes[ax_name]._type = 'integral'


    def cut(self, axis, value, reverse=False): 
        """
        cut axis at (approximately) value, return value where cut is 
        actually made. 

        The hist must be integrated and looses a dimension.
        """

        warnings.warn("this will be removed, use Axis.slice() instead", 
                      FutureWarning, stacklevel=2)

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
        
    def _pop_axis(self, name): 
        """
        be careful: doesn't do anything to the array, should be called 
        from _reduce or _slice
        """
        if not name in self._axes: 
            raise ValueError('axis {} not in hist'.format(name))
        ax = self._axes.pop(name)
        # FIXME: replace this with some other cleaner shifting function
        for ax_name in self._axes: 
            if self._axes[ax_name].number > ax.number: 
                self._axes[ax_name]._axis -= 1
        return ax

    def _reduce(self, axis): 
        """
        Remove an axis. If the axis has been integrated replace with the 
        maximum bin, otherwise replace with the sum. 
        """
        ax = self._pop_axis(axis)
        if ax.type == 'bare': 
            self._array = np.sum(self._array, axis=ax.number)
        elif ax.type == 'integral': 
            self._array = np.amax(self._array, axis=ax.number)
        else: 
            raise ValueError(
                'an axis somehow got undefined type: {}'.format(ax.type))

    def _get_slice(self, axis, bin_n, upper_bin=None): 
        new_axes = copy.deepcopy(self._axes)
        sl_tuple = [slice(None)]*len(new_axes)
        ax = new_axes.pop(axis)
        if upper_bin is None: 
            upper_bin = bin_n + 1
        sl_tuple[ax.number] = slice(bin_n, upper_bin)
        new_array = np.array(self._array[sl_tuple])
        new_array = new_array.sum(ax.number)

        # FIXME: replace with something cleaner as above
        for ax_name in new_axes: 
            if new_axes[ax_name].number > ax.number: 
                new_axes[ax_name]._axis -= 1
        new_hist = HistNd()
        new_hist._array = new_array
        new_hist._axes = new_axes
        new_hist._update_axes()
        if not new_hist.axes: 
            assert new_hist.array.size == 1
            new_hist = new_hist.array
        return new_hist

    def project_1d(self, axis): 
        """
        get (array, extent) tuple for axis
        """
        victim = copy.deepcopy(self)
        for red_ax in self.axes: 
            if red_ax == axis: continue
            victim._reduce(red_ax)
        
        assert len(victim.axes) == 1
        the_ax = victim.axes[axis]
        return victim._array, (the_ax.min, the_ax.max)

    def project_imshow(self, xaxis, yaxis): 
        """
        returns a dict that works with imshow
        """

        victim = copy.deepcopy(self)
        for red_ax in self.axes: 
            if red_ax in [xaxis, yaxis]: continue
            victim._reduce(red_ax) 
        
        array = victim._array[1:-1,1:-1]

        x_ax = self.axes[xaxis]
        y_ax = self.axes[yaxis]

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

class HistAdder(object): 
    """
    Generic histogram adder. Traverses the first given file to map out 
    histograms, then finds the corresponding hists for each call to add 
    and adds them. 
    """
    def __init__(self, base_group, weight=1.0): 
        self.hists = self._search(base_group, weight)
        
    def _search(self, group, weight): 
        subhists = {}
        for key, subgroup in group.iteritems(): 
            if isinstance(subgroup, Group): 
                subhists[key] = self._search(subgroup, weight)
            elif isinstance(subgroup, Dataset): 
                subhists[key] = HistNd(subgroup)*weight
            else: 
                raise HistAddError('not sure what to do with {} {}'.format(
                        type(subgroup), key))
        return subhists
    def _merge(self, hist_dict, new_hists, weight): 
        merged = {}
        for key, subgroup in hist_dict.iteritems(): 
            if not key in new_hists: 
                raise HistAddError(
                    "node {} not found in new hists".format(key))
            if isinstance(subgroup, dict): 
                merged[key] = self._merge(subgroup, new_hists[key], weight)
            elif isinstance(subgroup, HistNd): 
                if not isinstance(new_hists[key], Dataset): 
                    raise HistAddError(
                        "tried to merge non-dataset {}".format(key))
                new_hist = HistNd(new_hists[key])*weight
                merged[key] = subgroup + new_hist
            else: 
                raise HistAddError('not sure what to do with {}, {}'.format(
                        type(subgroup), key))
        return merged

    def _write(self, hists, group): 
        for key, hist in hists.iteritems(): 
            if isinstance(hist, dict): 
                subgrp = group.create_group(key)
                self._write(hist, subgrp)
            else: 
                hist.write_to(group, key)

    def add(self, group, weight=1.0): 
        self.hists = self._merge(self.hists, group, weight)

    def write_to(self, group): 
        self._write(self.hists, group)
        
    def dump(self, group=None, base=''): 
        if not group: 
            group = self.hists
        for key, subgroup in group.iteritems(): 
            path = '/'.join([base, key])
            if isinstance(subgroup, dict): 
                self.dump(subgroup, path)
            else: 
                print path, subgroup.array.sum()

class HistAddError(StandardError): 
    def __init__(self, args): 
        super(HistAddError, self).__init__(args)


class HistToBinsConverter(object): 
    """
    Initalized with a selection of bins in a window. 
    Converts count, extent arrays into binned values. 
    """
    def __init__(self, bins): 
        self.bins = bins
        self.error_threshold = 1e-4

    def _check_bins(self, in_centers): 
        """
        checks to make sure the bins are aligned in the input / output. 
        """
        in_delta = np.mean(np.diff(in_centers))
        bin_lows = in_centers - in_delta / 2.0
        bin_highs = in_centers + in_delta / 2.0
        
        bins = np.array(self.bins)
        binlow_off_matrix = bins[:-1].reshape((-1,1)) - bin_lows
        binhigh_off_matrix = bins[1:].reshape((-1,1)) - bin_highs
        low_offset2 = np.min(binlow_off_matrix**2, -1)
        high_offset2 = np.min(binhigh_off_matrix**2, -1)
        max_rel_high = np.max(low_offset2 / in_delta)
        max_rel_low = np.max(high_offset2 / in_delta)

        max_rel_off = max(max_rel_low, max_rel_high)
        if max_rel_off > self.error_threshold: 
            raise ValueError(
                'rel bin misalignment of {} (max allowed: {})'.format(
                    max_rel_off, self.error_threshold))

    def get_bin_counts(self, counts_array, extent, 
                       overflows_included=True): 
        """
        returns (underflow, x_center, x_err, y_sum, overflow). 
        Assumes the first and last entries in array are under / overflow, 
        unless overflows_included == False. 
        """
        if overflows_included: 
            array = counts_array[1:-1]
            under = counts_array[0]
            over = counts_array[-1]
        else: 
            array = counts_array
            under = 0.0
            over = 0.0

        in_centers = np.linspace(*extent, num=(len(array)*2 + 1))[1:-1:2]
        self._check_bins(in_centers)
        counts, edges = np.histogram(
            in_centers, bins=self.bins, weights=array)
        x_center = np.vstack([edges[1:], edges[:-1]]).mean(0)
        x_err = np.diff(edges) / 2.0
        under += array[in_centers < edges[0]].sum() 
        over += array[in_centers >= edges[-1]].sum() 
        return under, x_center, x_err, counts, over
