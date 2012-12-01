from stop import hyperstack, hists
import h5py 
import numpy as np
from os.path import join, basename, splitext, isdir
import os, re
from stop.profile import cum_cuts
from stop import bits
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

class HistBuilder(object): 
    def __init__(self, put_where='cache', base_cut='vxp_good', 
                 more_cuts=['lepton_veto']): 
        self._cache = put_where
        self._base_cut = base_cut
        self._more_cuts = more_cuts

        self.flags = 'vt'

        GeV = 1e3
        self.special_limits = { 
            'min_pt': 100*GeV, 
            'max_pt': 300*GeV, 
            }

    def build(self, root_file): 
        put_where = self._cache
        base_cut = self._base_cut
        more_cuts = self._more_cuts
        stem_name = basename(splitext(root_file)[0])
        out_file_name = join(put_where,'{}_hyper.h5'.format(stem_name))

        if not os.path.isdir(put_where): 
            os.mkdir(put_where)

        if os.path.isfile(out_file_name): 
            return out_file_name

        print 'making {}'.format(os.path.basename(out_file_name))

        the_cut = dict(cum_cuts())[base_cut]
        full_cut_name = '+'.join([base_cut] + more_cuts)
        for cut_to_add in more_cuts: 
            the_cut |= dict(bits.bits)[cut_to_add]

        cuts = [(full_cut_name, the_cut)]


        out_file = hyperstack.hypersusy(root_file, mask_list=cuts, 
                                        output_file=out_file_name, 
                                        flags=self.flags, 
                                        limit_dict=self.special_limits)
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
    """
    file combiner, badly needs a rewrite (shouldn't be doing filtering)
    """
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


def get_baseline(comparison_pkl, 
                 distillates, meta, sys_factor, lumi, signal): 

    if isfile(comparison_pkl): 
        with open(comparison_pkl) as pkl: 
            count_dict = cPickle.load(pkl)
    else: 
        count_dict = {}

    def k_from_r(f_name): 
        return splitext(basename(f_name))[0]

    dist_dict = {k_from_r(rf):rf for rf in distillates}

    missing_keys = set(dist_dict.keys()) - set(count_dict.keys())

    for key in missing_keys:
        missing_file = dist_dict[key]
        raw = run_cutflow(missing_file)
        count_dict[key] = raw * meta[key].xsec_per_evt * lumi

    if missing_keys: 
        with open(comparison_pkl,'w') as pkl: 
            cPickle.dump(count_dict, pkl)


    total_signal = 0
    total_bg = 0
    for key, count in count_dict.iteritems(): 
        if signal.replace('-','_') in key: 
            assert total_signal == 0
            total_signal += count
        else: 
            total_bg += count

    baseline_signif = total_signal / (
        total_signal + total_bg + (sys_factor * total_bg)**2)**0.5
    return baseline_signif
