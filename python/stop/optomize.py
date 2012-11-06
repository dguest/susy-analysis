from stop import hyperstack, hists
import h5py 
import numpy as np
from os.path import join, basename, splitext, isdir
import os, re
from stop.profile import cum_cuts

import ConfigParser, argparse

def build_hists(root_file, put_where='cache'): 
    stem_name = basename(splitext(root_file)[0])
    out_file_name = join(put_where,'{}_hyper.h5'.format(stem_name))

    if not os.path.isdir(put_where): 
        os.mkdir(put_where)

    if os.path.isfile(out_file_name): 
        return out_file_name

    print 'making {}'.format(os.path.basename(out_file_name))

    the_cut = dict(cum_cuts())['vxp_good']
    cuts = [('vxp_good',the_cut)]
    out_file = hyperstack.hypersusy(root_file, mask_list=cuts, 
                                    output_file=out_file_name, 
                                    flags='vt')
    return out_file


def select_signals(sample_list, signals = ['175-100']): 
    return list(sig_itr(sample_list, signals))

def sig_itr(sample_list, signals): 
    signals = [s.replace('-','_') for s in signals]
    for sample in sample_list: 
        if not 'directCC' in basename(sample):
            yield sample
        else: 
            for signal in signals: 
                if signal in sample: 
                    yield sample

def combine(h5_cache, meta): 
    sum_background = None
    sum_signal = None
    cut = 'vxp_good'
    signal = '175-100'
    signal_finder = re.compile('directCC_{}_{}'.format(*signal.split('-')))

    for hyper in  h5_cache: 
        key = splitext(basename(hyper))[0].replace('_hyper','')
        print 'loading {}'.format(hyper)

        with h5py.File(hyper) as hd_file: 
            hist = hists.HistNd(hd_file[cut])

        hist *= meta[key].xsec_per_evt
            
        if signal_finder.search(key): 
            if sum_signal: 
                raise ValueError('tried to redefine signal')
            else: 
                sum_signal = hist

        elif 'directCC' not in key:
            if not sum_background: 
                sum_background = hist
            else: 
                sum_background += hist

    return sum_signal, sum_background
