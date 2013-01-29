#!/usr/bin/env python2.7
"""
A simpler distillation script. 
"""

from susy.distiller import Distiller
import argparse 
# from ConfigParser import SafeConfigParser as Config
import sys
from os.path import isfile, isdir

def run(): 
    parser = argparse.ArgumentParser(
        description=__doc__)
    parser.add_argument('meta_info_path', help='input meta file for steering')

    d = 'default: %(default)s'
    dp = ' ({})'.format(d)
    parser.add_argument('-o','--out-dir', help='output directory' + dp, 
                        default='whiskey')
    parser.add_argument('-v','--verbose', action='store_true', 
                        help='print more help')
    parser.add_argument('--d3pd-dir', default='D3PDs',
                        help='input d3pd directory' + dp)
    parser.add_argument('-f', action='store_true', 
                        help='force rebuild')
    parser.add_argument(
        '-m', '--update-meta', action='store_true', 
        help='update meta only, don\'t rerun distiller')
    args = parser.parse_args(sys.argv[1:])

    fail_conditions = [ 
        not isdir(args.d3pd_dir), 
        not isfile(args.meta_info_path)
        ]
    if any(fail_conditions): 
        sys.exit("misconfiguration! maybe you're missing some inputs")

    distiller = Distiller(args)
    if args.f: 
        distiller.force_rebuild = True
    if args.verbose: 
        print 'mapping inputs'
    distiller.find_input_files(args.d3pd_dir)
    distiller.prepare_dataset_meta()
    if args.update_meta: 
        print 'updated meta, quitting'
        return None
    if args.verbose: 
        print 'running distill'
    distiller.distill()

if __name__ == '__main__': 
    run()
