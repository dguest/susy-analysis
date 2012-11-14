#!/usr/bin/env python2.7

import cPickle
import argparse
import sys, os

_dump_string = '{name:{nsize}} {xsec:>10} {evts:>10} {xsec_per_evt:>10}'

def dump_ds_info(value, nsize=30):
    vals = {}
    for v in dir(value): 
        if v.startswith('_'): continue
        vals[v] = getattr(value, v)
    print _dump_string.format(nsize=nsize, **vals)
        

if __name__ == '__main__': 
    parser = argparse.ArgumentParser()
    parser.add_argument('pickle', help='pickle to read in')
    args = parser.parse_args(sys.argv[1:])
    
    with open(args.pickle) as pkl: 
        the_dict = cPickle.load(pkl)
        
    nsize = max(len(k) for k in the_dict.keys())
    print _dump_string.format(name='# name', xsec='xsec', evts='evts', 
                              xsec_per_evt='xsec_per_evt', nsize=nsize)
    for name, value in sorted(list(the_dict.iteritems())): 
        try: 
            dump_ds_info(value, nsize + 1)
        except KeyError:
            print name, value
    
