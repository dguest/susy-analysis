#!/usr/bin/env python2.7
"""
reads in an optimum cuts textfile, produces signal significance plane
"""

import sys, argparse

def make_sr_dict(textfile): 
    return textfile


def run(): 
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('input_textfiles', nargs='+')
    args = parser.parse_args(sys.argv[1:])
    for f in args.input_textfiles: 
        sr_dict = make_sr_dict(f)
        sr_dict
