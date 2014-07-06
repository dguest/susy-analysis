#!/usr/bin/env python3.3
"""
Plotting routine for the fit inputs.
"""

import argparse
import sys, yaml

def run():
    """top level routine"""
    args = _get_args()
    with open(args.fit_inputs) as yml:
        inputs_dict = yaml.load(yml)

    


def _get_args():
    """input parser"""
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('fit_inputs')
    parser.add_argument('-o','--plot-directory', default='bg_fit')
    parser.add_argument('-e','--ext', default='.pdf', help='plot type')
    return parser.parse_args()

if __name__ == '__main__':
    run()
