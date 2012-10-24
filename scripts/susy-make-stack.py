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

def combine_backgrounds(hists): 

    # list is to preserve ordering
    combined_list = []
    combined = {}
    crap_finder = re.compile('Np[0-9]+')
    for h in hists: 
        hist_name = str(h)
        combined_name = crap_finder.split(hist_name)[0]
        if combined_name != hist_name: 
            if combined_name not in combined: 
                combined[combined_name] = h
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
    hists = sorted(hists)
    stack = Stack('stack')
    stack.ax.set_yscale('log')
    stack.y_min = 1
    signal_hists = [x for x in hists if str(x).startswith('Stop')]
    background_hists = [x for x in hists if not str(x).startswith('Stop')]
    combined_bkg = combine_backgrounds(background_hists)
    stack.add_signals(signal_hists)
    stack.add_backgrounds(combined_bkg)
    stack.add_legend()
    if not os.path.isdir('plots'): 
        os.mkdir('plots')
    stack.save('plots/stack_{var}_{cut}.pdf'.format(var=var, cut=cut))
    

def get_variable_and_cut(hist_name): 
    if not '_cut_' in hist_name: 
        return hist_name, None
    return hist_name.split('_cut_')


if __name__ == '__main__': 
    parser = argparse.ArgumentParser(
        description=__doc__, epilog='Author: Dan Guest <dguest@cern.ch>')
    parser.add_argument('config', nargs='?', help='default: %(default)s', 
                        default='cutflow.cfg')
    args = parser.parse_args(sys.argv[1:])
    config_parser = ConfigParser.SafeConfigParser()
    config_parser.read([args.config])
    file_config = dict(config_parser.items('files'))
    whisky_path = file_config['output_ntuples_location']
    distillates = glob.glob(os.path.join(whisky_path,'*.root'))

    norm_file = os.path.join(whisky_path, 'x_sec_per_event.pkl')
    with open(norm_file) as pkl: 
        x_sec_dict = cPickle.load(pkl)

    h5s = []
    for f in distillates: 
        profile_pth = build_profile(f)
        stem = os.path.splitext(profile_pth)[0]
        success, failure = hd_from_root(profile_pth)
        assert not failure, 'conversion fail {}'.format(failure)
        h5_name = '{}.h5'.format(stem)
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

    print_all_hists(all_hists)

        
