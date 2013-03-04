#!/usr/bin/env python2.7
"""
uses run / mc meta data and histogram files to produce stack plots
"""

from stop.hists import Hist1d
import matplotlib.pyplot as plt
import glob, os, sys
from os.path import join, isdir, dirname, splitext, basename, isfile
from stop import style, meta, hists
import argparse
import ConfigParser
from stop.aggregator import SampleAggregator, get_all_objects
import yaml

_plot_vars = [ 
    'jet1/pt', 
    'jet2/pt', 
    'jet3/pt', 
    'met'
    ]
_lumi_fb = 20.661

def _listify(prefix ,node):
    """
    magic function to get the bottom-level directories
    """
    outlist = []
    for name, subnode in node.items(): 
        full_prefix = '/'.join([prefix, name])
        try: 
            newlist = _listify(full_prefix, subnode)
            outlist += newlist
        except AttributeError: 
            return [prefix]
    return [v.lstrip('/') for v in outlist]

def vars_from_yaml(yaml_text): 
    top_node = yaml.load(yaml_text)
    return _listify('',top_node)

def get_config_file(): 
    config_files = glob.glob('*.cfg')
    if len(config_files) > 1: 
        raise IOError('multiple config files found, not sure what to use')
    elif len(config_files) < 1: 
        raise IOError('name a config file')
    return config_files[0]

def get_config(): 
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('config_file', nargs='?', default=None, 
                        help='defaults to *.cfg, can also give an .h5 basis')
    parser.add_argument('-o', '--output-ext', default='.pdf', 
                        help='(default: %(default)s)')
    parser.add_argument('--dump-yaml', action='store_true')
    args = parser.parse_args(sys.argv[1:])
    if not args.config_file: 
        args.config_file = get_config_file()
    config = ConfigParser.SafeConfigParser()
    if isfile(args.config_file) and args.config_file.endswith('.h5'): 
        plot_vars = _listify('',get_all_objects(args.config_file))
        args.config_file = 'plot.cfg'
        if isfile(args.config_file): 
            os.remove(args.config_file)
    else: 
        plot_vars = _plot_vars
    if not isfile(args.config_file):
        print 'Creating config file {}'.format(
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
        config.set('plot_opts','vars','\n'.join(plot_vars))
        config.set('plot_opts','lumi_fb',str(_lumi_fb))
        with open(args.config_file,'w') as cfg: 
            config.write(cfg)
        sys.exit('made config, quitting')
    
    config.read(args.config_file)
    args.meta_data = config.get('paths','meta')
    args.hists_dir = config.get('paths','hists')
    args.outplot_dir = config.get('paths','outplot_dir')
    args.variables = config.get('plot_opts','vars').split()
    if len(args.variables) == 1 and args.variables[0].endswith('.yml'): 
        with open(args.variables) as yml_file: 
            args.variables = vars_from_yaml(yml_file)
    args.lumi_fb = config.getfloat('plot_opts','lumi_fb')
    args.files = glob.glob('{}/*.h5'.format(args.hists_dir))
    return args



def run(): 
    args = get_config()
    if args.dump_yaml: 
        yaml_text = yaml.dump(get_all_objects(args.files[0]))
        print yaml_text
        sys.exit('dumped yaml, quitting...')
    aggregator = SampleAggregator(args.meta_data, args.files, args.variables)
    aggregator.lumi_fb = args.lumi_fb
    aggregator.aggregate()

    hist1_dict = {}
    for tup, hist in aggregator.plots_dict.iteritems():
        hist1_dict[tup] = hist1_from_histn(*tup, histn=hist, 
                                            lumi_fb=args.lumi_fb)

    print_plots(args, *sort_data_mc(hist1_dict))

def hist1_from_histn(physics, variable, cut, histn, lumi_fb): 
    if physics not in style.type_dict:
        raise ValueError("what the fuck is {}?".format(pt))

    assert len(histn.axes) == 1
    y_vals, extent = histn.project_1d('x')
    base_var = variable.split('/')[-1]
    units = histn.axes[0].units
    if units == 'MeV': 
        extent = [e / 1000.0 for e in extent]
        units = 'GeV'
    if base_var in style.ax_labels: 
        var_sty = style.ax_labels[base_var]
        var_sty.units = units
    else: 
        var_sty = sytle.VariableStyle(base_var, units)
    x_ax_lab = var_sty.axis_label
        
    x_ax_full_label = r' '.join(variable.split('/')[:-1] + [x_ax_lab])

    hist = Hist1d(y_vals, extent)
    n_center_bins = len(y_vals) - 2 
    if n_center_bins > 50 and n_center_bins % 4 == 0: 
        hist.rebin(4)
    hist.x_label = x_ax_full_label
    hist.y_label = style.event_label(lumi_fb)
    hist.color = style.type_dict[physics].color
    hist.title = style.type_dict[physics].tex
    hist.title += r': {:.1f} Evt'.format(float(hist))
    return hist

def sort_data_mc(hist1_dict): 
    stack_data = {}
    all_cuts = set(c for p,v,c in hist1_dict.keys())
    variables = set(v for p,v,c in hist1_dict.keys())
    lists = {(v,c):[] for v in variables for c in all_cuts}
    for threetup, hist in hist1_dict.iteritems(): 
        physics_type, variable, cut = threetup
        tup = (variable, cut)
        if physics_type == 'data': 
            if tup in stack_data: 
                raise ValueError('doubling the data')
            stack_data[(variable, cut)] = hist
        else: 
            lists[(variable, cut)].append(hist)

    for tup in lists: 
        lists[tup].sort()

    return stack_data, lists


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
        save_base = join(plot_dir, stack_name)
        if save_base.endswith('_cr'): 
            stack.add_data(stack_data[id_tup])
        else: 
            print '{} is blinded, not showing data'.format(save_base)
        stack.add_legend()
        save_name = save_base + args.output_ext
        stack.save(save_name)
        stack.close()
        

if __name__ == '__main__': 
    run()
