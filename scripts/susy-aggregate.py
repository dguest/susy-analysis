#!/usr/bin/env python2.7
"""
uses run / mc meta data and histogram files to produce stack plots
"""

import glob, os, sys
from os.path import join, isdir, dirname, splitext, basename, isfile
from stop import meta 
import argparse
import ConfigParser
from stop.aggregator import SampleAggregator, PlotsDict
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
    parser.add_argument('-o', '--output-ext', nargs='?', 
                        default=None, const='.pdf', 
                        help='%(const)s if no value given')
    parser.add_argument('--dump-yaml', action='store_true')
    parser.add_argument('-f','--force-aggregation', action='store_true')
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
            ('aggregated_cache', 'aggregated.h5'), 
            ]
        for thing in paths: 
            config.set('paths',*thing)
        config.add_section('plot_opts')
        config.set('plot_opts','vars','\n'.join(plot_vars))
        config.add_section('meta_opts')
        config.set('meta_opts','lumi_fb',str(_lumi_fb))
        with open(args.config_file,'w') as cfg: 
            config.write(cfg)
        sys.exit('made config, quitting')
    
    config.read(args.config_file)
    args.meta_data = config.get('paths','meta')
    args.hists_dir = config.get('paths','hists')
    args.outplot_dir = config.get('paths','outplot_dir')
    args.agg_cache = config.get('paths','aggregated_cache')
    args.variables = config.get('plot_opts','vars').split()
    if len(args.variables) == 1 and args.variables[0].endswith('.yml'): 
        with open(args.variables) as yml_file: 
            args.variables = vars_from_yaml(yml_file)
    args.lumi_fb = config.getfloat('meta_opts','lumi_fb')
    args.files = glob.glob('{}/*.h5'.format(args.hists_dir))
    return args



def run(): 
    args = get_config()
    if isfile(args.agg_cache) and not args.force_aggregation: 
        plots_dict = PlotsDict(args.agg_cache)
    else: 
        aggregator = SampleAggregator(
            args.meta_data, 
            args.files, 
            args.variables)
        aggregator.lumi_fb = args.lumi_fb
        aggregator.aggregate()
        if isfile(args.agg_cache): 
            os.remove(args.agg_cache)
        aggregator.write(args.agg_cache)
        plots_dict = aggregator.plots_dict

    if args.output_ext: 
        from stop.stack import plot
        plot.make_plots(plots_dict, args)

    if args.dump_yaml: 
        sys.exit('dump_yaml not implemented')
        

if __name__ == '__main__': 
    run()
