#!/usr/bin/env python2.7

"""
converts a list of datasets to the corresponding dq2-able strings
"""

import argparse
import sys

def _aggressive_match(short_name, possible_matches, 
                      secondary='susyfilt'): 
    matches = []
    for key in possible_matches: 
        if short_name in key: 
            matches.append(key)

    if len(matches) == 1: 
        return matches[0]
    elif len(matches) > 1: 
        new_matches = [x for x in matches if secondary in x]
        if len(new_matches) == 1: 
            return new_matches[0]
        elif new_matches: 
            raise LookupError("{} didn't kill them all".format(secondary)) 
        else: 
            raise LookupError("{} killed them all".format(secondary)) 

    else: 
        raise LookupError('no match for {} found'.format(short_name))
                          

def get_dq2_ids(dataset_names, lookup_file='susy_crosssections.txt'): 

    key_lines = {}
    name_to_number = {}
    with open(lookup_file) as susy_file: 
        for line in susy_file: 
            if not key_lines: 
                for num, key in enumerate(line.split(':')): 
                    key_lines[key] = num
                continue
        
            line = line.split('#')[0].strip()
            if line: 
                number, name = line.split()[0:2]
                name_to_number[name] = number
                
    match_pairs = []
    failed_matches = []
    for ds in dataset_names: 
        if ds in name_to_number: 
            match_pairs.append( (ds, name_to_number[ds]))
        else: 
            try: 
                new_ds = _aggressive_match(ds, name_to_number.keys())
                match_pairs.append( (new_ds, name_to_number[new_ds]))
            except LookupError as e: 
                failed_matches.append('{}: {}'.format(ds,str(e)))

    return match_pairs, failed_matches

if __name__ == '__main__': 
    
    parser = argparse.ArgumentParser(
        description=__doc__, 
        epilog='author: Dan Guest')
    parser.add_argument('lookup', help='susy x-sec textfile')
    parser.add_argument('datasets_file', nargs='?', 
                        type=argparse.FileType('r'), default=sys.stdin)
    parser.add_argument('--prepend', default='mc11_7TeV')
    
    args = parser.parse_args(sys.argv[1:])

    datasets = []

    for line in args.datasets_file: 
        line = line.split('#')[0].strip()
        if line: 
            datasets.append(line.split()[0].strip())

    matched, unmatched = get_dq2_ids(datasets, args.lookup)

    for ds, number in matched:
        print '{pre}.{num}.*{name}*NTUP_SUSY.*/'.format(
            pre=args.prepend, num=number, name=ds)
    
    for lin in unmatched: 
        if lin: 
            sys.stderr.write(lin + '\n')
