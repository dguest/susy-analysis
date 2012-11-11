import matplotlib.pyplot as plt
import matplotlib as mp
import numpy as np
from mpl_toolkits.axes_grid1 import make_axes_locatable

from os.path import join, basename, splitext, isdir, isfile
import os

from stop import hists
from stop.optimize import opttools

def new_name_ext(name, ext): 
    if name in ['met','leadingPt']: 
        newe = [x/1e3 for x in ext]
        return name, newe, 'GeV'
    else: 
        return name, ext, ''


def plot_2d(sig_hist, fom_label, put_where='plots'):     
    """
    sig_hist must be a hyperhist
    """

    for x_ax in [x.name for x in sig_hist.axes]: 
        for y_ax in [y.name for y in sig_hist.axes]: 
            if x_ax <= y_ax: continue
            print '{} vs {}'.format(y_ax, x_ax)
            fig = plt.figure(figsize=(6,4.5))
            ax = fig.add_subplot(1,1,1)
            im_dict = sig_hist.project_imshow(x_ax, y_ax)
            x_name, im_dict['extent'][0:2], x_unit = new_name_ext( 
                x_ax, im_dict['extent'][0:2])
            y_name, im_dict['extent'][2:4], y_unit = new_name_ext(
                y_ax, im_dict['extent'][2:4])
            im = ax.imshow(**im_dict)

            divider = make_axes_locatable(ax)
            cax = divider.append_axes("right", "5%", pad="1.5%")
            cb = plt.colorbar(im, cax=cax)
            plt.tight_layout()

            # cb.add_lines(ct)
            cb.set_label(fom_label)
            x_lab = '{} [{}]'.format(x_name, x_unit) if x_unit else x_name
            ax.set_xlabel(x_lab, x=0.98, ha='right')
            y_lab = '{} [{}]'.format(y_name, y_unit) if y_unit else y_name
            ax.set_ylabel(y_lab, y=0.98, va='top')
            fig.savefig(join(put_where,'{}_vs_{}.pdf'.format(y_ax, x_ax)), 
                        bbox_inches='tight')
            plt.close(fig)

def plot_1d(sig_hist, fom_label, put_where='plots', baseline=None, 
            signal_name='signal'): 
    for axis in sig_hist.axes: 
        array, ext = sig_hist.project_1d(axis.name)
        x_name, ext, units = new_name_ext(axis.name, ext)
        signal_histo = hists.Hist1d(array, ext)
        
        fig = plt.figure(figsize=(6,4.5))
        ax = fig.add_subplot(1,1,1)
        x_pts, y_pts = signal_histo.get_xy_pts()
        handle, = ax.plot(x_pts, y_pts, label=signal_name)
        if baseline: 
            nom_handel = ax.plot(x_pts, [baseline]*len(x_pts), 
                                 label='nominal')
        ax.set_ylabel(fom_label, y=0.98, va='top')
        x_lab = '{} [{}]'.format(x_name, units) if units else x_name
        ax.set_xlabel(x_lab, x=0.98, ha='right')
        ax.grid(True)
        ax.legend().get_frame().set_visible(False)
        for ext in ['.pdf','.png']: 
            fig.savefig(join(put_where,'{}{}'.format(axis.name, ext)),
                        bbox_inches='tight')
        plt.close(fig)

class CutflowOptimum(object): 
    def __init__(self, signal, background, cuts): 
        self._signal = signal
        self._background = background
        self.fom_template = (
            r'$\frac{{s}}{{\sqrt{{s + b + ({:.2f} \times b)^2}} }}$')
        self.fom_label = ''
        self.cuts = cuts
        self._cut_tuples = []
        self.colors = [
            'red', 
            'green',
            'blue',
            'orange',
            'pink',
            'black', 
            'cyan', 
            'magenta', 
            '0.5'
            ]
        self.baseline = None
        self.max_bins = {}
        
    def compute(self,sys_factor=0.3): 
        used = []
        for new in self.cuts: 
            used.append(new)
            sig = opttools.compute_significance(
                self._signal, self._background, kept_axes=used, 
                sys_factor=sys_factor)
            self._cut_tuples.append(sig.project_1d(new))
            self.fom_label = self.fom_template.format(sys_factor)

            self.max_bins = sig.max_bin_dict()


    def draw_normalized(self, output_file_name): 
        color_itr = iter(self.colors)
        fig = plt.figure(figsize=(6,4.5))
        ax = fig.add_subplot(1,1,1)
        for (array, extent), cut in zip(self._cut_tuples, self.cuts): 
            hist = hists.Hist1d(array, (0,1))
            new_name, extent, units = new_name_ext(cut, extent)

            maxbin = self.max_bins[cut] - 1 # offset for underflow
            nbins = array.size - 2
            maxbinx = np.linspace(*extent, num=nbins, endpoint=False)[maxbin]
            maxbiny = array[maxbin + 1] # undo underflow offset
            maxbinx_norm = np.linspace(0,1, nbins, endpoint=False)[maxbin]

            color = next(color_itr)
            x_pts, y_pts = hist.get_xy_pts()
            handle, = ax.plot(x_pts, y_pts, 
                              label=cut,
                              color=color)

            x_format = '{: .3g} {}'

            bbox = dict(ec='none', pad=3, color='none')
            txt_left = ax.text(0,array[1],
                               x_format.format(extent[0], units), 
                                size=9, 
                                va='bottom', ha='left',
                                color=color, 
                                bbox=bbox)
            txt_right = ax.text(1,array[-2],
                                x_format.format(extent[1], units), 
                                size=9, 
                                va='bottom', ha='right',
                                color=color, 
                                bbox=bbox)
            txt_max = ax.text(maxbinx_norm, maxbiny, 
                              x_format.format(maxbinx, units), 
                              size=9, 
                              color=color, va='bottom', ha='left')
            
        if self.baseline is not None: 
            handle, = ax.plot((0,1), [self.baseline]*2, 'k--', 
                              label='nominal')
            
        ax.set_ylabel(self.fom_label, y=0.98, va='top')
        ax.set_xlabel('arb', x=0.98, ha='right')
        ax.grid(True)
        ax.get_xaxis().set_ticklabels([])
        ax.get_xaxis().set_ticks([])

        ymin, ymax = ax.get_ylim()
        ax.set_ylim(ymin, ymax + (ymax - ymin)*0.2)
        ax.legend(ncol=3).get_frame().set_visible(False)

        fig.savefig(output_file_name, bbox_inches='tight')
        plt.close(fig)
        
