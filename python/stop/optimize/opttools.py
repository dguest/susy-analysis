from stop import hyperstack, hists
import h5py 
import numpy as np
from os.path import join, basename, splitext, isdir
import os, re
from stop.profile import cum_cuts
from os.path import isfile, basename, split, join
import cPickle, copy

import ConfigParser, argparse

def compute_significance(signal, background, kept_axes, sys_factor=0.3): 
    signal = copy.deepcopy(signal)
    background = copy.deepcopy(background)
    for axis in background.axes: 
        if not axis.name in kept_axes:
            signal.reduce(axis.name)
            background.reduce(axis.name)

    denom = ( signal + background + 
              (sys_factor * background)**2 )**0.5
    
    sig_hist = signal / denom
    return sig_hist


def build_hists(root_file, put_where='cache', cut='vxp_good'): 
    stem_name = basename(splitext(root_file)[0])
    out_file_name = join(put_where,'{}_hyper.h5'.format(stem_name))

    if not os.path.isdir(put_where): 
        os.mkdir(put_where)

    if os.path.isfile(out_file_name): 
        return out_file_name

    print 'making {}'.format(os.path.basename(out_file_name))

    the_cut = dict(cum_cuts())[cut]
    cuts = [(cut,the_cut)]
    out_file = hyperstack.hypersusy(root_file, mask_list=cuts, 
                                    output_file=out_file_name, 
                                    flags='vt')
    return out_file

def run_cutflow(root_file): 

    print 'running cutflow {}'.format(root_file)

    the_cut = dict(cum_cuts())['ctag_mainz']

    pass_number = hyperstack.cutflow(root_file, 
                                     mask=('dummy',the_cut), 
                                     flags='vt')
    return pass_number


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

def combine(h5_cache, meta, cut='vxp_good', signal='175-100'): 
    sum_background = None
    sum_signal = None

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


def get_baseline(cache, distillates, meta, sys_factor, lumi): 
    comparison_pkl = join(cache, 'comp_cutflow.pkl')
    if isfile(comparison_pkl): 
        with open(comparison_pkl) as pkl: 
            count_dict = cPickle.load(pkl)
    else: 
        raw_counts = map(run_cutflow, distillates)
        keys = [splitext(basename(rf))[0] for rf in distillates]
        keycount = zip(keys, raw_counts)
        def norm(key): 
            return meta[key].xsec_per_evt * lumi
        count_dict = {key: raw * norm(key) for key, raw in keycount}
        with open(comparison_pkl,'w') as pkl: 
            cPickle.dump(count_dict, pkl)



    total_signal = 0
    total_bg = 0
    for key, count in count_dict.iteritems(): 
        if 'directCC' in key: 
            assert total_signal == 0
            total_signal += count
        else: 
            total_bg += count

    baseline_signif = total_signal / (
        total_signal + total_bg + (sys_factor * total_bg)**2)**0.5
    return baseline_signif
