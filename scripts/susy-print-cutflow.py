#!/usr/bin/env python2.7
"""
dump cutflow info to text
"""

import sys, os
import argparse, ConfigParser
import cPickle


if __name__ == '__main__': 
    
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('input_pickle')
    parser.add_argument(
        '--filter', nargs='+', 
        help='print only samples with names containing these strings')

    args = parser.parse_args(sys.argv[1:])
    
    with open(args.input_pickle,'rb') as pkl: 
        all_cuts = cPickle.load(pkl)

    names = all_cuts['names']
    
    if args.filter: 
        def name_filter(pair): 
            for m in args.filter: 
                if m in pair[0]: 
                    return True
            return False

        all_cuts = dict(filter(name_filter, all_cuts.items()))

    overall_format = '1f'
    num_format = '{{:.{}}}'.format(overall_format)
    write_format = '{{:>{{w}}.{}}}'.format(overall_format)
    widths = {}
    for sample, cut_list in all_cuts.iteritems(): 
        if cut_list and sample != 'names':
            lowpre = [num_format.format(x) for x in cut_list]
            widths[sample] = max(len(x) for x in lowpre + [sample]) + 1

    all_samples = all_cuts.keys()
    sys.stdout.write(''.rjust(24))
    for sample in all_samples: 
        if sample in widths:
            sys.stdout.write(sample.rjust(widths[sample]))
    sys.stdout.write('\n')
    for cut_num, cut_name in enumerate(names): 
        sys.stdout.write(cut_name.rjust(23) + ':')
        for sample in all_samples: 
            if sample == 'names' or sample not in widths: 
                continue
            if cut_num < len(all_cuts[sample]): 
                val = all_cuts[sample][cut_num]
            else: 
                val = 0
            sys.stdout.write(write_format.format(val, w=widths[sample]))
        sys.stdout.write('\n')
