#!/usr/bin/env python2.7

"""
Top level for post-fit routines. At the moment not sure if this includes 
aggregation. 
"""

import argparse
import time
import sys, os
import yaml
import itertools
from stop.postfit import split_to_planes, numpy_plane_from_dict
import h5py
import numpy as np
from stop.bullshit import FlatProgressMeter

def _load_plotters(): 
    from matplotlib.backends.backend_agg import FigureCanvasAgg as FigureCanvas
    from matplotlib.figure import Figure
    import matplotlib.pyplot as plt
    from mpl_toolkits.axes_grid1 import make_axes_locatable
    import matplotlib as mp
    from stop.style import ax_labels, vdict, hdict

def run(): 
    d='default: %(default)s'
    parser = argparse.ArgumentParser(description=__doc__)
    subs = parser.add_subparsers(dest='which')

    agg = subs.add_parser('agg', description=_aggregate.__doc__)
    agg.add_argument('yaml_files', help='aggregate these', nargs='+')
    agg.add_argument(
        '-o','--output-file', help='should be .h5 or .yml ' + d, 
        default='all-fits.h5')

    yaml_reader = subs.add_parser('yml')
    yaml_reader.add_argument('yaml_file')
    yaml_reader.add_argument('-o', '--out-file')

    plotter = subs.add_parser('plot', description=_plot.__doc__)
    plotter.add_argument('hdf_file')
    plotter.add_argument('-o', '--out-dir', default='plots')

    plotter = subs.add_parser('sum', description=_plot_sum_fom.__doc__)
    plotter.add_argument('hdf_file')
    plotter.add_argument('-o', '--out-dir', default='plots')
    plotter.add_argument(
        '-f', '--fom', choices={'tanh','log','nex'}, default='nex', 
        help='figure of merit, default: %(default)s')

    args = parser.parse_args(sys.argv[1:])
    subroutines = {'yml':_yml_parser, 'plot':_plot, 'sum':_plot_sum_fom, 
                   'agg': _aggregate}
    subroutines[args.which](args)

def _aggregate(config): 
    """
    aggregate yaml output files from fits. 
    """
    allowed = {'.h5','.yml'}
    if not os.path.splitext(config.output_file)[1] in allowed: 
        raise OSError(
            'extension should be in {}'.format(' or '.join(allowed)))
    all_points = []
    n_files = len(config.yaml_files)
    prog = FlatProgressMeter(n_files)
    for filen, yfile in enumerate(config.yaml_files): 
        prog.update(filen)
        with open(yfile) as result: 
            res_dict = yaml.load(result)
        all_points.append(res_dict)

    if config.output_file.endswith('.yml'): 
        with open(config.output_file,'w') as yml: 
            yml.writelines(yaml.dump(all_points))
    elif config.output_file.endswith('.h5'): 
        save_h5(all_points, config.output_file)

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

    with h5py.File(out_file_name,'w') as h5_file: 
        for config in configs: 
            sp_group = h5_file.create_group(config)
            for sp in signal_points: 
                plane_list = planes[config, sp]
                array, extents, ax_names = numpy_plane_from_dict(plane_list)
                ds = sp_group.create_dataset(
                    sp, data=array, compression='gzip')
                ds.attrs['extent'] = extents
                ds.attrs['ax_names'] = ax_names

def _plot(args): 
    """
    Makes all the plots
    """
    _load_plotters()
    hdf = h5py.File(args.hdf_file)
    for config, sp_group in hdf.iteritems(): 
        for sp, dataset in sp_group.iteritems(): 
            array = np.array(dataset)
            extents = dataset.attrs['extent']
            ax_names = dataset.attrs['ax_names']
            
            out_file_name = '{}-plane.pdf'.format(sp)
            if not os.path.isdir(args.out_dir): 
                os.mkdir(args.out_dir)
            out_path = os.path.join(args.out_dir, out_file_name)
            print 'plotting {}'.format(out_path)
            _draw_plot(array, extents, (config, sp), ax_names=ax_names, 
                       out_path=out_path)

# --- several figures of merit to choose from
def _is_excluded(array): 
    base = np.zeros(array.shape)
    base[array < 1.0] += 1
    return base
def _nlog(array): 
    return -np.log(array)
def _ntanh(array): 
    return -np.tanh(array)
_fom_funcs = {'tanh': _ntanh, 'log': _nlog, 'nex': _is_excluded}

def _plot_sum_fom(args): 
    """
    Sums over signal points in the pt / eta plane, makes a plot. 
    """
    _load_plotters()
    fom_func = _fom_funcs[args.fom]
    hdf = h5py.File(args.hdf_file)
    base_ds = next(next(hdf.itervalues()).itervalues())
    counts_array = np.zeros(base_ds.shape)
    extents = base_ds.attrs['extent']
    ax_names = base_ds.attrs['ax_names']
    for config, sp_group in hdf.iteritems(): 
        for sp, dataset in sp_group.iteritems(): 
            array = np.array(dataset)
            counts_array += fom_func(array)
        
        out_file_name = '{}-{}.pdf'.format(args.fom,config)
        if not os.path.isdir(args.out_dir): 
            os.mkdir(args.out_dir)
        out_path = os.path.join(args.out_dir, out_file_name)
        _draw_plot(counts_array, extents, (config, args.fom), 
                   ax_names, out_path)


def _get_label(label): 
    for lab in ax_labels: 
        if lab in label: 
            return ax_labels[lab].axis_label

def _draw_plot(array, extents, key, ax_names, out_path): 
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
    FigureCanvas(figure).print_figure(out_path, bbox_inches='tight')

if __name__ == '__main__': 
    run()
