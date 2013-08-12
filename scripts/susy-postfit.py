#!/usr/bin/env python2.7

"""
Top level for post-fit routines. At the moment not sure if this includes 
aggregation. 
"""

import argparse
import sys
import yaml
from stop.postfit import split_to_planes, numpy_plane_from_dict

from matplotlib.backends.backend_agg import FigureCanvasAgg as FigureCanvas
from matplotlib.figure import Figure
import matplotlib.pyplot as plt
from mpl_toolkits.axes_grid1 import make_axes_locatable
import matplotlib as mp
from stop.style import ax_labels, vdict, hdict

def run(): 
    parser = argparse.ArgumentParser()
    parser.add_argument('yaml_file')
    args = parser.parse_args(sys.argv[1:])
    with open(args.yaml_file) as yml: 
        point_list = yaml.load(yml)
    plot(point_list)

def plot(point_list): 
    planes = split_to_planes(point_list)
    for plane_key, plane in planes.iteritems():
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
    FigureCanvas(figure).print_figure('test.png',bbox_inches='tight')

if __name__ == '__main__': 
    run()
