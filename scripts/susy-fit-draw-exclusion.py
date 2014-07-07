#!/usr/bin/env python3.3
"""
Top level for routines to draw excluded contour.
"""
_best_help = 'use best cls value from all regions'
import argparse, sys
from scharm.limits import planeplt
import yaml

def run():
    d = 'default: %(default)s'
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('cls_file')
    parser.add_argument('-o', '--output-plot', default='plane.pdf')
    parser.add_argument('-f', '--filter-stop', action='store_true')
    dowhat = parser.add_mutually_exclusive_group()
    dowhat.add_argument('-b', '--band-region')
    dowhat.add_argument('--best', action='store_true', help=_best_help)
    dowhat.add_argument('--best-regions', action='store_true',
                        help='show which region is used for each point')
    dowhat.add_argument('--heatmap', action='store_true')
    args = parser.parse_args(sys.argv[1:])
    if args.best or args.best_regions:
        _max_exclusion_plane(args, show_regions=args.best_regions)
    elif args.band_region:
        _make_exclusion_plane(args)
    else:
        _multi_exclusion_plane(args)

def _only_scharm_mass(cls_dict):
    """filter out the stop cross-check points"""
    filt = {}
    for config, points in cls_dict.items():
        filt[config] = []
        for pt in points:
            if pt['scharm_mass'] - pt['lsp_mass'] > 90:
                filt[config].append(pt)
    return filt

def _make_exclusion_plane(args):
    with open(args.cls_file) as cls_yml:
        cls_dict = yaml.load(cls_yml)
        if args.filter_stop:
            cls_dict = _only_scharm_mass(cls_dict)

    ex_plane = planeplt.CLsExclusionPlane()
    ex_plane.lw = 1.5
    cls_list = []
    if args.band_region:
        for sp in cls_dict[args.band_region]:
            sch, lsp = sp['scharm_mass'], sp['lsp_mass']
            low, high = sp['cls_down_1_sigma'], sp['cls_up_1_sigma']
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
    with open(args.cls_file) as cls_yml:
        cls_dict = yaml.load(cls_yml)
        if args.filter_stop:
            cls_dict = _only_scharm_mass(cls_dict)

    ex_plane = planeplt.CLsExclusionPlane()
    ex_plane.lw = 1.5
    point_dict = {}
    for conf_name, cls_list in sorted(cls_dict.items()):
        for sp in cls_list:
            sch, lsp = sp['scharm_mass'], sp['lsp_mass']
            low, high = sp['cls_down_1_sigma'], sp['cls_up_1_sigma']
            expt = sp['cls_exp']
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
    with open(args.cls_file) as cls_yml:
        cls_dict = yaml.load(cls_yml)
        if args.filter_stop:
            cls_dict = _only_scharm_mass(cls_dict)

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
            low, high = sp['cls_down_1_sigma'], sp['cls_up_1_sigma']
            band_tups.append( (sch, lsp, low, high))
            line_tups.append( (sch, lsp, sp['cls_exp']) )
        ex_plane.add_config(line_tups, conf_name, style=color,
                            heatmap=args.heatmap)
        if len(sort_cls) <= 3 and not args.heatmap:
            ex_plane.add_band(band_tups, color=color)

    ex_plane.save(args.output_plot)


if __name__ == '__main__':
    run()
