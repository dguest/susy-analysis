#!/usr/bin/env python2.7

"""
Top level for routines to draw excluded contour. 
"""

import argparse
import sys, os, re
import yaml
import h5py
import numpy as np
import bisect

def _load_plotters(): 
    from matplotlib.backends.backend_agg import FigureCanvasAgg as FigureCanvas
    from matplotlib.figure import Figure
    import matplotlib.pyplot as plt
    from mpl_toolkits.axes_grid1 import make_axes_locatable
    import matplotlib as mp
    from stop.style import ax_labels, vdict, hdict

def get_args(): 
    d = 'default: %(default)s'
    pl_parent = argparse.ArgumentParser(add_help=False)
    pl_parent.add_argument(
        '-e','--plot-ext', help='output extension', choices={'.pdf','.png'}, 
        default='.pdf')
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('hdf_input')
    parser.add_argument('yaml_cuts')
    return parser.parse_args(sys.argv[1:])

def _get_part_mstop_mlsp(string): 
    the_re = re.compile('([^-]+)-([0-9]+)-([0-9]+)')
    match = the_re.search(string)
    mstop, mlsp = [int(x) for x in match.group(2,3)]
    part = match.group(1)
    return part, mstop, mlsp

def _value_from_plane(plane, sr_cuts): 
    extents = plane.attrs['extent']
    ax_names = plane.attrs['ax_names']
    ext_chunks = [extents[i:i+2] for i in xrange(0, len(extents), 2)]
    index = []
    for name, extent, n_bins in zip(ax_names, ext_chunks, plane.shape): 
        cut_val = sr_cuts[name]
        index.append(_bin_from_cut(cut_val, extent, n_bins))
    return plane[tuple(index)]

def _bin_from_cut(cutval, extent, n_bins): 
    bin_vals = np.linspace(*extent, num=n_bins, endpoint=False)
    index = bisect.bisect_left(bin_vals, cutval)
    if not bin_vals[index] == cutval: 
        raise ValueError('{} != {}'.format(bin_vals[index], cutval))
    return index

def run(): 
    args = get_args()
    hdf = h5py.File(args.hdf_input)
    with open(args.yaml_cuts) as cuts_yml: 
        cuts_dict = yaml.load(cuts_yml)
    for sr_name, sp_group in hdf.iteritems(): 
        stop_lsp_ul = []
        sr_cuts = cuts_dict[sr_name]
        for sp_name, plane in sp_group.iteritems(): 
            part, mstop, mlsp = _get_part_mstop_mlsp(sp_name)
            upper_limit = _value_from_plane(plane, sr_cuts)
            stop_lsp_ul.append( (mstop, mlsp, upper_limit) )
        

if __name__ == '__main__': 
    run()

    
