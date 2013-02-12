#!/usr/bin/env python2.7
"""
uses run / mc meta data and histogram files to produce stack plots
"""

from stop.hists import Hist1d, HistNd
import matplotlib.pyplot as plt
import h5py, glob, os, sys
from os.path import join, isdir, dirname, splitext, basename, isfile
from stop import style, meta, hists
import argparse
import ConfigParser

_files = glob.glob('hackhist2/*.h5')
_meta = 'filter-meta.yml'

_plot_vars = [ 
    'jet1/pt', 
    'jet2/pt', 
    'jet3/pt', 
    'met'
    ]
_lumi_fb = 15.0

def get_config(): 
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('config_file', nargs='?', default=None, 
                        help='defaults to *.cfg')
    parser.add_argument('-o', '--output-ext', default='.pdf', 
                        help='(default: %(default)s)')
    args = parser.parse_args(sys.argv[1:])
    if not args.config_file: 
        config_files = glob.glob('*.cfg')
        if len(config_files) > 1: 
            raise IOError('multiple config files found, not sure what to use')
        elif len(config_files) < 1: 
            raise IOError('name a config file')
        args.config_file = config_files[0]
    
    config = ConfigParser.SafeConfigParser()
    if not isfile(args.config_file): 
        print 'no config file, {} found. Creating with defaults'.format(
            args.config_file)
        config.add_section('paths')
        paths = [ 
            ('meta','meta.yml'), 
            ('hists','hists'), 
            ('outplot_dir', 'plots'),
            ]
        for thing in paths: 
            config.set('paths',*thing)
        config.add_section('plot_opts')
        config.set('plot_opts','vars','\n'.join(_plot_vars))
        config.set('plot_opts','lumi_fb',str(_lumi_fb))
        with open(args.config_file,'w') as cfg: 
            config.write(cfg)
        sys.exit('made config, quitting')
    
    config.read(args.config_file)
    args.meta_data = config.get('paths','meta')
    args.hists_dir = config.get('paths','hists')
    args.outplot_dir = config.get('paths','outplot_dir')
    args.variables = config.get('plot_opts','vars').split()
    args.lumi_fb = config.getfloat('plot_opts','lumi_fb')
    args.files = glob.glob('{}/*.h5'.format(args.hists_dir))
    return args



def run(): 
    args = get_config()
    aggregator = SampleAggregator(args.meta_data, args.files, args.variables)
    aggregator.lumi_fb = args.lumi_fb
    aggregator.aggregate()

    all_cuts_used = aggregator.all_cuts_used
    plots_dict = aggregator.plots_dict

    # todo: we should do the conversion before sorting into lists
    hist_data = aggregator.get_data()
    stack_data = {}
    for tup, hist in hist_data.iteritems(): 
        stack_data[tup] = hist1_from_histn(hist.physics, *tup, histn=hist, 
                                           lumi_fb=args.lumi_fb)

    stack_mc_lists = {}
    hist_mc_lists = aggregator.get_mc_lists()
    for tup, histlist in hist_mc_lists.iteritems(): 
        stack_mc_lists[tup] = []
        for hist in histlist: 
            stack_mc_lists[tup].append(
                hist1_from_histn(hist.physics, *tup, histn=hist, 
                                 lumi_fb=args.lumi_fb))

    print_plots(args, stack_data, stack_mc_lists)

def hist1_from_histn(physics, variable, cut, histn, lumi_fb): 
    y_vals, extent = histn.project_1d('x')
    base_var = variable.split('/')[-1]
    if base_var in style.ax_labels: 
        var_sty = style.ax_labels[base_var]
        extent = [var_sty.rescale(x) for x in extent]
        x_ax_lab = var_sty.axis_label
    else: 
        x_ax_lab = base_var

    hist = Hist1d(y_vals, extent)
    hist.rebin(4)
    hist.x_label = x_ax_lab
    hist.y_label = style.event_label(lumi_fb)
    hist.color = style.type_dict[physics].color
    hist.title = style.type_dict[physics].tex
    return hist


def print_plots(args, stack_data, stack_mc_lists): 

    plot_dir = args.outplot_dir
    if not isdir(plot_dir): 
        os.mkdir(plot_dir)
    for id_tup in stack_mc_lists.keys(): 
        variable, cut = id_tup
        stack_name = '_'.join([variable.replace('/','-'), cut])
        stack = hists.Stack(stack_name)
        stack.y_min = 0.1
        stack.ax.set_yscale('log')
        stack.add_backgrounds(stack_mc_lists[id_tup])
        stack.add_data(stack_data[id_tup])
        stack.add_legend()
        save_name = join(plot_dir, stack_name) + args.output_ext
        stack.save(save_name)
        stack.close()
        
class SampleAggregator(object): 
    """
    Takes some meta data and whiskey as an input, produces aggrigated 
    histograms. 
    """
    def __init__(self, meta_path, whiskey, variables): 
        self.whiskey = whiskey
        self.variables = variables
        self.filter_meta = meta.DatasetCache(meta_path)
        physics_types = {ds.physics_type for ds in self.filter_meta.values()}
        for pt in physics_types:
            if pt not in style.type_dict:
                raise ValueError("what the fuck is {}?".format(pt))
        self.lumi_fb = 15.0

    def aggregate(self): 
        plots_dict = {}
        all_cuts_used = set()
        for f in self.whiskey: 
            meta_name = basename(splitext(f)[0])
            if meta_name not in self.filter_meta: 
                continue
    
            file_meta = self.filter_meta[meta_name]
            if file_meta.bugs: 
                print "uh oh, bugs: {} in {}".format(file_meta.bugs, meta_name)
                continue
    
            physics_type = file_meta.physics_type
            if physics_type == 'data': 
                lumi_scale = 1.0
            else: 
                lumi_scale = self.lumi_fb / file_meta.effective_luminosity_fb 
    
            with h5py.File(f) as hfile: 
                for variable in self.variables: 
                    for cut_name, h5hist in hfile[variable].iteritems(): 
                        hist = HistNd(h5hist)
                        hist *= lumi_scale
                    
                        idx_tuple = (physics_type, variable, cut_name)
                        if not idx_tuple in plots_dict: 
                            plots_dict[idx_tuple] = hist
                        else: 
                            plots_dict[idx_tuple] += hist
                        all_cuts_used.add(cut_name)

        self.all_cuts_used = all_cuts_used
        self.plots_dict = plots_dict

    def get_mc_lists(self): 
        """
        ugly, remove
        """
        lists = {(v,c):[] for v in self.variables for c in self.all_cuts_used}
        for threetup, hist in self.plots_dict.iteritems(): 
            physics_type, variable, cut = threetup
            tup = (variable, cut)
            if physics_type != 'data': 
                hist.physics = physics_type
                lists[(variable, cut)].append(hist)

        for tup in lists: 
            lists[tup].sort()

        return lists
    
    def get_data(self): 
        """
        ugly, remove
        """
        stack_data = {}
        for threetup, hist in self.plots_dict.iteritems(): 
            physics_type, variable, cut = threetup
            tup = (variable, cut)
            if physics_type == 'data': 
                hist.physics = physics_type
                if tup in stack_data: 
                    raise ValueError('doubling the data')
                stack_data[(variable, cut)] = hist
        return stack_data

if __name__ == '__main__': 
    run()