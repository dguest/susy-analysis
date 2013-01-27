#!/usr/bin/env python2.7
"""
Script to consolidate all the needed meta info in one place. 

Should be usable in a few ways: 
 - to generate a meta file that is read in by susy-run-cutflow. The cutflow 
   routine is responsible for mapping out the datasets and running over 
   them 
 - to update the meta info before feeding the meta info to stacking routines.
"""


import os, sys
from stop.meta import MetaFactory, DatasetCache
import argparse, ConfigParser


def _get_parser(): 
    parser = argparse.ArgumentParser(
        formatter_class=argparse.RawDescriptionHelpFormatter, 
        description=__doc__, 
        epilog='Author: Dan Guest', 
        )

    def_meta = 'meta.pkl'
    defaults = { 
        'output_pickle': def_meta, 
        'steering_file': def_meta
        }
    
    d = 'default: %(default)s'

    parser.set_defaults(**defaults)
    parser.add_argument(
        'steering_file', 
        help=('either a textfile of datasets'
              ' or an existing meta file ({})'.format(d)))
    parser.add_argument('--susy-lookup', help=d)
    parser.add_argument('--output-pickle', help=d)
    parser.add_argument('--add-meta', 
                        help='add this meta ({})'.format(d))
    parser.add_argument('-f', action='store_true',
                        help='force overwrite of meta (if it exists)')
    parser.add_argument('-a', action='store_true',
                        help='ami lookup')
    parser.add_argument('-t', '--trust-ds-names', action='store_true')
    parser.add_argument('-d','--dump', action='store_true')
    parser.add_argument(
        '--write-steering', help='rewrite steering file')

    return parser

def run(): 
    args = _get_parser().parse_args(sys.argv[1:])

    text_in = args.steering_file.endswith('.txt')
    if os.path.isfile(args.output_pickle) and text_in and not args.f: 
        sys.exit('output-pickle exists already, refusing to overwrite')

    mf = MetaFactory(args.steering_file)
    
    mf.verbose = True

    if args.susy_lookup: 
       with open(args.susy_lookup) as susy:
          mf.lookup_susy(susy)

    mf.processes = 10
    if args.a: 
        print 'looking up names in ami'
        mf.lookup_ami_names(args.trust_ds_names)
        mf.write_meta(args.output_pickle)
    if args.write_steering: 
        missing = [ds.name for ds in mf.get_unnamed_ds()]
        if missing: 
            print 'missing dataset names:'
            for m in missing: 
                print m
        with open(args.write_steering, 'w') as st: 
            for ds in mf.get_found_full_ds_names(): 
                st.write(ds.strip('/') + '/\n')
    if args.a: 
        mf.lookup_ami(stream=sys.stdout)
    if args.dump: 
        mf.dump()
    mf.write_meta(args.output_pickle)

if __name__ == '__main__': 
   run()
