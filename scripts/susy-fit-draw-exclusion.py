#!/usr/bin/env python3.3
"""
Top level for routines to draw excluded contour.
"""
_best_help = 'use best cls value from all regions'
_regions_help = 'only plot a subset of regions'
import argparse, sys
from scharm.limits import planeplt
import yaml

def run():
    d = 'default: %(default)s'
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('cls_file')
    parser.add_argument('-o', '--output-plot', default='plane.pdf')
    parser.add_argument('-r', '--regions', help=_regions_help, nargs='+')
    dowhat = parser.add_mutually_exclusive_group()
    dowhat.add_argument('-b', '--band-region')
    dowhat.add_argument('--best', action='store_true', help=_best_help)
    dowhat.add_argument('--best-regions', action='store_true',
                        help='show which region is used for each point')
    dowhat.add_argument('--heatmap', action='store_true')
    dowhat.add_argument('--noband', action='store_true')
    args = parser.parse_args(sys.argv[1:])
    if args.best or args.best_regions:
        _max_exclusion_plane(args, show_regions=args.best_regions)
    elif args.band_region:
        _make_exclusion_plane(args)
    else:
        _multi_exclusion_plane(args)

def _load_subset(cls_file, subset):
    """
    load only a subset of the cls entries, checking to make sure they
    all exist
    """
    with open(cls_file) as cls_yml:
        cls_dict = yaml.load(cls_yml)

    if not subset:
        return cls_dict

    try:
        return {x:cls_dict[x] for x in subset}
    except KeyError as err:
        keys = ', '.join(cls_dict.keys())
        raise ValueError('{} not in {}'.format(err.args[0], keys))

def _make_exclusion_plane(args):
    cls_dict = _load_subset(args.cls_file, args.regions)

    ex_plane = planeplt.CLsExclusionPlane()
    ex_plane.lw = 1.5
    cls_list = []
    if args.band_region:
        for sp in cls_dict[args.band_region]:
            sch, lsp = sp['scharm_mass'], sp['lsp_mass']
            low, high = sp['exp_d1s'], sp['exp_u1s']
            cls_list.append( (sch, lsp, low, high))
        ex_plane.add_band(cls_list)

    # add middle band
    def get_tup(x):
        return (x['scharm_mass'], x['lsp_mass'], x['cls_exp'])
    for conf_name, cls_list in sorted(cls_dict.items()):
        cls_tup = [get_tup(x) for x in cls_list]
        style = '-k' if conf_name == args.band_region else None
        ex_plane.add_config(cls_tup,conf_name, style=style, )

    ex_plane.save(args.output_plot)

def _max_exclusion_plane(args, show_regions=False):
    """
    Exclusion plane using the "best" region for each point. With show_regions
    will show what region that is.
    """
    cls_dict = _load_subset(args.cls_file, args.regions)

    ex_plane = planeplt.CLsExclusionPlane()
    ex_plane.lw = 1.5
    point_dict = {}
    for conf_name, cls_list in sorted(cls_dict.items()):
        for sp in cls_list:
            sch, lsp = sp['scharm_mass'], sp['lsp_mass']
            low, high = sp['exp_d1s'], sp['exp_u1s']
            expt = sp['exp']
            if not (sch, lsp) in point_dict or point_dict[sch, lsp][0] > expt:
                point_dict[sch, lsp] = (expt, low, high, conf_name)

    if show_regions:
        cls_list = [ (x[0], x[1], y[0], y[3]) for x,y in point_dict.items()]
    else:
        cls_list = [ (x[0], x[1], y[0]) for x,y in point_dict.items()]
    ex_plane.add_config(cls_list, 'best expected', '-k')

    band_list = [ (x[0], x[1], y[1], y[2]) for x,y in point_dict.items()]
    ex_plane.add_band(band_list)

    ex_plane.save(args.output_plot)


def _multi_exclusion_plane(args):
    cls_dict = _load_subset(args.cls_file, args.regions)

    ex_plane = planeplt.CLsExclusionPlane()
    ex_plane.lw = 1.5
    colors = list('rgbmc') + ['orange']
    sort_cls = sorted(cls_dict.items())
    if args.heatmap and len(cls_dict) > 1:
        raise ValueError("can't do heatmaps for more than one contour")
    for color, (conf_name, cls_list) in zip(colors, sort_cls):
        band_tups = []
        line_tups = []
        for sp in cls_list:
            sch, lsp = sp['scharm_mass'], sp['lsp_mass']
            line_tups.append( (sch, lsp, sp['exp']) )
            if all('exp_{}1s'.format(x) in sp for x in 'ud'):
                low, high = sp['exp_d1s'], sp['exp_u1s']
                band_tups.append( (sch, lsp, low, high))
        ex_plane.add_config(line_tups, conf_name, style=color,
                            heatmap=args.heatmap)
        if not any([len(sort_cls) > 3, args.heatmap, args.noband]):
            ex_plane.add_band(band_tups, color=color)

    ex_plane.save(args.output_plot)


if __name__ == '__main__':
    run()
