#!/usr/bin/env python2.7

"""
Top level for post-fit routines. At the moment not sure if this includes 
aggregation. 
"""

import argparse
import sys
import yaml
from stop.postfit import split_to_planes, numpy_plane_from_dict

def run(): 
    parser = argparse.ArgumentParser()
    parser.add_argument('yaml_file')
    args = parser.parse_args(sys.argv[1:])
    with open(args.yaml_file) as yml: 
        point_list = yaml.load(yml)
    hdify(point_list)

def hdify(point_list): 
    planes = split_to_planes(point_list)
    for plane_key, plane in planes.iteritems():
        numpy_plane_from_dict(plane)

if __name__ == '__main__': 
    run()
