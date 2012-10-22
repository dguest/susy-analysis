#!/usr/bin/env python2.7
"""
susy stop cutflow script. 

Defaults are set (by default) in cutflow.cfg.

Output is saved in the output-pickle, and can be printed with
susy-print-cutflow.py. 
"""

from susy import cutflow, normed
import sys, os, re
import argparse, ConfigParser
import warnings
import collections
import multiprocessing
import cPickle

def cutflow_job(ins): 
    """
    job wrapper to pass to multiprocessing map
    """
    samp, data_paths, flags, mainz_cutflow = ins

    data_location = data_paths['input']

    debug = 'b' in flags

    data_type = 'background'
    counts = None

    more_flags = flags
    if samp.startswith('Stop-'): 
        data_type = 'signal'
        more_flags += 's'

    matched_files = mainz_cutflow.add_ds_lookup(samp, data_location)

    if matched_files: 
        counts = mainz_cutflow.get_normed_counts(samp, flags=more_flags)

    elif samp == 'Data': 
        warnings.warn('Data not implemented')
    else: 
        warnings.warn("couldn't find any matches for {}".format(samp))

    return counts, data_type

def run_cutflow(samples, data_paths, susy_lookup, 
                mainz_lookup='SampleListStop.txt',
                flags='v', cores=1,    
                counts_cache='raw_counts'): 

    if not os.path.isdir(counts_cache): 
        os.makedirs(counts_cache)
    
    output = ''
    if 'output' in data_paths: 
        output = data_paths['output']

    mainz_cutflow = normed.NormedCutflow(
        mainz_lookup, 
        file_format='mainz', 
        raw_counts_cache=counts_cache, 
        output_ntuples_dir=output)

    # susy_cutflow = cutflow.NormedCutflow(
    #     susy_lookup, 
    #     file_format='official', 
    #     raw_counts_cache=counts_cache, 
    #     output_ntuples_dir=output)

    sig_counts = {}
    bg_counts = {}


    pool = multiprocessing.Pool(n_cores)

    inputs = []
    for samp in samples: 
        ins = (samp, data_paths, flags, mainz_cutflow)
        inputs.append(ins)

    if len(inputs) > 1 and cores > 1 and 'b' not in flags: 
        all_counts = pool.map(cutflow_job, inputs)
    else: 
        all_counts = map(cutflow_job, inputs)

    for samp, (count, data_type) in zip(samples,all_counts): 
        if not count: 
            print '{} not found'.format(samp)
            continue
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

        particle = sample_name
        
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

    defaults = { 
        'cache': 'cache', 
        'output_pickle': 'final_cuts.pkl', 
        }

    if os.path.isfile(args.config_file): 
        config_parser = ConfigParser.SafeConfigParser()
        config_parser.read([args.config_file])
        defaults.update(dict(config_parser.items('files')))
        
        parser.set_defaults(**defaults)

    n_cores = multiprocessing.cpu_count()
    parser.add_argument('used_samples', nargs='?', 
                        help='default: %(default)s')
    parser.add_argument('--mainz-lookup', help='default: %(default)s' )
    parser.add_argument('--susy-lookup', help='default: %(default)s')
    parser.add_argument('--rootcoredir', help='default: %(default)s')
    parser.add_argument('--cache', help='default: %(default)s')
    parser.add_argument('-t','--terse', action='store_true')
    parser.add_argument(
        '--debug', action='store_true', 
        help='may not do anything right now'
        )
    parser.add_argument('--multi', type=int, default=1, const=n_cores, 
                        nargs='?',
                        help='use multiple cores (or all if no arg given)')
    parser.add_argument('--aggressive', action='store_true', 
                        help='remove corrupted files and carry on')
    parser.add_argument(
        '--output-pickle', 
        help='pickle where cutflow is saved (default: %(default)s)')

    args = parser.parse_args(remaining)

    if not args.used_samples: 
        sys.exit('used_samples is required')

    with open(args.used_samples) as inputs: 
        used_samples = [v.split('#')[0].strip() for v in inputs]
        used_samples = filter(None,used_samples)

    flags = 'f'                 #FIXME: should not assume atlfast

    flags += '' if args.terse or args.multi > 1 else 'v'
        
    if args.debug: 
        flags += 'd'
    if args.aggressive: 
        flags += 'a'

    data_paths = {'input':args.data_location}

    if 'output_ntuples_location' in args: 
        data_paths['output'] = args.output_ntuples_location

    # allow local override of ROOTCOREDIR
    old_rootcoredir = ''
    if args.rootcoredir: 
        if 'ROOTCOREDIR' in os.environ: 
            old_rcd = os.environ['ROOTCOREDIR']
        os.environ['ROOTCOREDIR'] = args.rootcoredir
    try: 
        all_cuts = run_cutflow(
            used_samples, 
            susy_lookup=args.susy_lookup, 
            mainz_lookup=args.mainz_lookup, 
            flags=flags, 
            data_paths=data_paths, 
            cores=args.multi, 
            counts_cache=args.cache
            )
    finally: 
        if old_rootcoredir: 
            os.environ['ROOTCOREDIR'] = old_rootcoredir

    with open(args.output_pickle,'wb') as out_pkl: 
        cPickle.dump(all_cuts, out_pkl)

    


