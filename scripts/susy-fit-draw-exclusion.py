#!/usr/bin/env python3.3
"""
Top level for routines to draw excluded contour.
"""
_best_help = 'use best cls value from all regions'
_regions_help = 'only plot a subset of regions'
_clean_help = 'suppress signal points and grid'
_ul_help = 'show upper limits'
_mono_help = 'include monojet limits (expects a csv file with points to draw)'
import argparse, sys
from scharm.limits import planeplt
from scharm.bullshit import helvetify
import yaml

def run():
    d = 'default: %(default)s'
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('cls_file')
    parser.add_argument('-o', '--output-plot', default='plane.pdf')
    parser.add_argument('-r', '--regions', help=_regions_help, nargs='+')
    parser.add_argument('-i', '--interpolation', default='gauss',
                        choices=planeplt.interpolators)
    dowhat = parser.add_mutually_exclusive_group()
    dowhat.add_argument('-b', '--band-region')
    dowhat.add_argument('--best', action='store_true', help=_best_help)
    dowhat.add_argument('--best-regions', action='store_true',
                        help='show which region is used for each point')
    dowhat.add_argument('--heatmap', action='store_true')
    dowhat.add_argument('--noband', action='store_true')
    dowhat.add_argument('--clean', action='store_true', help=_clean_help)
    dowhat.add_argument('--ul', action='store_true', help=_ul_help)
    dowhat.add_argument('--mono', help=_mono_help)
    args = parser.parse_args(sys.argv[1:])
    helvetify()
    if any([args.best, args.best_regions, args.clean, args.ul, args.mono]):
        _max_exclusion_plane(args, show_regions=args.best_regions,
                             clean=args.clean, ul=args.ul)
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

# __________________________________________________________________________
# single exclusion plane

def _make_exclusion_plane(args):
    """show exclusion plane for one config"""
    cls_dict = _load_subset(args.cls_file, args.regions)

    ex_plane = planeplt.CLsExclusionPlane(interpolation=args.interpolation)
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

# ___________________________________________________________________________
# "best" exclusion

class Point:
    """minimal class to hold point info"""
    def __init__(self, sp, config_name):
        self.ms, self.ml = sp['scharm_mass'], sp['lsp_mass']
        self.low, self.high = sp['exp_d1s'], sp['exp_u1s']
        self.expt = sp['exp']
        self.obs = sp['obs']
        self.obs_high = sp.get('obs_u1s', self.obs)
        self.obs_low = sp.get('obs_d1s', self.obs)
        self.xsec = sp.get('xsec')
        ul = sp.get('ul')
        self.ul = None if ul == -1 else ul
        self.config_name = config_name

    def cfg_tup(self):
        return self.ms, self.ml, self.expt, self.config_name

    def lowhigh_tup(self):
        return self.ms, self.ml, self.low, self.high

    def xy(self):
        return self.ms, self.ml

def _max_exclusion_plane(args, show_regions=False, clean=False, ul=False):
    """
    Exclusion plane using the "best" region for each point. With show_regions
    will show what region that is.
    """
    cls_dict = _load_subset(args.cls_file, args.regions)

    plane_opts = dict(
        grid = not clean and not ul,
        show_points = not any([clean, ul, args.mono]),
        kinematic_bounds = 'both',
        interpolation=args.interpolation)

    ex_plane = planeplt.CLsExclusionPlane(**plane_opts)
    ex_plane.lw = 1.5
    ex_plane.approved = True
    pdict = _get_max_expected_points(cls_dict)

    if show_regions:
        cls_list = [ x.cfg_tup() for x in pdict.values()]
    else:
        cls_list = [ x.cfg_tup()[:3] for x in pdict.values()]
        ex_plane.add_labels()

    band_list = [ x.lowhigh_tup() for x in pdict.values()]
    ex_plane.add_band(band_list, label='expected')

    ex_plane.add_observed(pdict.values())
    ex_plane.add_config(cls_list, 'expected', '-darkblue')
    if ul:
        ex_plane.add_upper_limits(pdict.values())
    if args.mono:
        montit = 'Monojet     [1407.0608]'
        ex_plane.add_exclusion(_xy_from_csv(args.mono), montit, pushdown=True)

    ex_plane.save(args.output_plot)

def _xy_from_csv(file_path):
    """return (x, y) values from input file"""

    def pt_from_line(line):
        return [float(x) for x in line.split(',')]

    with open(file_path) as csv:
        return [pt_from_line(x) for x in csv]

def _get_max_expected_points(cls_dict):
    """helper for _max_exclusion_plane"""
    pdict = {}
    for conf_name, cls_list in sorted(cls_dict.items()):
        for sp in cls_list:
            pt = Point(sp, conf_name)
            if pt.xy() not in pdict or pdict[pt.xy()].expt > pt.expt:
                pdict[pt.xy()] = pt
    return pdict

# ____________________________________________________________________________
# overlay multiple exclusions

def _multi_exclusion_plane(args):
    cls_dict = _load_subset(args.cls_file, args.regions)

    ex_plane = planeplt.CLsExclusionPlane(interpolation=args.interpolation)
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
