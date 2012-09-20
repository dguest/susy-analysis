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
import multiprocessing
from itertools import izip_longest

def cutflow_job(ins): 
    """
    job wrapper to pass to multiprocessing map
    """
    samp, data_location, flags, mainz_cutflow, susy_cutflow = ins

    debug = 'b' in flags

    data_type = 'unknown'
    counts = {}
    if samp.startswith('Stop'): 
        data_type = 'signal'
        if debug: 
            data_type = 'debug'
            return '{} will go to signal'.format(samp), 'debug'
        matched_files = mainz_cutflow.add_ds_lookup(samp, data_location)
        signal_flags = flags + 's'
        if matched_files: 
            counts = mainz_cutflow.get_normed_counts(
                samp, flags=signal_flags)

    elif samp == 'Data': 
        data_type = 'data'
        if debug: 
            return '{} is Data'.format(samp), 'debug'
        warnings.warn('Data not implemented')

    else: 
        data_type = 'background'
        if debug: 
            return '{} will go to bg'.format(samp), 'debug'
        matched_files = susy_cutflow.add_ds_lookup(samp, data_location)
        if matched_files: 
            counts = susy_cutflow.get_normed_counts(
                samp, flags=flags)
    return counts, data_type



def run_cutflow(samples, susy_lookup, mainz_lookup='SampleListStop.txt',
                data_location='.', flags='v', cores=1,    
                counts_cache='raw_counts'): 



    if not os.path.isdir(counts_cache): 
        os.makedirs(counts_cache)
    
    mainz_cutflow = cutflow.NormedCutflow(mainz_lookup, 
                                          file_format='mainz', 
                                          raw_counts_cache=counts_cache)

    susy_cutflow = cutflow.NormedCutflow(susy_lookup, 
                                         file_format='official', 
                                         raw_counts_cache=counts_cache)

    sig_counts = {}
    bg_counts = {}


    pool = multiprocessing.Pool(n_cores)

    inputs = []
    for samp in samples: 
        ins = (samp, data_location, flags, mainz_cutflow, susy_cutflow)
        inputs.append(ins)

    if len(inputs) > 1: 
        all_counts = pool.map(cutflow_job, inputs)
    else: 
        all_counts = map(cutflow_job, inputs)

    for samp, (count, data_type) in zip(samples,all_counts): 
        if data_type == 'signal': 
            sig_counts[samp] = count
        elif data_type == 'background': 
            bg_counts[samp] = count
        else: 
            print count


    signal_cutflows = collections.defaultdict(list)
    cuts_names = []
    def check_cut(number, name): 
        if len(cuts_names) <= number: 
            cuts_names.append(name)
        elif cuts_names[number] != name: 
            raise ValueError(
                'cut number {}: {} got weird name ({})'.format(
                    number, cuts_names[number], name) )

    for sample_name, cuts in sig_counts.items(): 

        particle, vx = sample_name.split('_')
        
        chan_cutflow = signal_cutflows[particle]

        for number, (name, value) in enumerate(cuts): 
            if len(chan_cutflow) <= number: 
                chan_cutflow.append(value)
            else: 
                chan_cutflow[number] += value
            check_cut(number, name)

    np_finder = re.compile('Np[0-9]')
    bg_cutflows = collections.defaultdict(list)
    for sample_name, cuts in bg_counts.items(): 
        short_name = sample_name
        if np_finder.findall(sample_name): 
            short_name = np_finder.split(sample_name)[0]
            
        chan_cutflow = bg_cutflows[short_name]

        for number, (name, value) in enumerate(cuts): 
            if len(chan_cutflow) <= number: 
                chan_cutflow.append(value)
            else: 
                chan_cutflow[number] += value
            check_cut(number, name)


    signal_cutflows.update(bg_cutflows)
    signal_cutflows['names'] = cuts_names

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

    n_cores = multiprocessing.cpu_count()
    parser.add_argument('used_samples', nargs='?', 
                        help='default: %(default)s')
    parser.add_argument('--mainz-lookup', help='default: %(default)s' )
    parser.add_argument('--susy-lookup', help='default: %(default)s')
    parser.add_argument('-t','--terse', action='store_true')
    parser.add_argument('--debug', action='store_true')
    parser.add_argument('--multi', type=int, default=1, const=n_cores, 
                        nargs='?',
                        help='use multiple cores (or all if no arg given)')
    parser.add_argument('--aggressive', action='store_true', 
                        help='remove corrupted files and carry on')

    args = parser.parse_args(remaining)

    if not args.used_samples: 
        sys.exit('used_samples is required')

    with open(args.used_samples) as inputs: 
        used_samples = [v.split('#')[0].strip() for v in inputs]
        used_samples = filter(None,used_samples)
        
    flags = '' if args.terse or args.multi > 1 else 'v'
        
    if args.debug: 
        flags += 'b'
    if args.aggressive: 
        flags += 'a'

    all_cuts = run_cutflow(
        used_samples, 
        susy_lookup=args.susy_lookup, 
        mainz_lookup=args.mainz_lookup, 
        flags=flags, 
        data_location=args.data_location, 
        cores=args.multi, 
        )

    names = all_cuts['names']
    
    num_format = '{:.4g}'
    write_format = '{:>{w}.4g}'
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


