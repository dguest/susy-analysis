import h5py, numpy
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
    converts points in a list (composed of dicts) to an h5 array
    """
    
