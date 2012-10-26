#!/usr/bin/env python2.7

import os, re, glob, cPickle, sys
from stop.hists import Hist1d, Stack
from stop.profile import build_profile
import ConfigParser, argparse
from hdroot import hd_from_root
import h5py
import numpy as np


def sample_name_from_file(file_name): 
    parts = os.path.splitext(os.path.basename(file_name))[0].split('_')
    if parts[-1] == 'pro2': 
        return '_'.join(parts[:-1])
    else: 
        return '_'.join(parts)

def print_all_hists(all_hists): 
    samplist, varlist, cutlist = zip(*all_hists.keys())
    samples = sorted(set(samplist))
    variables = sorted(set(varlist))
    cuts = set(filter(None,cutlist))

    # cut_bits = dict(cum_cuts())
    
    for cut in cuts: 
        # assert cut in cut_bits, cut
        for var in variables: 
            make_hist(all_hists, var, cut)

def fix_metpt_like(hist): 
    lab = hist.x_label
    triggers = [
        'met' in lab and not 'jetmet' in lab, 
        '_pt' in lab
        ]
    if any(triggers): 
        bin_sparse_vars(hist)
        mev_to_gev(hist)

def bin_sparse_vars(hist): 
    break_pts = range(50,70,2) + [80, 100]
    for low, high in zip(break_pts[:-1],break_pts[1:]): 
        hist.average_bins(np.arange(low,high))

def mev_to_gev(hist): 
    hist.extent = tuple(e / 1e3 for e in hist.extent)
    hist.x_label = '{} [GeV]'.format(hist.x_label)

def combine_backgrounds(hists): 

    # list is to preserve ordering
    combined_list = []
    combined = {}
    crap_finder = re.compile('Np[0-9]+')
    finders = [
        (r'$Z \to \nu \nu$ + jets', r'Znunu','orange'), 
        (r'$W \to l \nu$ + jets', r'W(tau|e|mu)nu','green'), 
        (r'$W \to q q$', r'W(c|b)','r'), 
        (r'$Z \to l l$ + jets', r'Z(tau|e|mu){2}','c'), 
        (r'$t\bar{t}$',         r'ttbar_','b'),
        ]
    
    for h in hists: 
        hist_name = h.title
        fix_metpt_like(h)
        combined_name = crap_finder.split(hist_name)[0]
        color = 'b'
        for rep, old, color_cand in finders: 
            reg = re.compile(old)
            if reg.findall(combined_name): 
                combined_name = rep
                color = color_cand
        if combined_name != hist_name: 
            h.title = combined_name
            if combined_name not in combined: 
                combined[combined_name] = h
                combined[combined_name].color = color
                combined_list.append(combined_name)
            else: 
                combined[combined_name] += h

    for name in combined_list: 
        yield combined[name]

def make_hist(all_hists, var, cut): 
    used_hists = {}
    for id_tuple, hist in all_hists.iteritems(): 
        if id_tuple[1] == var and id_tuple[2] == cut: 
            used_hists[id_tuple[0]] = hist

    hists = []
    for sample, (hist, extent) in used_hists.iteritems(): 
        hists.append(Hist1d(hist, extent, x_label=var, title=sample))
    # hists = sorted(hists)
    stack = Stack('stack')
    stack.ax.set_yscale('log')
    stack.y_min = 0.1
    signal_hists = [x for x in hists if str(x).startswith('Stop')]
    background_hists = [x for x in hists if not str(x).startswith('Stop')]
    combined_bkg = combine_backgrounds(background_hists)
    combined_bkg = sorted(combined_bkg)
    for sig in signal_hists: 
        fix_metpt_like(sig)

    stack.add_signals(signal_hists)
    stack.add_backgrounds(combined_bkg)
    stack.add_legend()
    if not os.path.isdir('plots'): 
        os.mkdir('plots')

    for ext in ['pdf','png']: 
        stack.save('plots/stack_{var}_cut_{cut}.{ext}'.format(
                var=var, cut=cut, ext=ext))

    stack.close()
    

def get_variable_and_cut(hist_name): 
    if not '_cut_' in hist_name: 
        return hist_name, None
    return hist_name.split('_cut_')

def select_signals(sample_list, signals = ['175-100']): 
    accepted = []
    for sample in sample_list: 
        if not os.path.basename(sample).startswith('Stop'): 
            accepted.append(sample)
        else: 
            for signal in signals: 
                if signal in sample: 
                    accepted.append(sample)
    return accepted

if __name__ == '__main__': 
    parser = argparse.ArgumentParser(
        description=__doc__, epilog='Author: Dan Guest <dguest@cern.ch>')
    parser.add_argument('config', nargs='?', help='default: %(default)s', 
                        default='draw.cfg')
    parser.add_argument('--fast', action='store_true')
    args = parser.parse_args(sys.argv[1:])
    config_parser = ConfigParser.SafeConfigParser()
    config_parser.read([args.config])
    file_config = dict(config_parser.items('files'))
    whisky_path = file_config['output_ntuples_location']
    distillates = glob.glob(os.path.join(whisky_path,'*.root'))
    if 'signals' in file_config: 
        distillates = select_signals(distillates, 
                                     file_config['signals'].split())
    else:
        print file_config

    norm_file = os.path.join(whisky_path, 'x_sec_per_event.pkl')
    with open(norm_file) as pkl: 
        x_sec_dict = cPickle.load(pkl)

    h5s = []
    for f in distillates: 
        profile_pth = build_profile(f)
        stem = os.path.splitext(profile_pth)[0]
        h5_name = '{}.h5'.format(stem)
        if not os.path.isfile(h5_name): 
            success, failure = hd_from_root(profile_pth)
            assert not failure, 'conversion fail {}'.format(failure)
        h5s.append(h5_name)

    all_hists = {}
    for histofile in h5s: 
        sample = sample_name_from_file(histofile)
        with h5py.File(histofile) as hist_file: 
            for hist in hist_file: 
                h5_array = hist_file[hist]
                extent = [h5_array.attrs[n] for n in ['xmin','xmax']]
                var, cut = get_variable_and_cut(hist)
                array = np.array(h5_array)
                scale_name = sample_name_from_file(histofile)
                array = array * x_sec_dict[scale_name]
                reduced = np.add.reduce(array, axis=1)
                short_var = var.replace('met_vs_','')
                all_hists[(sample,short_var,cut)] = (reduced, extent)

    if args.fast: 
        fast_hists = {}
        for (sample, short, cut), hist in all_hists.iteritems(): 
            if cut == 'met_120' and short == 'jet1_pt': 
                fast_hists[(sample,short,cut)] = hist
        all_hists = fast_hists

    print_all_hists(all_hists)

        
