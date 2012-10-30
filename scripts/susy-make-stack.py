#!/usr/bin/env python2.7

import os, re, glob, cPickle, sys, cProfile
import collections, warnings
from stop.hists import Hist1d, Stack
from stop.profile import build_profile, cum_cuts
from stop import hyperstack
import ConfigParser, argparse
from hdroot import hd_from_root
import h5py
import numpy as np

def build_hists(root_file, put_where='cache'): 
    out_file_name = '{}_hists.h5'.format(os.path.splitext(root_file)[0])
    out_file_name = os.path.join(put_where,os.path.basename(out_file_name))

    if not os.path.isdir(put_where): 
        os.mkdir(put_where)

    if os.path.isfile(out_file_name): 
        return out_file_name

    print 'making {}'.format(os.path.basename(out_file_name))

    cuts = cum_cuts()
    out_file = hyperstack.stacksusy(root_file, mask_list=cuts, 
                                    output_file=out_file_name, 
                                    flags='v')
    return out_file

def sample_name_from_file(file_name): 
    parts = os.path.splitext(os.path.basename(file_name))[0].split('_')
    if parts[-1] == 'hists': 
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

def fix_signame(hist): 
    finder = re.compile('directCC_([0-9]+)_([0-9]+)')
    found = finder.search(hist.title)
    if found: 
        newname = 'Stop-{}-{}'.format(found.group(1), found.group(2))
        hist.title = newname

def combine_backgrounds(hists): 

    # list is to preserve ordering
    combined_list = []
    combined = {}
    crap_finder = re.compile('Np[0-9]+')
    rep_groups = {
        'Zlljets':'Zll', 
        }
    finders = [
        (r'$Z \to \nu \nu$ + jets', 'Znunujets','orange'), 
        (r'$Z \to ll$ + jets',      'Zll','c'), 
        (r'$W \to l \nu$',          'Wlnu','green'), 
        (r'$W$ + jets',             'Wjets','r'), 
        (r'$t\bar{t}$',             'ttbar','b'),
        ]
    
    for h in hists: 
        hist_name = h.title

        if h.group in rep_groups: 
            h.group = rep_groups[h.group]
        fix_metpt_like(h)
        combined_name = crap_finder.split(hist_name)[0]
        color = 'b'
        for rep, group, color_cand in finders: 
            if group in h.group: 
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
        else: 
            warnings.warn("can't fit in {}".format(hist_name))

    for name in combined_list: 
        yield combined[name]

def make_hist(all_hists, var, cut): 
    used_hists = {}
    for id_tuple, hist in all_hists.iteritems(): 
        if id_tuple[1] == var and id_tuple[2] == cut: 
            used_hists[id_tuple[0]] = hist

    hists = []
    for sample, hist in used_hists.iteritems(): 
        hists.append(hist)
    # hists = sorted(hists)
    stack = Stack('stack')
    stack.ax.set_yscale('log')
    stack.y_min = 1.0
    signal_hists = [x for x in hists if 'directCC' in x.title]
    background_hists = [x for x in hists if not 'directCC' in x.title]
    combined_bkg = combine_backgrounds(background_hists)
    combined_bkg = sorted(combined_bkg)
    for sig in signal_hists: 
        fix_metpt_like(sig)
        fix_signame(sig)

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
    signals = [s.replace('-','_') for s in signals]
    for sample in sample_list: 
        if not 'directCC' in os.path.basename(sample):
            accepted.append(sample)
        else: 
            for signal in signals: 
                if signal in sample: 
                    accepted.append(sample)
    return accepted

def write_xsec_corrections(used_xsecs): 
    used = []
    for name, factor in used_xsecs.items(): 
        used.append((factor, name))

    used = sorted(used)

    with open('xsec_used.txt','w') as xsec_used: 
        for scale, name in used: 
            record = '{:<30} {}\n'.format(name, scale)
            xsec_used.write(record)

def fitr(f): 
    for l in f: 
        if not l.strip().startswith('#'): 
            yield l.strip()

def hist_itr(f): 
    """
    Traverses an HDF5 file, yields (var, cut, array) tuples
    """
    for name, entry in f.iteritems(): 
        if name.startswith('jet'): 
            for var, cut, array in hist_itr(entry): 
                yield '{}_{}'.format(name,var), cut, array

        else: 
            for cut, array in entry.iteritems(): 
                yield name, cut, array


def run_main(): 
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
    if 'exclude_pattern' in file_config: 
        comp_pattern = re.compile(file_config['exclude_pattern'])
        def ds_filt(ds): 
            return not comp_pattern.search(ds)
        distillates = filter(ds_filt,distillates)

    with open('files_used.txt','w') as files_used: 
        for f in distillates:
            base = os.path.basename(f)
            files_used.write('{}\n'.format(base))

    with open(file_config['grouping_file']) as grp: 
        grouping = dict(f.split() for f in fitr(grp))

    norm_file = os.path.join(whisky_path, 'x_sec_per_event.pkl')
    with open(norm_file) as pkl: 
        x_sec_dict = cPickle.load(pkl)

    lumi = 1.0
    if config_parser.has_option('physics','total_lumi'): 
        lumi = config_parser.getfloat('physics','total_lumi')

    h5s = []
    for f in distillates: 
        h5_name = build_hists(f)
        h5s.append(h5_name)

    all_hists = {}
    used_xsecs = collections.defaultdict(lambda: 0)
    for histofile in h5s: 
        sample = sample_name_from_file(histofile)
        if not sample in x_sec_dict: 
            warnings.warn(
                '{} is missing from scal file, skipping'.format(sample))
            continue
        with h5py.File(histofile) as hist_file: 
            for short_var, cut, h5_array in hist_itr(hist_file): 
                extent = [h5_array.attrs[n] for n in ['x_min','x_max']]
                if args.fast: 
                    requirements = [
                        cut == 'met_120', 
                        short_var == 'jet1_pt', 
                        ]
                    if not all(requirements): 
                        continue
                    print 'found fast hist in {}'.format(histofile)
                array = np.array(h5_array)
                scale_factor = x_sec_dict[sample]
                array = array * scale_factor * lumi
                total_area = array.sum() * scale_factor * lumi
                used_xsecs[sample] = max(used_xsecs[sample],
                                             total_area)
                group = grouping[sample]
                hist = Hist1d(array, extent=extent, x_label=short_var, 
                              title=sample, group=group)
                all_hists[(sample,short_var,cut)] = hist

    write_xsec_corrections(used_xsecs)

    print_all_hists(all_hists)

        
if __name__ == '__main__': 
    run_main()
    # cProfile.run('run_main()','profile.txt')
