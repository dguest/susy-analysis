#!/usr/bin/env python3
"""
Top level for routines to draw excluded contour.
"""
_best_help = 'use best cls value from all regions'
_regions_help = 'only plot a subset of regions'
_clean_help = 'suppress signal points and grid'
_ul_help = 'show upper limits'
_mono_help = 'include monojet limits (expects a csv file with points to draw)'
_regs_help = 'show which region is used for each point'
_ext_help = 'plotting tweaks for external plots'
import argparse, sys
from scharm.limits import planeplt
from scharm.bullshit import helvetify
import yaml

def run():
    d = 'default: %(default)s'
    b = dict(action='store_true') # boolian
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('cls_file')
    parser.add_argument('-o', '--output-plot', default='plane.pdf')
    parser.add_argument(
        '-r', '--regions', help=_regions_help, nargs='+', default=[])
    parser.add_argument('-i', '--interpolation', default='gauss',
                        choices=planeplt.interpolators)
    parser.add_argument('-e', '--external', help=_ext_help, **b)
    dowhat = parser.add_mutually_exclusive_group()
    dowhat.add_argument('-b', '--band-region')
    dowhat.add_argument('--best', help=_best_help, **b)
    dowhat.add_argument('--best-regions', help=_regs_help, **b)
    dowhat.add_argument('--heatmap', **b)
    dowhat.add_argument('--noband', **b)
    dowhat.add_argument('--clean', help=_clean_help, **b)
    dowhat.add_argument('--ul', help=_ul_help, **b)
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
        if not all(x in sp for x in ['exp_d1s', 'exp_u1s', 'exp']):
            pt_name = '{}-{}'.format(self.ms, self.ml)
            haveargs = ', '.join(sp.keys())
            if sp.get('ul') == -1:
                raise NullPointError(self.ms, self.ml)
            err = 'expected CLs missing for {}, keys: {}'.format(
                pt_name, haveargs)
            raise KeyError(err)
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

class NullPointError(Exception):
    def __init__(self, ms, ml):
        super().__init__('point with no expected cls')
        self.xy = (ms, ml)

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
        interpolation=args.interpolation,
        high_contrast=args.external)

    ex_plane = planeplt.CLsExclusionPlane(**plane_opts)
    ex_plane.approved = False and args.external # WAIT FOR APPROVAL
    pdict = _get_max_expected_points(cls_dict)

    if show_regions:
        cls_list = [ x.cfg_tup() for x in pdict.values()]
        if args.external and False: # WAIT FOR SOMEONE TO ASK
            ex_plane.add_labels(0.5)
    else:
        cls_list = [ x.cfg_tup()[:3] for x in pdict.values()]
        config = args.regions[0] if len(args.regions) == 1 else None
        ex_plane.add_labels(config=config)

    band_list = [ x.lowhigh_tup() for x in pdict.values()]
    ex_plane.add_band(band_list, label='expected')

    ex_plane.add_observed(pdict.values())
    ex_plane.add_config(cls_list, 'expected', '-darkblue')
    if ul:
        ex_plane.add_upper_limits(pdict.values())
    if args.mono:
        montit = 'Monojet [1407.0608]'
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
    # HACK: poison points (where no expected CLs is found, but UL is
    # defined) are removed only to keep us consistent with the limits
    # we showed before.
    poison_points = set()
    for conf_name, cls_list in sorted(cls_dict.items()):
        for sp in cls_list:
            try:
                pt = Point(sp, conf_name)
            except NullPointError as err:
                poison_points.add(err.xy)
                continue
            if pt.xy() not in pdict or pdict[pt.xy()].expt > pt.expt:
                pdict[pt.xy()] = pt
    return {key: pdict[key] for key in set(pdict) - poison_points}

# ____________________________________________________________________________
# overlay multiple exclusions

def _multi_exclusion_plane(args):
    cls_dict = _load_subset(args.cls_file, args.regions)

    ex_plane = planeplt.CLsExclusionPlane(interpolation=args.interpolation)
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
