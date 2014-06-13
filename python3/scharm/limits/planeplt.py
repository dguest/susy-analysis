"""
Exclusion plane plotter
"""
import numpy as np
from matplotlib.figure import Figure
from matplotlib.backends.backend_agg import FigureCanvasAgg as FigCanvas
from scharm.style import vdict, hdict
from scipy.interpolate import LinearNDInterpolator
from matplotlib.lines import Line2D
from matplotlib.colors import colorConverter
from os.path import dirname

class CLsExclusionPlane:
    """Scharm to Charm exclusion plane"""
    # plot limits
    xlim = (100.0, 600.0)
    ylim = (0.0,   400.0)

    # limits on internal interpolated grid
    low_x = 80
    low_y = 0

    def __init__(self, threshold=0.05, **argv):
        width = 9.0
        height = width*argv.get('aspect_ratio',8.0/width)
        self.figure = Figure(figsize=(width,height))
        self.canvas = FigCanvas(self.figure)
        self.ax = self.figure.add_subplot(1,1,1)
        self.ax.grid(True)
        self.ax.tick_params(labelsize=16)
        self.ax.set_ylabel(r'$m_{\mathrm{lsp}}$ [GeV]', **vdict)
        self.ax.set_xlabel(r'$m_{\tilde{c}}$ [GeV]', **hdict)
        self.colors = list('rgbmc') + ['orange']
        self.used_colors = set()
        self._proxy_contour = []
        self.lw = 3
        self._pts = None
        self._threshold = threshold

    def _get_style(self, style_string=''):
        if not style_string:
            for color in self.colors:
                if not color in self.used_colors:
                    self.used_colors.add(color)
                    return color, 'solid'

        line_style = 'dashed' if '-' in style_string else 'solid'
        the_color = style_string.replace('-','')
        self.used_colors.add(the_color)
        return the_color, line_style


    def add_config(self, stop_lsp_cls, label, style=None):
        """
        Expects a list of (mass stop, mass lsp, upper limit) tuples.
        """
        point_lables = None
        if len(stop_lsp_cls[0]) > 3:
            byvar = list(zip(*stop_lsp_cls))
            stop_lsp_cls = zip(*byvar[:3])
            point_lables = byvar[3]
        slu = np.array(list(stop_lsp_cls))
        x, y, z = slu.T
        xmin, xmax = self.low_x, max(x)
        ymin, ymax = self.low_y, max(y)
        xpts = 100

        xp, yp, zp = _get_interpolated_xyz(
            x, y, z, (xmin, xmax), (ymin, ymax), xpts)
        zp[(xp - yp) < 100] = self._threshold*1.1
        zp[np.isnan(zp) & (yp > self.ylim[0])] = self._threshold*1.2
        extent = [xmin, xmax, ymin, ymax]
        ct_color, ct_style = self._get_style(style)
        draw_opts = dict(color=ct_color, linewidth=self.lw,
                         linestyle=ct_style)
        ct = self.ax.contour(
            xp, yp, zp, [self._threshold],
            colors=ct_color, linewidths=self.lw, linestyles=ct_style )
        self._proxy_contour.append(
            ( Line2D((0,0),(0,1), **draw_opts), str(label)) )
        if point_lables:
            self._add_point_labels(x, y, point_lables)
        elif not self._pts:
            inpts = (x > xmin) & (y > ymin)
            self._pts, = self.ax.plot(x[inpts],y[inpts],'.k')
            self._proxy_contour.insert(0,(self._pts, 'signal points'))

    def _add_point_labels(self, x, y, point_lables):
        xy = {l: [] for l in point_lables}
        for x, y, lab in zip(x, y, point_lables):
            xy[lab].append((x, y))
        for lab, ptlist in sorted(xy.items()):
            x, y = np.array(ptlist).T
            inpts = (x > self.low_x) & (y > self.low_y)
            pts, = self.ax.plot(x[inpts], y[inpts], '.', label=lab,
                                markersize=20)
            self._proxy_contour.append((pts, lab))
        self._pts = True

    def add_band(self, stop_lsp_low_high, color=None):
        """
        Expects a list of (mass stop, mass lsp, upper limit) tuples.
        """
        x, y, low, high = np.array(stop_lsp_low_high).T
        xmin, xmax = self.low_x, max(x)
        ymin, ymax = self.low_y, max(y)
        xpts = 200

        xp, yp, lowp = _get_interpolated_xyz(
            x, y, low, (xmin, xmax), (ymin, ymax), xpts)
        *nada, highp = _get_interpolated_xyz(
            x, y, high, (xmin, xmax), (ymin, ymax), xpts)
        th = self._threshold
        zp = np.maximum( (lowp - th), -(highp - th))

        zp[np.isnan(zp)] = 0.001
        extent = [xmin, xmax, ymin, ymax]
        draw_opts = dict(color=None, linewidth=self.lw,
                         linestyle='-')

        if not color:
            color = colorConverter.to_rgba('yellow')
        else:
            color = colorConverter.to_rgba(color, alpha=0.2)

        ct = self.ax.contourf(
            xp, yp, zp, [-1, 0],
            colors=[color])
        # self.ax.imshow(lowp, extent=extent, origin='lower',
        #                vmin=-0.3,vmax=2)
        if not self._pts:
            inpts = (x > xmin) & (y > ymin)
            self._pts, = self.ax.plot(x[inpts],y[inpts],'.k')
            self._proxy_contour.insert(0,(self._pts, 'signal points'))


    def add_labels(self):
        self.ax.text(0.7, 0.3,
                     '$\sqrt{s}\ =\ 8\ \mathrm{TeV}$',
                     transform=self.ax.transAxes, size=24)
        self.ax.text(0.6, 0.2,
                     '$\int\ \mathcal{L}\ dt\ =\ 20.3\ \mathrm{fb}^{-1}$',
                     transform=self.ax.transAxes, size=24)
        self.ax.text(0.7, 0.05, 'ATLAS', style='italic', weight='bold',
                     horizontalalignment='right',
                     transform=self.ax.transAxes, size=24)
        self.ax.text(0.7, 0.05, ' INTERNAL', style='italic',
                     horizontalalignment='left',
                     transform=self.ax.transAxes, size=24)

    def save(self, name):
        self.ax.set_ylim(*self.ylim)
        self.ax.set_xlim(*self.xlim)
        leg = self.ax.legend(
            *zip(*self._proxy_contour), fontsize='xx-large',
             loc='upper left', framealpha=0.0, numpoints=1)
        # may be able to use these to positon lables
        # (x1, y1),(x2,y2) = leg.get_bbox_to_anchor().get_points()
        pl_dir = dirname(name)
        if pl_dir:
            os.makedirs(pl_dir)
        self.canvas.print_figure(name, bbox_inches='tight')


def _get_interpolated_xyz(x, y, z, xlims, ylims, xpts, log=True):
    xmin, xmax = xlims
    ymin, ymax = ylims
    xi = np.linspace(*xlims, num=xpts)
    num_y = xpts * (ymax - ymin) // (xmax - xmin)
    yi = np.linspace(*ylims, num=num_y)
    xp, yp = np.meshgrid(xi, yi)

    pts = np.dstack((x,y)).squeeze()
    # take a log transform?
    lin = LinearNDInterpolator(pts, np.log(z) if log else z)
    interp_points = np.dstack((xp.flatten(), yp.flatten())).squeeze()
    zp = lin(interp_points).reshape(xp.shape)
    # transform back
    if log:
        zp = np.exp(zp)
    return xp, yp, zp
