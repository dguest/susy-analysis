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
    args = parser.parse_args(sys.argv[1:])
    _make_exclusion_plane(args)

def _make_exclusion_plane(args):
    with open(args.ul_file) as ul_yml:
        ul_list = yaml.load(ul_yml)

    ex_plane = planeplt.ExclusionPlane(threshold=1.0)
    for lim in ['lower','mean','upper']:
        ul_tup = [(x['scharm_mass'], x['lsp_mass'], x[lim]) for x in ul_list]
        ex_plane.add_config(ul_tup, lim)
    ex_plane.save(args.output_plot)

if __name__ == '__main__':
    run()
