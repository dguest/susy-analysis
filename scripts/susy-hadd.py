#!/usr/bin/env python3.3
"""
add .h5 histogram files
"""
import argparse, sys
from scharm.hists import HistNd, HistAdder
from scharm.aggregate import histadd

def run(): 
    config = get_config()
    histadd.hadd(config)

def get_config(): 
    d = 'default: %(default)s'
    c = "with no argument is '%(const)s'"

    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('input_hists', nargs='+')
    parser.add_argument('-o', '--output', 
                          help='hist or (for dash-hadd) directory')
    parser.add_argument('-a', '--aggressive', action='store_true')
    parser_method = parser.add_mutually_exclusive_group()
    parser_method.add_argument(
        '-d', '--dash-hadd', action='store_true', 
        help='multiple hadds, splits input hists at \'-\'')
    parser_method.add_argument(
        '-r', '--recursive', action='store_true', 
        help='multiple hadds, splits input by dir and hists at \'-\'')
    parser.add_argument('--norm', help=(
            'normalize using this meta file (scales to 1 fb^-1)'))
    parser.add_argument(
        '-f','--fast', action='store_true', 
        help='disable some checks')

    return parser.parse_args(sys.argv[1:])

if __name__ == '__main__': 
    run()
