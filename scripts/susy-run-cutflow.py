#!/usr/bin/env python2.7
"""
susy stop cutflow script. 

At some point I'll probably also add a flag to output some small ntuple 
(for the real Science)
"""

from susy import cutflow 
import sys, os
import argparse, ConfigParser
import warnings



def run_cutflow(samples, susy_lookup, mainz_lookup='SampleListStop.txt',
                data_location='.', flags='v'): 

    debug = 'b' in flags
    
    mainz_cutflow = cutflow.NormedCutflow(mainz_lookup, 
                                          file_format='mainz')

    susy_cutflow = cutflow.NormedCutflow(susy_lookup, 
                                         file_format='official')

    sig_counts = {}
    bg_counts = {}
    for samp in samples: 
        if samp.startswith('Stop'): 

            if debug: 
                print '{} will go to signal'.format(samp)
                continue

            matched_files = mainz_cutflow.add_ds_lookup(samp, data_location)

            if matched_files: 
                sig_counts[samp] = mainz_cutflow.get_normed_counts(
                    samp, flags=flags)

        elif samp == 'Data': 
            if debug: 
                print '{} is Data'.format(samp)
                continue
            
            warnings.warn('Data not implemented')

        else: 
            if debug: 
                print '{} will go to bg'.format(samp)
                continue
    
    cuts_total = {}
    for cf in sig_counts.values(): 
        if not cuts_total: 
            cuts_total = cf
        else: 
            for cut in cuts_total: 
                cuts_total[cut] += cf[cut]

    sorted_cuts = sorted(cuts_total.items(), 
                         key=lambda cut: cut[1], reverse=True)
           
    return sorted_cuts

if __name__ == '__main__': 
    preparser = argparse.ArgumentParser(
        add_help=False, 
        )
    
    preparser.add_argument('--config-file', 
                           default='cutflow.cfg', 
                           help='default: %(default)s')
    args, remaining = preparser.parse_known_args(sys.argv[1:])

    if os.path.isfile(args.config_file): 
        config_parser = ConfigParser.SafeConfigParser()
        config_parser.read([args.config_file])
        default_files = dict(config_parser.items('files'))

    parser = argparse.ArgumentParser(
        parents=[preparser], 
        description=__doc__, 
        epilog='Author: Dan Guest', 
        )
    parser.set_defaults(**default_files)

    parser.add_argument('used_samples', nargs='?', 
                        help='default: %(default)s')
    parser.add_argument('--mainz-lookup', help='default: %(default)s' )
    parser.add_argument('--susy-lookup', help='default: %(default)s')
    parser.add_argument('-t','--terse', action='store_true')
    parser.add_argument('--debug', action='store_true')

    args = parser.parse_args(remaining)

    with open(args.used_samples) as inputs: 
        used_samples = [v.strip() for v in inputs]

    flags = '' if args.terse else 'v'
    if args.debug: 
        flags += 'b'

    cuts = run_cutflow(
        used_samples, 
        susy_lookup=args.susy_lookup, 
        mainz_lookup=args.mainz_lookup, 
        flags=flags, 
        data_location=args.data_location, 
        )

    for name, count in cuts: 
        print '{:>20}: {: >12.2f}'.format(name,count)
