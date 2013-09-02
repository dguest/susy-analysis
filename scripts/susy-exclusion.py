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
from os.path import dirname

from stop.bullshit import make_dir_if_none
from stop.style import vdict, hdict

from matplotlib.figure import Figure
from matplotlib.backends.backend_agg import FigureCanvasAgg as FigCanvas
import matplotlib.pyplot as plt
from mpl_toolkits.axes_grid1 import make_axes_locatable
from stop.style import vdict
from scipy.interpolate import Rbf, interp2d
from matplotlib.mlab import griddata
from matplotlib.lines import Line2D
from matplotlib.patches import Rectangle

def get_args(): 
    d = 'default: %(default)s'
    pl_parent = argparse.ArgumentParser(add_help=False)
    pl_parent.add_argument(
        '-e','--plot-ext', help='output extension', choices={'.pdf','.png'}, 
        default='.pdf')
    pl_parent.add_argument('-o','--out-dir', default='exclusion')

    parser = argparse.ArgumentParser(description=__doc__, parents=[pl_parent])
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
    ex_plane = ExclusionPlane()
    for sr_name, sp_group in hdf.iteritems(): 
        stop_lsp_ul = []
        sr_cuts = cuts_dict[sr_name]
        for sp_name, plane in sp_group.iteritems(): 
            part, mstop, mlsp = _get_part_mstop_mlsp(sp_name)
            upper_limit = _value_from_plane(plane, sr_cuts)
            stop_lsp_ul.append( (mstop, mlsp, upper_limit) )

        out_name = os.path.join(args.out_dir, sr_name + args.plot_ext)
        ex_plane.add_config(stop_lsp_ul, sr_name)
        # _plot_points(stop_lsp_ul, out_name)
    ex_plane.save(os.path.join(args.out_dir, 'comp' + args.plot_ext))

class ExclusionPlane(object): 
    def __init__(self): 
        self.figure = Figure(figsize=(9,8))
        self.canvas = FigCanvas(self.figure)
        self.ax = self.figure.add_subplot(1,1,1)
        self.ax.tick_params(labelsize=16)
        self.ax.set_ylabel('$m_{\mathrm{lsp}}$ [GeV]', **vdict)
        self.ax.set_xlabel('$m_{\mathrm{stop}}$ [GeV]', **hdict)
        self.color_itr = iter('rgbm')
        self._proxy_contour = []
        self.lw = 3
        self._pts = None
        
        
    def add_config(self, stop_lsp_ul, label): 
        x, y, z = zip(*stop_lsp_ul)
        xmin = 0.0
        ymin = 0.0
        xi = np.linspace(xmin, max(x), 100)
        yi = np.linspace(ymin, max(y), 100)
        xp, yp = np.meshgrid(xi, yi)
        delta = xp - yp
        mask = (delta < 0) | ( delta > 80.4)

        rbf = Rbf(x, y, z, function='linear')
        zp = rbf(xp, yp)
        # zp = griddata(x, y, z, xp, yp)

        zp[mask] = 3
        extent = [xmin, max(x), ymin,max(y)]
        ct_color = next(self.color_itr)
        draw_opts = dict(color=ct_color, linewidth=self.lw)
        ct = self.ax.contour(xp, yp, zp, [1.0], 
                             colors=ct_color, linewidths=self.lw )
        self._proxy_contour.append(
            ( Line2D((0,0),(0,1), **draw_opts), str(label)) )
        if not self._pts: 
            self._pts = self.ax.plot(x,y,'.k')

    def save(self, name): 
        self.ax.legend(
            *zip(*self._proxy_contour), fontsize='xx-large', 
             loc='upper left', framealpha=0.0)
        make_dir_if_none(dirname(name))
        self.canvas.print_figure(name, bbox_inches='tight')
        

def _plot_points(stop_lsp_ul, out_name): 
    figure = Figure(figsize=(9,8))
    canvas = FigCanvas(figure)
    ax = figure.add_subplot(1,1,1)
    divider = make_axes_locatable(ax)
    cax = divider.append_axes("right", "5%", pad="1.5%")
    x, y, z = zip(*stop_lsp_ul)
    xi = np.linspace(min(x), max(x), 100)
    yi = np.linspace(min(y), max(y), 100)
    xp, yp = np.meshgrid(xi, yi)
    delta = xp - yp
    mask = (delta < 0) | ( delta > 80.4)
    rbf = Rbf(x, y, z, function='linear')
    zp = rbf(xp, yp)
    zp[mask] = 3
    extent = [min(x), max(x), min(y),max(y)]
    im = ax.imshow(zp, vmin=0.0, vmax=2.0, origin='lower',
                   extent=extent)
    cb = plt.colorbar(im,cax=cax)
    ct = ax.contour(xp, yp, zp, [1.0])
    ax.plot(x,y,'o')

    make_dir_if_none(dirname(out_name))

    canvas.print_figure(out_name, bbox_inches='tight')

if __name__ == '__main__': 
    run()

    
