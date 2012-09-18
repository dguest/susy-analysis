#!/usr/bin/env python2.7
"""
susy stop cutflow script. 

Defaults are set (by default) in cutflow.cfg.

At some point I'll add a flag to output some small ntuple 
(for the real Science)
"""

from susy import cutflow 
import sys, os, re
import argparse, ConfigParser
import warnings
import collections


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

            signal_flags = flags + 's'

            if matched_files: 
                sig_counts[samp] = mainz_cutflow.get_normed_counts(
                    samp, flags=signal_flags)

        elif samp == 'Data': 
            if debug: 
                print '{} is Data'.format(samp)
                continue
            
            warnings.warn('Data not implemented')

        else: 
            if debug: 
                print '{} will go to bg'.format(samp)
                continue

            matched_files = susy_cutflow.add_ds_lookup(samp, data_location)
            if matched_files: 
                bg_counts[samp] = susy_cutflow.get_normed_counts(
                    samp, flags=flags)
    
    def get_zeroed_counter(): 
        return collections.defaultdict(int)
    signal_cutflows = collections.defaultdict(get_zeroed_counter)

    for sample_name, cuts in sig_counts.items(): 

        particle, vx = sample_name.split('_')
        
        for name, value in cuts.items(): 
            signal_cutflows[particle][name] += value

    np_finder = re.compile('Np[0-9]')
    bg_cutflows = collections.defaultdict(get_zeroed_counter)
    for sample_name, cuts in bg_counts.items(): 
        short_name = sample_name
        if np_finder.findall(sample_name): 
            short_name = np_finder.split(sample_name)[0]
        for name, value in cuts.items(): 
            bg_cutflows[short_name][name] += value

    signal_cutflows.update(bg_cutflows)

    return signal_cutflows

if __name__ == '__main__': 
    preparser = argparse.ArgumentParser(
        add_help=False, 
        )
    
    preparser.add_argument('--config-file', 
                           default='cutflow.cfg', 
                           help='default: %(default)s')
    args, remaining = preparser.parse_known_args(sys.argv[1:])


    parser = argparse.ArgumentParser(
        parents=[preparser], 
        description=__doc__, 
        epilog='Author: Dan Guest', 
        )

    if os.path.isfile(args.config_file): 
        config_parser = ConfigParser.SafeConfigParser()
        config_parser.read([args.config_file])
        default_files = dict(config_parser.items('files'))

        parser.set_defaults(**default_files)

    parser.add_argument('used_samples', nargs='?', 
                        help='default: %(default)s')
    parser.add_argument('--mainz-lookup', help='default: %(default)s' )
    parser.add_argument('--susy-lookup', help='default: %(default)s')
    parser.add_argument('-t','--terse', action='store_true')
    parser.add_argument('--debug', action='store_true')

    args = parser.parse_args(remaining)

    if not args.used_samples: 
        sys.exit('used_samples is required')

    with open(args.used_samples) as inputs: 
        used_samples = [v.split('#')[0].strip() for v in inputs]
        used_samples = filter(None,used_samples)

    flags = '' if args.terse else 'v'
    if args.debug: 
        flags += 'b'

    all_cuts = run_cutflow(
        used_samples, 
        susy_lookup=args.susy_lookup, 
        mainz_lookup=args.mainz_lookup, 
        flags=flags, 
        data_location=args.data_location, 
        )

    

    for sample, cut_dict in all_cuts.iteritems(): 
        print sample
        sort_cuts = [(name, value) for name, value in cut_dict.iteritems()]
        sort_cuts = sorted(sort_cuts, key=lambda cut: cut[1], reverse=True)
        for name, value in sort_cuts: 
            print '{:>20}: {: >12.2f}'.format(name,value)



