"""
Exclusion plane plotter
"""
from os.path import dirname
import os, math

import numpy as np

# for interpolation
from scipy.interpolate import LinearNDInterpolator
from scipy.stats import norm
from matplotlib.figure import Figure
from matplotlib.backends.backend_agg import FigureCanvasAgg as FigCanvas

from matplotlib.lines import Line2D
from matplotlib.patches import Patch, Polygon
from matplotlib.colors import colorConverter

from scharm.style import vdict, hdict
from scharm.limits import limitsty

# enums for kinematic bounds
BOTH = 'both'
UPPER = 'upper'
_allowed_kinbounds = {None, True, False, BOTH, UPPER}

# enums for region labels
from scharm.limits.limitsty import OBSERVED, EXPECTED

class CLsExclusionPlane:
    """Scharm to Charm exclusion plane"""
    # text sizes
    # big_size = 24
    # med_size = 18
    # small_size = 14
    big_size = 26
    med_size = 20
    small_size = 16

    # plot limits
    xlim = (0.0, 600.0)
    ylim = (0.0, 500.0)

    # limits on internal interpolated grid
    gev_per_bin = 5
    assert (xlim[1] - xlim[0]) % gev_per_bin == 0, 'non-int number of xbins'
    assert (ylim[1] - ylim[0]) % gev_per_bin == 0, 'non-int number of ybins'
    grid_xlim = xlim
    grid_ylim = (ylim[0] - gev_per_bin, ylim[1])

    # epsilon offset to put regions above / below threshold
    threshold_margin = 0.1
    zval_forbidden = 10

    # physical constants relating to the kinematic bounds
    _w_mass = 80.385
    _t_mass = 173.3
    _c_mass = 1.29
    _b_mass = 4.18

    # labels
    lsp = limitsty.lsp
    scharm = limitsty.scharm
    antischarm = limitsty.scharm.replace(r'{c}', r'{c}^{*}')
    stop = limitsty.stop

    # legend setup
    legend_properties = dict(
        loc='upper left', framealpha=0.0, numpoints=1)

    # special strings
    title_tmp = r' Direct ${c}{cb}$, ${c} \to c{l}$, single ${c}$ state'
    right_side_ul_info = 'Numbers give 95% CLs excluded cross section [fb]'
    ax_tmp = r'$m_{{ {} }}$ [GeV]'

    # text formatting
    x_fmt = dict(y=0.98, ha='right', fontsize=big_size)
    y_fmt = dict(x=0.98, ha='right', fontsize=big_size)

    # ticks
    major_pars = dict(length=10, labelsize=med_size)
    minor_pars = dict(length=5)

    def __init__(self, threshold=0.05, **argv):
        width = 9.0
        height = width*argv.get('aspect_ratio',8.0/width)
        self.figure = Figure(figsize=(width,height))
        self.canvas = FigCanvas(self.figure)
        self.ax = self.figure.add_subplot(1,1,1)

        self.ax.grid(argv.get('grid', True), alpha=0.03, ls='-')
        self.ax.tick_params(**self.major_pars)
        self.ax.minorticks_on()
        self.ax.tick_params(which='minor', **self.minor_pars)
        self.ax.set_ylabel(self.ax_tmp.format(self.lsp), **self.x_fmt)
        self.ax.set_xlabel(self.ax_tmp.format(self.scharm), **self.y_fmt)

        self.colors = list('rgbmc') + ['orange']
        self.ultxt = dict(fontsize=10, ha='center', va='bottom', color='grey')
        self.used_colors = set()

        self.lw = 1.5
        self.wideline = 3
        self.approved = False
        self.stage = 'Internal'

        interp = argv.get('interpolation','gauss')
        if interp not in interpolators:
            raise ValueError("no interpolator '{}'".format(interp))
        self._interpolator = interpolators[interp]

        self._proxy_contour = []
        self._pts = set()
        self._labeled_points = set()
        self._threshold = threshold
        self._finalized = False
        self._ax2 = None        # just for an added label
        self._labeled = False

        self._drawpoints = argv.get('show_points', True)
        self._kinbounds = argv.get('kinematic_bounds', True)
        assert self._kinbounds in _allowed_kinbounds

        hc = argv.get('high_contrast')
        self._kinbound_alpha = 0.7 if hc else 0.3
        self._mono_alpha = 0.3 if hc else 0.1

        self._fill_low_stop = argv.get('fill_low_stop', True)

    def _get_style(self, style_string=''):
        if not style_string:
            for color in self.colors:
                if not color in self.used_colors:
                    self.used_colors.add(color)
                    return color, 'solid'
        line_style = 'solid'
        if '-' in style_string:
            line_style = 'dashed'
        elif ':' in style_string:
            line_style = 'dotted'
        the_color = style_string.translate({ord(x):None for x in '-:'})
        self.used_colors.add(the_color)
        return the_color, line_style

    def add_observed(self, points):
        """
        Expects a list of `Point`s.
        """
        low = []
        med = []
        high = []
        for pt in points:
            low.append( (pt.ms, pt.ml, pt.obs_low) )
            med.append( (pt.ms, pt.ml, pt.obs) )
            high.append( (pt.ms, pt.ml, pt.obs_high) )
        line_opts = {'linewidths':self.wideline, 'colors': 'firebrick'}
        patch_opts = dict(ec='firebrick', fill=False, linestyle='dotted')
        label = OBSERVED
        self.add_config(low, style=':firebrick')
        self.add_config(med, add_draw_opts = line_opts, label=label)
        self.add_config(high, style=':firebrick')
        self._proxy_contour.append((Patch(**patch_opts), label))

    def add_upper_limits(self, points):
        xlow, xhigh = self.xlim
        for pt in points:
            if pt.ul and pt.xsec:
                is_low_dm = pt.ml + self._w_mass > pt.ms
                if not (xlow < pt.ms < xhigh) or is_low_dm:
                    continue
                xstr = '{:.0f}'.format(pt.ul * pt.xsec)
                self.ax.text(pt.ms, pt.ml, xstr, **self.ultxt)
        if not self._ax2:
            self._ax2 = self.ax.twinx()
            self._ax2.set_ylim(*self.ylim)
            self._ax2.tick_params(**self.major_pars)
            self._ax2.minorticks_on()
            self._ax2.tick_params(which='minor', **self.minor_pars)
            self._ax2.yaxis.set_ticklabels([])
            self._ax2.yaxis.set_label_position('right')
            self._ax2.set_ylabel(self.right_side_ul_info)

    def add_config(self, stop_lsp_cls, label=None, style=None, heatmap=False,
                   add_draw_opts=None):
        """
        Expects a list of (mass stop, mass lsp, upper limit) tuples.
        """
        point_lables = None
        stop_lsp_cls, _ = _remove_bads(stop_lsp_cls)
        if len(stop_lsp_cls[0]) > 3:
            byvar = list(zip(*stop_lsp_cls))
            stop_lsp_cls = zip(*byvar[:3])
            point_lables = byvar[3]
        slu = np.array(list(stop_lsp_cls))
        x, y, z = slu.T
        xmin, xmax = self.grid_xlim
        ymin, ymax = self.grid_ylim
        xpts = (xmax - xmin) // self.gev_per_bin + 1

        xp, yp, zp = self._interpolator(
            x, y, z, (xmin, xmax), (ymin, ymax), xpts)
        self._extrap_bottom(zp, y)
        self._fill_in_interp(zp,xp,yp,x,y)
        extent = [xmin, xmax, ymin, ymax]
        ct_color, ct_style = self._get_style(style)
        draw_opts = dict(colors=ct_color, linewidths=self.lw,
                         linestyles=ct_style)
        if add_draw_opts:
            draw_opts.update(add_draw_opts)
        ct = self.ax.contour(
            xp, yp, zp, [self._threshold], zorder=2, **draw_opts)
        if label in {OBSERVED, EXPECTED}:
            # self.ax.plot(*zip(*_get_contour_points(ct)))
            pass
        if heatmap:
            self.ax.imshow(
                zp, extent=extent, origin='lower', interpolation='nearest',
                aspect='auto', vmin=self._threshold*0.5,
                vmax=self._threshold*1.1)
        if label:
            singular_opts = {x.rstrip('s'): y for x,y in draw_opts.items()}
            proxline = Line2D((0,0),(0,1), zorder=1, **singular_opts)
            self._proxy_contour.append( (proxline, label))

        if point_lables:
            self._add_point_labels(x, y, point_lables)
            self._labeled_points |= set(xy for xy in zip(x,y))
        else:
            self._pts |= set( xy for xy in zip(x,y))

    def _fill_in_interp(self, zp, xp, yp, x, y):
        """misc hacks to fill in the interpolated space"""
        th_high = self._threshold * (1 + self.threshold_margin)
        th_low  = self._threshold * (1 - self.threshold_margin)
        znan = np.isnan(zp)
        min_dm = np.min(xp[~znan] - yp[~znan])

        # replace nan values with something above the threshold
        zp[znan] = th_high

        # fill in low values if we have stop points
        dm = (xp - yp)
        if self._fill_low_stop and min_dm < self._w_mass / 2:
            zp[(xp < min(x)) & (dm >= 0.0)] = th_low

            # # temp HACK to fill in area around missing 100-1 point
            # zp[(xp < 300) & (xp > 90) & (yp < 75)] = th_low

            # make sure forbidden values aren't included
            zp[dm < min_dm] = self.zval_forbidden

    def _add_point_labels(self, x, y, point_lables):
        xy = {l: [] for l in point_lables}
        for x, y, lab in zip(x, y, point_lables):
            xy[lab].append((x, y))
        for (lab, ptlist), color in zip(sorted(xy.items()), self.colors):
            x, y = np.array(ptlist).T
            inpts = (x > self.xlim[0]) & (y > self.ylim[0])
            pts, = self.ax.plot(
                x[inpts], y[inpts], '.', label=lab,
                color=color, markersize=20, zorder=1)
            self._proxy_contour.append((pts, lab))

    def add_band(self, stop_lsp_low_high, color=None, label=None):
        """
        Expects a list of (mass stop, mass lsp, upper limit) tuples.
        """
        stop_lsp_low_high, _ = _remove_bads(stop_lsp_low_high)
        x, y, low, high = np.array(stop_lsp_low_high).T
        xmin, xmax = self.grid_xlim
        ymin, ymax = self.grid_ylim
        xpts = (xmax - xmin) // self.gev_per_bin + 1

        xp, yp, lowp = self._interpolator(
            x, y, low, (xmin, xmax), (ymin, ymax), xpts)
        *nada, highp = self._interpolator(
            x, y, high, (xmin, xmax), (ymin, ymax), xpts)
        th = self._threshold
        zp = np.maximum( (lowp - th), -(highp - th))

        self._extrap_bottom(zp, y)

        extent = [xmin, xmax, ymin, ymax]
        draw_opts = dict(color=None, linewidth=self.lw,
                         linestyle='-')

        if not color:
            color = colorConverter.to_rgba('yellow')
        else:
            color = colorConverter.to_rgba(color, alpha=0.2)

        ct = self.ax.contourf(
            xp, yp, zp, [-1, 0],
            colors=[color], zorder=0)
        self._pts |= set( xy for xy in zip(x,y))
        if label:
            self._proxy_contour.append((Patch(color=color, zorder=0), label))

    def _extrap_bottom(self, zp, y):
        """
        fill in the bottom few points (since the exclusion goes off the
        page there)
        """
        bot_bins = min(y) // self.gev_per_bin + 2
        zp[0:bot_bins,:] = zp[bot_bins,:]

    def add_exclusion(self, xy, label, pushdown=False, properties=None):
        """
        Expects list of (x, y) points.
        With `pushdown`, makes sure the x values are below W mass.
        """
        pts = np.array(xy)
        if pushdown:
            upper = self._w_mass + self._b_mass
            pts[:,0] = np.minimum(pts[:,0], upper + pts[:,1])
            highpts = pts[(pts[:,0] > 100) & (pts[:,1] > 50)]
            min_dm = np.min(highpts[:,0] - highpts[:,1])
            pts = np.vstack(([[min_dm,0]], highpts, [[upper, 0]]))

        # default is monojet properties, but others are allowed
        poly_props = properties or dict(
            color='CornflowerBlue', alpha=self._mono_alpha, zorder=0)
        patch = Polygon(pts, **poly_props)
        self.ax.add_patch(patch)
        self._proxy_contour.append( (patch, label) )

    def add_atlas_label(self, atl_x, atl_y, stack=False):
        topopts = dict(size=self.big_size, transform=self.ax.transAxes,
                       ha='right', weight='bold', style='italic')
        botopts = dict(size=self.big_size, transform=self.ax.transAxes,
                       ha='left')
        if stack:
            topopts.update(dict(va='bottom', ha='right'))
            botopts.update(dict(va='top', ha='right'))
        self.ax.text(atl_x, atl_y, 'ATLAS', **topopts)
        if not self.approved:
            self.ax.text(atl_x, atl_y, ' ' + self.stage, **botopts)

    def add_labels(self, y=None, config=None):
        if y is None:
            y = 0.08 * len(set(x[1] for x in self._proxy_contour)) + 0.08

        # add lumi
        lumi_y = 1-y
        squeeze = y > 0.45
        lumisize = self.small_size if squeeze else self.med_size
        lumi_x = 0.03 if squeeze else 0.05
        self.ax.text(lumi_x, lumi_y,
                     r'$\sqrt{s}\ =$ 8 TeV, 20.3 fb$^{\sf -1}$',
                     transform=self.ax.transAxes, size=lumisize)

        # add atlas label
        sf = lumisize / self.med_size # squeeze factor
        atl_x = 0.275 if self.approved else 0.23
        atl_y = lumi_y - 0.08*sf
        self.add_atlas_label(atl_x, atl_y)

        title = self.title_tmp.format(
            c=self.scharm, cb=self.antischarm, l=self.lsp)
        if config:
            title += ', ' + _fancy_label(config)
        self.ax.set_title(
            title, loc='left', fontsize=self.big_size, va='bottom')
        self._labeled = True

    def _add_kinematic_bounds(self):
        """adds lines indicating where stop -> charm decays are allowed"""
        xl = self.xlim
        yl = self.ylim
        x_pts = np.array([0, max(xl[1], xl[1])])

        alpha = self._kinbound_alpha
        bound_style = dict(color='k', alpha=0.3)
        y_pts_f = x_pts
        y_pts_w = x_pts - (self._w_mass + self._b_mass)
        self.ax.plot(x_pts, y_pts_f, **bound_style)
        if self._kinbounds == BOTH or self._kinbounds == True:
            self.ax.plot(x_pts, y_pts_w, **bound_style)

    def _add_kinbound_text(self):
        """adds text on the kinematic bounds"""
        # calculate rotation (use point 1,1 to get angle
        p0 = self.ax.transData.transform_point((0,0))
        p1 = self.ax.transData.transform_point((1,1))
        pd = p1 - p0
        slope_deg = math.degrees(math.atan2(pd[1], pd[0]))

        xpos = 270
        px, py = xpos, xpos + 1.5*self.small_size
        text_style = dict(
            ha='left', va='bottom', rotation=slope_deg,
            color=(0,0,0,self._kinbound_alpha), fontsize=self.small_size)
        fmt_dict = dict(s=self.stop, l=self.lsp, c=self.scharm)
        upper_text = (
            r'$m_{{ {c} }} <\ m_{{ {l} }}$'
            r' (${c} \to c{l}$ forbidden)').format(**fmt_dict)
        self.ax.text(px, py, upper_text, **text_style)
        if self._kinbounds == BOTH or self._kinbounds == True:
            xoff = self._w_mass + self._b_mass
            lower_text = (
                r'$m_{{ {c} }} <\ m_{{ {l} }} +\ m_W +\ m_b$'
                r' (${s} \to c {l}$)').format(**fmt_dict)
            self.ax.text(px + xoff, py, lower_text , **text_style)

    def _add_signal_points(self):
        """Add unlabeled signal points"""
        # TODO: make drawing of labeled points happen here too
        unlab = self._pts - self._labeled_points
        if not unlab:
            return
        x, y = np.array(list(unlab)).T
        inpts = (x > self.xlim[0]) & (y > self.ylim[0])
        self._pts, = self.ax.plot(x[inpts],y[inpts],'.k', zorder=3)
        self._proxy_contour.insert(0,(self._pts, 'signal points'))

    def _finalize(self):
        if self._finalized:
            return
        if self._drawpoints:
            self._add_signal_points()
        if self._kinbounds:
            self._add_kinematic_bounds()
        self.ax.set_ylim(*self.ylim)
        self.ax.set_xlim(*self.xlim)
        self._add_kinbound_text()
        leg_labels = _get_legend_labels(self._proxy_contour)
        small_leg = len(leg_labels[0]) > 4 and self._labeled
        leg = self.ax.legend(
            *leg_labels,
             fontsize=(self.small_size if small_leg else self.med_size),
             **self.legend_properties)
        self._finalized = True

    def save(self, name):
        self._finalize()
        pl_dir = dirname(name)
        if pl_dir and not os.path.isdir(pl_dir):
            os.makedirs(pl_dir)
        self.canvas.print_figure(name, bbox_inches='tight')

# _________________________________________________________________________
# interpolation functions

def _get_meshgrid(xlims, ylims, xpts):
    xmin, xmax = xlims
    ymin, ymax = ylims
    xi = np.linspace(*xlims, num=xpts)
    ratio = (ymax - ymin) / (xmax - xmin)
    num_y = round(xpts * ratio)
    yi = np.linspace(*ylims, num=num_y)
    xp, yp = np.meshgrid(xi, yi)
    return xp, yp

def _interpolate_log(x, y, z, xlims, ylims, xpts):
    xp, yp = _get_meshgrid(xlims, ylims, xpts)
    pts = np.dstack((x,y)).squeeze()
    lin = LinearNDInterpolator(pts, np.log(z))
    interp_points = np.dstack((xp.flatten(), yp.flatten())).squeeze()
    interp_z = lin(interp_points).reshape(xp.shape)
    zp = np.exp(interp_z)
    return xp, yp, zp

def _interpolate_normal(x, y, z, xlims, ylims, xpts):
    xp, yp = _get_meshgrid(xlims, ylims, xpts)
    pts = np.dstack((x,y)).squeeze()
    lin = LinearNDInterpolator(pts, norm.ppf(z))
    interp_points = np.dstack((xp.flatten(), yp.flatten())).squeeze()
    interp_z = lin(interp_points).reshape(xp.shape)
    # avoid passing nan into the norm.cdf function
    nans = np.isnan(interp_z)
    zp = np.empty(interp_z.shape)
    zp[nans] = np.nan
    zp[~nans] = norm.cdf(interp_z[~nans])
    return xp, yp, zp

def _interpolate_linear(x, y, z, xlims, ylims, xpts):
    xp, yp = _get_meshgrid(xlims, ylims, xpts)
    pts = np.dstack((x,y)).squeeze()
    lin = LinearNDInterpolator(pts, z)
    interp_points = np.dstack((xp.flatten(), yp.flatten())).squeeze()
    zp = lin(interp_points).reshape(xp.shape)
    return xp, yp, zp

# interpolators for significance
interpolators = {
    'log': _interpolate_log,
    'gauss': _interpolate_normal,
    'lin': _interpolate_linear,
    }



# _________________________________________________________________________
# misc utilities

def _dump_contour(contour, label):
    """find points, dump them"""
    print(label, _get_contour_points(contour))

def _get_contour_points(contour):
    """return (x,y) points in an array"""
    paths = xy_pts = contour.collections[0].get_paths()[0]
    paths.simplify_threshold = 1 # should be in GeV?
    xy_pts = paths.cleaned(simplify=True).vertices
    return xy_pts

def _remove_bads(args, baddex=2):
    """returns two lists, of good and bad (test = -1.0) points"""
    goods = []
    bads = []
    for vals in args:
        cls = vals[baddex]
        applist = bads if cls == -1.0 else goods
        applist.append(vals)
    return goods, bads

def _get_legend_labels(legend_tuples):
    """
    Translates a list of (handle, name) tuples into the format needed
    by legend: ([list of handles], [list of labels]).
    """
    labels = []
    handles = {}
    for handle, label in legend_tuples:
        if label not in handles:
            labels.append(label)
            handles[label] = [handle]
        else:
            handles[label].append(handle)

    fancy_labels = [_fancy_label(x) for x in labels]
    handle_list = [tuple(handles[x]) for x in labels]
    return handle_list, fancy_labels

def _fancy_label(label):
    """make label pretty for regions"""
    return limitsty.config_names.get(label,label.replace('_',' '))
