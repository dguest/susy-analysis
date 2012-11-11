import matplotlib.pyplot as plt
import matplotlib as mp
from mpl_toolkits.axes_grid1 import make_axes_locatable

from os.path import join, basename, splitext, isdir, isfile
import os

from stop import hists

def new_name_ext(name, ext): 
    if name in ['met','leadingPt']: 
        newe = [x/1e3 for x in ext]
        return '{} [GeV]'.format(name), newe
    else: 
        return name, ext


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
            x_name, im_dict['extent'][0:2] = new_name_ext( 
                x_ax, im_dict['extent'][0:2])
            y_name, im_dict['extent'][2:4] = new_name_ext(
                y_ax, im_dict['extent'][2:4])
            im = ax.imshow(**im_dict)

            divider = make_axes_locatable(ax)
            cax = divider.append_axes("right", "5%", pad="1.5%")
            cb = plt.colorbar(im, cax=cax)
            plt.tight_layout()

            # cb.add_lines(ct)
            cb.set_label(fom_label)
            ax.set_xlabel(x_name, x=0.98, ha='right')
            ax.set_ylabel(y_name, y=0.98, va='top')
            fig.savefig(join(put_where,'{}_vs_{}.pdf'.format(y_ax, x_ax)), 
                        bbox_inches='tight')
            plt.close(fig)

def plot_1d(sig_hist, fom_label, put_where='plots', baseline=None, 
            signal_name='signal'): 
    for axis in sig_hist.axes: 
        array, ext = sig_hist.project_1d(axis.name)
        x_name, ext = new_name_ext(axis.name, ext)
        signal_histo = hists.Hist1d(array, ext)
        
        fig = plt.figure(figsize=(6,4.5))
        ax = fig.add_subplot(1,1,1)
        x_pts, y_pts = signal_histo.get_xy_pts()
        handle, = ax.plot(x_pts, y_pts, label=signal_name)
        if baseline: 
            nom_handel = ax.plot(x_pts, [baseline]*len(x_pts), 
                                 label='nominal')
        ax.set_ylabel(fom_label, y=0.98, va='top')
        ax.set_xlabel(x_name, x=0.98, ha='right')
        ax.grid(True)
        ax.legend().get_frame().set_visible(False)
        for ext in ['.pdf','.png']: 
            fig.savefig(join(put_where,'{}{}'.format(axis.name, ext)),
                        bbox_inches='tight')
        plt.close(fig)
