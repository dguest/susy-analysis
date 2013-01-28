#!/usr/bin/env python2.7

from susy.distiller import Distiller
from argparse import ArgumentParser
# from ConfigParser import SafeConfigParser as Config
import sys
from os.path import isfile, isdir

def run(): 
    parser = ArgumentParser(description=__doc__)
    parser.add_argument(
        'meta-info', destination='meta_info_path', 
        help='input meta file for steering')
    parser.add_argument('-o','--out-dir', default='whiskey')
    parser.add_argument('-v','--verbose', action='store_true')
    parser.add_argument('--d3pd-dir', default='D3PDs')
    args = parser.parse_args(sys.argv[1:])

    fail_conditions = [ 
        not isdir(args.d3pd_dir), 
        not isfile(args.meta_info_path)
        ]
    if any(fail_conditions): 
        sys.exit("misconfiguration! maybe you're missing some inputs")

    distiller = Distiller(args)
    distiller.find_input_files(args.d3pd_dir)
    distiller.distill()

if __name__ == '__main__': 
    run()
