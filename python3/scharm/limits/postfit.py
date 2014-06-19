import h5py
import numpy as np
from collections import defaultdict

def split_to_planes(input_list):
    """
    splits list of points into dict keyed by non-plane things
    """
    the_dict = defaultdict(list)
    key_values = ('tag_config', 'signal_point')
    for point in input_list:
        the_key = tuple(point[x] for x in key_values)
        the_dict[the_key].append(
            {k:v for k,v in point.iteritems() if k not in key_values})
    return dict(the_dict)       # make non-default

def numpy_plane_from_dict(input_list):
    """
    converts points in a list (composed of dicts) to an numpy array.
    Extents are given by extrapolating one bin above the largest.
    """
    axes = ('leading_jet_pt','met')
    ax_values = []
    ax_dims = []
    for axis in axes:
        def val_iter(list_o_dicts):
            for d in list_o_dicts:
                yield d[axis]
        all_values = np.fromiter(val_iter(input_list),'i')
        unique_values = np.unique(all_values)
        ax_values.append(unique_values)
        ax_dims.append(unique_values.size)

    val_plane = np.ones(ax_dims)*-1
    for entry in input_list:
        indices = []
        for ax_n, ax_name in enumerate(axes):
            index = np.searchsorted(ax_values[ax_n],entry[ax_name])
            indices.append(index)
        idx = tuple(indices)
        if np.any(val_plane[idx] > 0):
            print val_plane[idx]
        val_plane[idx] = entry['upper_limit']['mean']

    extents = []
    for axvals in ax_values:
        extents.append(axvals[0])
        bin_size = np.unique(np.diff(axvals))
        if not bin_size.size == 1:
            raise ArithmeticError('bins not evenly spaced, sizes: {}'.format(
                    np.diff(axvals)))
        upper = axvals[-1] + bin_size[0]
        extents.append(upper)
    return val_plane, extents, axes
