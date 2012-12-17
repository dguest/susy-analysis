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

def get_significance(signal, background, sys_factor): 
    """
    this should be the _only_ way significance is calculated for the sake 
    of consistency 
    """
    denom = ( background + 
              (sys_factor * background)**2 )**0.5
    sig_hist = signal / denom
    return sig_hist
    

def compute_significance(signal, background, kept_axes, sys_factor=0.3): 
    signal = copy.deepcopy(signal)
    background = copy.deepcopy(background)
    for axis in background.axes: 
        if not axis.name in kept_axes:
            signal.reduce(axis.name)
            background.reduce(axis.name)

    return get_significance(signal, background, sys_factor)

class HistBuilder(object): 
    """
    builds multi-dimensional histograms from a distilled dataset. 
    """
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
            the_cut |= dict(bits.bits + bits.more_bits)[cut_to_add]

        cuts = [(full_cut_name, the_cut)]


        out_file = hyperstack.hypersusy(root_file, mask_list=cuts, 
                                        output_file=out_file_name, 
                                        flags=self.flags, 
                                        limit_dict=self.special_limits)
        return out_file

class Optimum(object): 
    """
    Class to store optimum cuts, signal significance, etc. for a sample.
    """
    cuts = None
    significance = None
    n_signal = None
    n_background = None
    def __init__(self, signal_hist, bg_hist, significance_hist=None,
                 sys_factor=0.3): 
        signal_hist.integrate()
        bg_hist.integrate()
        if significance_hist is None: 
            sig_hist = get_significance(signal_hist, bg_hist, sys_factor)
        else:
            sig_hist = significance_hist
        try: 
            max_index = np.unravel_index(sig_hist.array.argmax(), 
                                         sig_hist.array.shape)
            self.significance = sig_hist.array[max_index]
            self.n_signal = signal_hist.array[max_index]
            self.n_background = bg_hist.array[max_index]

            self.cuts = {}
            for axis, cut in zip(signal_hist.axes, max_index): 
                self.cuts[axis.name] = axis.get_bin_extent(cut)[0]

        except ValueError: 
            self.significance = float(sig_hist.array)
            self.n_signal = float(signal_hist.array)
            self.n_background = float(bg_hist.array)
            self.cuts = {}


class OptimaCache(dict): 
    """
    Abstraction layer for percistence of cutflow optima. 
    """
    def __init__(self, cache_name): 
        self._cache_name = cache_name
        if isfile(cache_name): 
            with open(cache_name) as cache: 
                cached_rep = cPickle.load(cache)
            super(OptimaCache,self).__init__(cached_rep)

    def __enter__(self):
        return self
    def __exit__(self, exc_type, exc_val, exc_tb): 
        if exc_type is None: 
            self.write()
    
    def write(self, cache_name=''): 
        if not cache_name: 
            cache_name = self._cache_name
        with open(cache_name,'w') as cache: 
            out_dict = dict(self)
            cPickle.dump(out_dict, cache)

def run_cutflow(root_file): 

    print 'running cutflow {}'.format(root_file)

    the_cut = dict(cum_cuts())['ctag_mainz']

    pass_number = hyperstack.cutflow(root_file, 
                                     mask=('dummy',the_cut), 
                                     flags='vt')
    return pass_number


def select_signals(sample_list, signals = ['175-100']): 
    return list(sig_itr(sample_list, signals))

class SignalKeyMapper(object): 
    signal_re = re.compile('directCC_([0-9]+)_([0-9]+)')
    def __init__(self, long_keys): 
        self._long_from_short = {}
        for key in long_keys: 
            matches = self.signal_re.search(key)
            if matches: 
                short_key = '{}-{}'.format(*matches.groups())
                self._long_from_short[short_key] = key
    def long_from_short(self, key): 
        return self._long_from_short[key]
    def short_from_long(self, key): 
        matches = self.signal_re.search(key)
        if matches: 
            return '{}-{}'.format(*matches.groups())
        else: 
            raise KeyError('could not map {} to a short signal name'.format(
                    key))
            

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

    try: 
        # TODO: use the class from above here
        signal_finder = re.compile(
            'directCC_{}_{}'.format(*signal.split('-')))
    except Exception: 
        print 'had an issue with {}'.format(signal)
        raise 

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

    baseline_signif = get_significance(total_signal, total_bg, sys_factor)
    return baseline_signif
