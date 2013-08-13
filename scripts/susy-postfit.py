#!/usr/bin/env python2.7

"""
Top level for post-fit routines. At the moment not sure if this includes 
aggregation. 
"""

import argparse
import sys, os
import yaml
import itertools
from stop.postfit import split_to_planes, numpy_plane_from_dict
import h5py

from matplotlib.backends.backend_agg import FigureCanvasAgg as FigureCanvas
from matplotlib.figure import Figure
import matplotlib.pyplot as plt
from mpl_toolkits.axes_grid1 import make_axes_locatable
import matplotlib as mp
from stop.style import ax_labels, vdict, hdict

def run(): 
    parser = argparse.ArgumentParser()
    subs = parser.add_subparsers(dest='which')
    yaml_reader = subs.add_parser('yml')
    yaml_reader.add_argument('yaml_file')
    yaml_reader.add_argument('-o', '--out-file')
    args = parser.parse_args(sys.argv[1:])
    subroutines = {'yml':_yml_parser}
    subroutines[args.which](args)

def _yml_parser(args): 
    print 'loading yaml file (slow)'
    with open(args.yaml_file) as yml: 
        point_list = yaml.load(yml)
    if not args.out_file: 
        args.out_file = os.path.splitext(args.yaml_file)[0] + '.h5'
    save_h5(point_list, args.out_file)

def save_h5(point_list, out_file_name): 
    configs = set()
    signal_points = set()
    planes = split_to_planes(point_list)

    for config, sp in planes: 
        configs.add(config)
        signal_points.add(sp)

    with h5py.File(out_file_name) as h5_file: 
        for config in configs: 
            sp_group = h5_file.create_group(config)
            for sp in signal_points: 
                plane_list = planes[config, sp]
                array, extents, ax_names = numpy_plane_from_dict(plane_list)
                ds = sp_group.create_dataset(
                    sp, data=array, compression='gzip')
                ds.attrs['extent'] = extents
                ds.attrs['ax_names'] = ax_names

def plot(point_list): 
    print 'splitting'
    planes = split_to_planes(point_list)
    for plane_key, plane in planes.iteritems():
        print plane_key
        array, extents, ax_names = numpy_plane_from_dict(plane)
        _draw_plot(array, extents, plane_key, ax_names=ax_names)
        
def _get_label(label): 
    for lab in ax_labels: 
        if lab in label: 
            return ax_labels[lab].axis_label

def _draw_plot(array, extents, key, ax_names): 
    figure = Figure(figsize=(9,8))
    ax = figure.add_subplot(1,1,1)
    divider = make_axes_locatable(ax)
    cax = divider.append_axes("right", "5%", pad="1.5%")
    im = ax.imshow(array.T, extent=extents, aspect='auto', 
                   interpolation='nearest', origin='lower')
    ax.set_xlabel(_get_label(ax_names[0]), **hdict)
    ax.set_ylabel(_get_label(ax_names[1]), **vdict)
    cb = plt.colorbar(im,cax=cax)
    label_str = 'expected ul, {}'.format(key[1])
    cb.set_label(label_str, **vdict)
    out_file_name = '{}-plane.pdf'.format(key[1])
    FigureCanvas(figure).print_figure(out_file_name,bbox_inches='tight')

if __name__ == '__main__': 
    run()
