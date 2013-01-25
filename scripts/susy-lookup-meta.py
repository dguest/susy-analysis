#!/usr/bin/env python2.7
"""
Script to consolidate all the needed meta info in one place. 

Should be usable in a few ways: 
 - to generate a meta file that is read in by susy-run-cutflow. The cutflow 
   routine is responsible for mapping out the datasets and running over 
   them 
 - to update the meta info before feeding the meta info to stacking routines.
"""


from pyAMI.client import AMIClient
from pyAMI.auth import AMI_CONFIG, create_auth_config
import os, sys
from stop.meta import MetaFactory, DatasetCache
import argparse, ConfigParser

def run(): 
    
    client = AMIClient()
    if not os.path.exists(AMI_CONFIG):
       create_auth_config()
    client.read_config(AMI_CONFIG)

    parser = argparse.ArgumentParser(
        # parents=[preparser], 
        description=__doc__, 
        epilog='Author: Dan Guest', 
        )

    defaults = { 
        'output_pickle': 'meta.pkl', 
        }

    parser.set_defaults(**defaults)
    parser.add_argument('steering_file')
    parser.add_argument('--susy-lookup', help='default: %(default)s')
    parser.add_argument('--output-pickle', help='default: %(default)s')
    parser.add_argument('--config-file', help='default: %(default)s', 
                        default='stop.cfg')
    parser.add_argument('--base-meta', help='use this for a basis', 
                        default='meta.pkl')

    args = parser.parse_args(sys.argv[1:])

    config_parser = ConfigParser.SafeConfigParser()
    config_parser.read([args.config_file])

    if not os.path.isfile(args.base_meta): 
       args.base_meta = None

    if args.base_meta: 
       datasets = DatasetCache(args.base_meta)
    else: 
       datasets = None
    mf = MetaFactory(datasets)
    with open(args.steering_file) as steering: 
       mf.build_base_ds(steering)
    
    mf.verbose = True
    mf.no_ami_overwrite = True
    mf.dump_sources()
    if args.susy_lookup: 
       with open(args.susy_lookup) as susy:
          mf.lookup_susy(susy)
    mf.lookup_ami(client)
    mf.dump_sources()
    mf.write_meta(args.output_pickle)
    

if __name__ == '__main__': 
   run()
