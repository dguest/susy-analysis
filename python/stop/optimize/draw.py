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
            signal_name='signal', signal_hist=None, background_hist=None): 
    for axis in sig_hist.axes: 
        array, ext = sig_hist.project_1d(axis.name)
        x_name, ext, units = new_name_ext(axis.name, ext)
        signal_histo = hists.Hist1d(array, ext)

        shape = (axis.bins + 2, -1)
        reshaped = sig_hist.array.swapaxes(axis.number,0).reshape(shape)
        max_args = np.argmax(reshaped,1)
        # maxes = reshaped[np.arange(shape[0]),max_args]
        global_max = np.unravel_index(reshaped.argmax(),reshaped.shape)
        fixed_maxes = reshaped[np.arange(shape[0]),global_max[1]]
        fixed_hist = hists.Hist1d(fixed_maxes,ext)
        
        fig = plt.figure(figsize=(6,4.5))
        ax = fig.add_subplot(1,1,1)
        x_pts, y_pts = signal_histo.get_xy_pts()
        handle, = ax.plot(x_pts, y_pts, label=r'$\sigma$ optimal', 
                          color='blue')
        
        fixed, = ax.plot(*fixed_hist.get_xy_pts(), linestyle='dashed', 
                          color='blue', 
                          label=r'$\sigma$ fixed')
        if baseline: 
            nom_handel = ax.plot(x_pts, [baseline]*len(x_pts), 
                                 color='green', 
                                 label=r'$\sigma$ nominal')
        ax.set_ylabel(fom_label, y=0.98, va='top')
        x_lab = '{} [{}]'.format(x_name, units) if units else x_name
        ax.set_xlabel(x_lab, x=0.98, ha='right')
        ax.grid(True)
        lims = ax.get_ylim()
        ax.set_ylim(lims[0], (lims[1] - lims[0])*1.3 + lims[0])
        ax.legend(loc='upper left').get_frame().set_visible(False)
        if signal_hist is not None: 
            count_ax = ax.twinx()
            count_ax.set_yscale('log')

            r_sg = signal_hist.array.swapaxes(axis.number,0).reshape(shape)
            signal_maxes = r_sg[np.arange(shape[0]),max_args]
            r_bg = background_hist.array.swapaxes(axis.number,0).reshape(shape)
            background_maxes = r_bg[np.arange(shape[0]),max_args]

            signal_hist_1d = hists.Hist1d(signal_maxes, ext)
            ct_x, ct_y = signal_hist_1d.get_xy_pts()
            count_ax.plot(ct_x, ct_y,'r', label='signal')
            count_ax.set_ylabel('$n$ events', y=0.98, va='top')

            bg_hist_1d = hists.Hist1d(background_maxes, ext)
            bg_x, bg_y = bg_hist_1d.get_xy_pts()
            count_ax.plot(bg_x, bg_y,'orange', label='background')
            count_ax.legend(loc='upper right').get_frame().set_visible(False)

            lims = count_ax.get_ylim()
            count_ax.set_ylim(lims[0], lims[1]*20)

            
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
        self._baseline = None
        self.max_bins = {}
        self.ymin = None
        self._do_log = False

    @property
    def baseline(self): 
        return self._baseline
    @baseline.setter
    def baseline(self, value): 
        self._baseline = value
        if value: 
            self.ymin = min(value, self.ymin)

        
    def compute(self,sys_factor=0.3): 
        used = []
        mins = []
        for new in self.cuts: 
            used.append(new)
            sig = opttools.compute_significance(
                self._signal, self._background, kept_axes=used, 
                sys_factor=sys_factor)
            array, extent = sig.project_1d(new)
            mins.append(np.min(array[array != 0.0]))
            self._cut_tuples.append((array, extent))
            self.fom_label = self.fom_template.format(sys_factor)

        self.max_bins = sig.max_bin_dict()
        self.ymin = min(mins)
        
    def get_efficiencies(self): 
        if not self.max_bins: 
            self.compute()
        sig_ratios = self._array_ratio(self._signal)
        bkg_ratios = self._array_ratio(self._background)
        assert self._signal.axes == self._background.axes

        names = [a.name for a in self._signal.axes]
        return {n: (s,b) for n,s,b in zip(names, sig_ratios, bkg_ratios)}

    def _array_ratio(self, histogram):
        out_vals = []
        for ax in histogram.axes: 
            max_bin = self.max_bins[ax.name]
            ar = histogram.array
            if ax.type == 'bare': 
                precut_count = ar.sum()
                postcut_count = ar.swapaxes(0,ax.number)[max_bin:,...].sum()
            elif ax.type == 'integral': 
                precut_count = ar.max()
                postcut_count = ar.swapaxes(0,ax.number)[max_bin:,...].max()
            else: 
                raise ValueError(
                    'axis has type {}, WTF?'.format(ax.type))
            out_vals.append(postcut_count / precut_count)

        return out_vals

    def set_log(self, min_val=None): 
        if min_val is not None: 
            self.ymin = min_val
        self._do_log = True

    def draw_normalized(self, output_file_name): 
        color_itr = iter(self.colors)
        fig = plt.figure(figsize=(6,4.5))
        ax = fig.add_subplot(1,1,1)
        for (array, extent), cut in zip(self._cut_tuples, self.cuts): 
            if self._do_log: 
                array = np.maximum(self.ymin, array)
            hist = hists.Hist1d(array, (0,1))
            new_name, extent, units = new_name_ext(cut, extent)


            color = next(color_itr)
            x_pts, y_pts = hist.get_xy_pts()
            plot_label = '{cut} {} - {} {units}'.format(
                cut=cut, *extent, units=units)
            handle, = ax.plot(x_pts, y_pts, 
                              label=plot_label,
                              color=color)

            x_format = '{: .3g} {}'

            maxbin = self.max_bins[cut] - 1 # offset for underflow
            nbins = array.size - 2
            maxbiny = array[maxbin + 1] # undo underflow offset
            if nbins > maxbin >= 0: 
                maxbinx = np.linspace(
                    *extent, num=nbins, endpoint=False)[maxbin]
                maxbinx_norm = np.linspace(
                    0,1, nbins, endpoint=False)[maxbin]

                txt_max = ax.text(maxbinx_norm, maxbiny, 
                                  x_format.format(maxbinx, units), 
                                  size=9, 
                                  color=color, va='bottom', ha='left')
            elif maxbin == nbins: 
                ax.text(1, maxbiny, 'overflow', size=9, color=color, 
                        va='bottom', ha='right')
            elif maxbin == -1: 
                ax.test(0, maxbiny, 'underflow', size=9, color=color, 
                        va='bottom', ha='left')
            else: 
                raise RuntimeError("something wront")
                
            
        if self._baseline is not None: 
            handle, = ax.plot((0,1), [self._baseline]*2, 'k--', 
                              label='nominal')
            
        ax.set_ylabel(self.fom_label, y=0.98, va='top')
        ax.set_xlabel('cut value', x=0.98, ha='right')
        ax.grid(True)
        ax.get_xaxis().set_ticklabels([])
        ax.get_xaxis().set_ticks([])

        if self._do_log: 
            ax.set_yscale('log')

        ymin, ymax = ax.get_ylim()
        top_edge = ymax * 2.5 if self._do_log else ymax + (ymax - ymin)*0.2
        ax.set_ylim(ymin, top_edge)
        leg = ax.legend(ncol=2)
        leg.get_frame().set_visible(False)
        plt.setp(leg.get_texts(), fontsize=9) 
        fig.savefig(output_file_name, bbox_inches='tight')
        plt.close(fig)
        
