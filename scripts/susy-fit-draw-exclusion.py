#!/usr/bin/env python3.3
"""
Top level for routines to draw excluded contour.
"""
import argparse, sys
from scharm.limits import planeplt
import yaml

def run():
    d = 'default: %(default)s'
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('ul_file')
    parser.add_argument('-o', '--output-plot', default='plane.pdf')
    parser.add_argument('-b', '--band-region')
    args = parser.parse_args(sys.argv[1:])
    _make_exclusion_plane(args)

def _make_exclusion_plane(args):
    with open(args.ul_file) as ul_yml:
        ul_dict = yaml.load(ul_yml)

    ex_plane = planeplt.ExclusionPlane(threshold=1.0)
    ex_plane.lw = 1.5
    lims_list = []
    if args.band_region:
        for sp in ul_dict[args.band_region]:
            sch, lsp = sp['scharm_mass'], sp['lsp_mass']
            low, high = sp['lower'], sp['upper']
            lims_list.append( (sch, lsp, low, high))
        ex_plane.add_band(lims_list)

    # add middle band
    def get_tup(x):
        return (x['scharm_mass'], x['lsp_mass'], x['mean'])
    for conf_name, ul_list in ul_dict.items():
        ul_tup = [get_tup(x) for x in ul_list]
        style = '-k' if conf_name == args.band_region else None
        ex_plane.add_config(ul_tup,conf_name, style=style)

    ex_plane.save(args.output_plot)

if __name__ == '__main__':
    run()
