#!/usr/bin/env python2.7

from stop.hists import Hist1d, HistNd
import matplotlib.pyplot as plt
import h5py, glob, os, sys
from os.path import join, isdir, dirname, splitext, basename
from stop import style, meta, hists

_files = glob.glob('hackhist2/*.h5')
_meta = 'filter-meta.yml'

def print_hist(h5hist, name): 
    hist = Hist1d(*HistNd(h5hist).project_1d('x'))

    fig = plt.figure(figsize=(8,6))
    ax = fig.add_subplot(1,1,1)
    ax.plot(*hist.get_xy_pts())
    fig.savefig(name, bbox_inches='tight')

class Plots(object): 
    def __init__(self, name, path): 
        self.name = name
        self.path = path
        self.plots = {}
        self.ext = '.pdf'
    def eat(self, hfile): 
        for pname, hplot in hfile[self.path].iteritems(): 
            if not pname in self.plots: 
                hist = Hist1d(*HistNd(hplot).project_1d('x'))
                self.plots[pname] = hist
            else: 
                self.plots[pname] += Hist1d(*HistNd(hplot).project_1d('x'))
    def rebin(self, n=4): 
        for name in self.plots: 
            self.plots[name].scale(n)
            self.plots[name].average_bins(n)
            self.rebin_factor = n
    def save_overlay(self, base_dir): 
        fig = plt.figure(figsize=(8,6))
        ax = fig.add_subplot(1,1,1) 
        for pname, hist in self.plots.iteritems(): 
            leg_name = '{}: {}'.format(pname, float(hist) / self.rebin_factor)
            ax.plot(*hist.get_xy_pts(), label=leg_name)
        ax.legend()
        if not isdir(base_dir): 
            os.makedirs(base_dir)
        out_name = join(base_dir, '{}{}'.format(self.name, self.ext))
        fig.savefig(out_name, bbox_inches='tight')

_plot_vars = [ 
    'jet1/pt', 
    'jet2/pt', 
    'jet3/pt', 
    'met'
    ]
_lumi_fb = 15.0

def run(): 
    filter_meta = meta.DatasetCache(_meta)
    physics_types = {ds.physics_type for ds in filter_meta.values()}
    for pt in physics_types:
        if pt not in style.type_dict:
            raise ValueError("what the fuck is {}?".format(pt))

    plots_dict = {}
    all_cuts_used = set()
    for f in _files: 
        meta_name = basename(splitext(f)[0])
        if meta_name not in filter_meta: 
            continue

        file_meta = filter_meta[meta_name]
        if file_meta.bugs: 
            print "uh oh, bugs: {} in {}".format(file_meta.bugs, meta_name)
            continue

        physics_type = file_meta.physics_type
        if physics_type == 'data': 
            lumi_scale = 1.0
        else: 
            lumi_scale = _lumi_fb / file_meta.effective_luminosity_fb * 1000
        with h5py.File(f) as hfile: 
            for variable in _plot_vars: 
                for cut_name, h5hist in hfile[variable].iteritems(): 
                    hist = Hist1d(*HistNd(h5hist).project_1d('x'))
                    hist.scale(lumi_scale)
                    hist.color = style.type_dict[physics_type].color
                    idx_tuple = (physics_type, variable, cut_name)
                    if not idx_tuple in plots_dict: 
                        plots_dict[idx_tuple] = hist
                    else: 
                        plots_dict[idx_tuple] += hist
                    all_cuts_used.add(cut_name)

    stack_mc_lists = {(v,c):[] for v in _plot_vars for c in all_cuts_used}
    stack_data = {}
            
    for (physics_type, variable, cut), hist in plots_dict.iteritems(): 
        tup = (variable, cut)
        if physics_type == 'data': 
            if tup in stack_data: 
                raise ValueError('doubling the data')
            stack_data[(variable, cut)] = hist
        else: 
            stack_mc_lists[(variable, cut)].append(hist)

    plot_dir = 'plots'
    if not isdir(plot_dir): 
        os.mkdir(plot_dir)
    for id_tup in stack_mc_lists.keys(): 
        variable, cut = id_tup
        stack_name = '_'.join([variable.replace('/','-'), cut])
        stack = hists.Stack(stack_name)
        stack.y_min = 0.1
        stack.add_backgrounds(stack_mc_lists[id_tup])
        stack.add_data(stack_data[id_tup])
        stack.ax.set_yscale('log')
        stack.add_legend()
        save_name = join(plot_dir, stack_name) + '.pdf'
        stack.save(save_name)
        stack.close()
        

if __name__ == '__main__': 
    run()
