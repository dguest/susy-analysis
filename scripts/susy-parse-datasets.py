#!/usr/bin/env python2.7
"""
parses twiki / maybe other text files into other textfiles for various things
"""


import sys
from argparse import ArgumentParser, FileType

def run(args): 
    dump_from_twiki(args.textfile)

def dump_from_twiki(input_file): 
    for line in input_file: 
        line_array = line.strip().split('|')[1:-1]
        clean_array = [word.strip() for word in line_array]
        needed, ds_name, total_evt, xsec_nb, eff, bugs = clean_array
        if needed == 'yes': 
            print ds_name

if __name__ == '__main__': 
    parser = ArgumentParser(description=__doc__) 
    parser.add_argument('textfile', nargs='?', type=FileType(), 
                        default=sys.stdin)
    args = parser.parse_args(sys.argv[1:])
    run(args)
    
