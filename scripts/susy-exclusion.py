#!/usr/bin/env python2.7
"""
Top level for routines to draw excluded contour. Cuts used are defined in 
yaml_cuts. To make fancy names for regions, use the 'fancy_name' key. 

Colors can be given in draw_style, which will also make dashes 
if a '-' appears. 
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
from scipy.interpolate import LinearNDInterpolator
from matplotlib.lines import Line2D
from matplotlib.patches import Rectangle
from matplotlib.font_manager import FontProperties

def get_args(): 
    d = 'default: %(default)s'
    pl_parent = argparse.ArgumentParser(add_help=False)
    pl_parent.add_argument(
        '-e','--plot-ext', help='output extension', 
        choices={'.pdf','.png','.eps', '.txt'}, default='.pdf')
    pl_parent.add_argument(
        '-r','--aspect-ratio', nargs='?', type=float, default=8.0/9.0,
        const=0.64, 
        help=("defaults to %(default).2f, "
              "const value is %(const).2f"))
    pl_parent.add_argument('-o','--out-dir', default='exclusion')
    pl_parent.add_argument('-n','--out-name', default='comb')

    parser = argparse.ArgumentParser(
        description=__doc__, parents=[pl_parent], 
        formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument('hdf_input')
    parser.add_argument('yaml_cuts')
    parser.add_argument('-t','--threshold', default=1.0, type=float, help=d)
    parser.add_argument('--heat', action='store_true')
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
    plot_args = dict(
        threshold=args.threshold, 
        aspect_ratio=args.aspect_ratio, 
        signal_prefix='stop')
    with open(args.yaml_cuts) as cuts_yml: 
        signal_regions = yaml.load(cuts_yml)
    if args.plot_ext == '.txt': 
        ex_plane = ExcludedList(**plot_args)
    else: 
        ex_plane = ExclusionPlane(**plot_args)
    for signal_region in signal_regions: 
        sr_name = signal_region['region_key']
        sp_group = hdf[sr_name]
        stop_lsp_ul = []
        for sp_name, plane in sp_group.iteritems(): 
            part, mstop, mlsp = _get_part_mstop_mlsp(sp_name)
            upper_limit = _value_from_plane(plane, signal_region)
            stop_lsp_ul.append( (mstop, mlsp, upper_limit) )

        out_name = os.path.join(args.out_dir, sr_name + args.plot_ext)
        contour_name = signal_region.get('fancy_name',sr_name)
        style = signal_region.get('draw_style', None)
        ex_plane.add_config(stop_lsp_ul, contour_name, style)
        if args.heat: 
            _plot_points(stop_lsp_ul, out_name)

    ex_plane.save(os.path.join(args.out_dir, args.out_name + args.plot_ext))

class ExcludedList(object): 
    def __init__(self, threshold, **argv): 
        self._threshold = threshold
        self._excluded = set()
        def make_signame(mstop, mlsp): 
            return '{}-{}-{}'.format(argv['signal_prefix'], mstop, mlsp)
        self.make_signame = make_signame
    def add_config(self, stop_lsp_ul, label, style=None): 
        for x, y, z in stop_lsp_ul: 
            if z < self._threshold: 
                self._excluded.add(self.make_signame(x,y))
    def save(self, name): 
        make_dir_if_none(dirname(name))
        with open(name,'w') as output: 
            output.write('\n'.join(self._excluded) + '\n')

class ExclusionPlane(object): 
    def __init__(self, threshold=1.0, **argv): 
        width = 9.0
        height = width*argv.get('aspect_ratio',8.0/width)
        self.figure = Figure(figsize=(width,height))
        self.canvas = FigCanvas(self.figure)
        self.ax = self.figure.add_subplot(1,1,1)
        self.ax.grid(True)
        self.ax.tick_params(labelsize=16)
        self.ax.set_ylabel('$m_{\mathrm{lsp}}$ [GeV]', **vdict)
        self.ax.set_xlabel('$m_{\mathrm{stop}}$ [GeV]', **hdict)
        self.colors = list('rgbmc') + ['orange']
        self.used_colors = set()
        self._proxy_contour = []
        self.lw = 3
        self._pts = None
        self._threshold = threshold
        
    def _get_style(self, style_string): 
        if not style_string: 
            for color in self.colors: 
                if not color in self.used_colors: 
                    self.used_colors.add(color)
                    return color, 'solid'

        line_style = 'dashed' if '-' in style_string else 'solid'
        the_color = style_string.replace('-','')
        self.used_colors.add(the_color)
        return the_color, line_style

    def add_config(self, stop_lsp_ul, label, style=None): 
        """
        Expects a list of (mass stop, mass lsp, upper limit) tuples. 
        """
        low_x = 80
        low_y = 50
        slu = np.array(stop_lsp_ul)
        low_dummy = self._threshold*0.99
        add_pts = np.array([(5,0, low_dummy),(75, 0, low_dummy)])
        x, y, z = np.vstack((slu, add_pts)).T
        xmin, xmax = low_x, max(x)
        ymin, ymax = low_y, max(y)
        xpts = 100
        xi = np.linspace(xmin, xmax, xpts)
        yi = np.linspace(ymin, xmax, xpts * (ymax - ymin) // (xmax - xmin))
        xp, yp = np.meshgrid(xi, yi)
        delta = xp - yp
        mask = (delta < 0) | ( delta > 80.4)

        pts = np.dstack((x,y)).squeeze()
        lin = LinearNDInterpolator(pts, z)
        interp_points = np.dstack((xp.flatten(), yp.flatten())).squeeze()
        zp = lin(interp_points).reshape(xp.shape)

        zp[np.isnan(zp)] = self._threshold*1.001
        extent = [xmin, xmax, ymin, ymax]
        ct_color, ct_style = self._get_style(style)
        draw_opts = dict(color=ct_color, linewidth=self.lw, linestyle=ct_style)
        ct = self.ax.contour(
            xp, yp, zp, [self._threshold], 
            colors=ct_color, linewidths=self.lw, linestyles=ct_style )
        self._proxy_contour.append(
            ( Line2D((0,0),(0,1), **draw_opts), str(label)) )
        if not self._pts: 
            inpts = (x > xmin) & (y > ymin)
            self._pts, = self.ax.plot(x[inpts],y[inpts],'.k')
            self._proxy_contour.insert(0,(self._pts, 'signal points'))

    def _add_labels(self): 
        self.ax.text(0.7, 0.3, 
                     '$\sqrt{s}\ =\ 8\ \mathrm{TeV}$',
                     transform=self.ax.transAxes, size=24)
        self.ax.text(0.6, 0.2, 
                     '$\int\ \mathcal{L}\ dt\ =\ 20.3\ \mathrm{fb}^{-1}$',
                     transform=self.ax.transAxes, size=24)
        self.ax.text(0.7, 0.1, 'ATLAS', style='italic', weight='bold', 
                     horizontalalignment='right', 
                     transform=self.ax.transAxes, size=24)
        self.ax.text(0.7, 0.1, ' INTERNAL', style='italic', 
                     horizontalalignment='left', 
                     transform=self.ax.transAxes, size=24)

    def save(self, name): 
        leg = self.ax.legend(
            *zip(*self._proxy_contour), fontsize='xx-large', 
             loc='upper left', framealpha=0.0, numpoints=1)
        # may be able to use these to positon lables
        # (x1, y1),(x2,y2) = leg.get_bbox_to_anchor().get_points()
        self._add_labels()
        make_dir_if_none(dirname(name))
        self.canvas.print_figure(name, bbox_inches='tight')
        

def _plot_points(stop_lsp_ul, out_name): 
    figure = Figure(figsize=(9,8))
    canvas = FigCanvas(figure)
    ax = figure.add_subplot(1,1,1)
    divider = make_axes_locatable(ax)
    cax = divider.append_axes("right", "5%", pad="1.5%")
    ax.tick_params(labelsize=16)
    ax.set_ylabel('$m_{\mathrm{lsp}}$ [GeV]', **vdict)
    ax.set_xlabel('$m_{\mathrm{stop}}$ [GeV]', **hdict)

    x, y, z = zip(*stop_lsp_ul)
    xi = np.linspace(min(x), max(x), 100)
    yi = np.linspace(min(y), max(y), 100)
    xp, yp = np.meshgrid(xi, yi)
    delta = xp - yp
    mask = (delta < 0) | ( delta > 80.4)
    # rbf = Rbf(x, y, z, function='linear')
    # zp = rbf(xp, yp)
    pts = np.dstack((x,y))[0]
    lin = LinearNDInterpolator(pts, z)
    interp_points = np.dstack((xp.flatten(), yp.flatten()))[0]
    zp = lin(interp_points).reshape(xp.shape)
    # zp = griddata(x, y, z, xp, yp)
    # zp[mask] = 3
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

    
