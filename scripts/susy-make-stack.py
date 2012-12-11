#!/usr/bin/env python2.7

import os, re, glob, cPickle, sys, cProfile
import collections, warnings
from stop.hists import Hist1d, Stack
from stop.profile import cum_cuts
from stop import hyperstack
from stop import bits
import ConfigParser, argparse
from hdroot import hd_from_root
import h5py
import numpy as np
from multiprocessing import Pool, cpu_count

_basic_cuts = [
    'n_events','GRL','EF_xe80_tclcw_loose','lar_error','core',
    'jet_clean', 'vxp_good', 'n_jet_geq_3','lepton_veto', 
    ]
_opt_cuts = [a for a, b in bits.more_bits]

GeV = 1e3

class DrawOpts(object): 
    rebin = None
    y_min = 1.0
    do_log = False
    cuts = {}

    def __init__(self,args={}, cuts={}): 
        if args:
            self.ext = args.ext
            self.do_log = args.log
            self.cuts = cuts


def build_nminus_hists(root_file, opt_cuts=_opt_cuts, 
                       basic_cuts=_basic_cuts, options=DrawOpts()): 
    base_mask = 0
    bdict = dict(bits.better_bits + bits.more_bits + bits.composite_bits)
    for cut in basic_cuts + opt_cuts: 
        base_mask |= bdict[cut]
        
    cuts = []
    for minus_cut in opt_cuts: 
        minus_mask = (base_mask & ~bdict[minus_cut])
        cuts.append( (minus_cut, minus_mask))
    cuts.append( ('all', base_mask) )

    none_mask = 0
    for cut in basic_cuts: 
        none_mask |= bdict[cut]
    cuts.append( ('base', none_mask))
    
    for name, mask in cuts: 
        stuffs = []
        for bname, bmask in bdict.iteritems(): 
            if bmask & mask: 
                stuffs.append(bname)
        assert name not in stuffs

    return build_hists(root_file, cuts=cuts, options=options)

def build_hists(root_file, put_where=None, cuts=cum_cuts(), 
                options=DrawOpts()): 
    if put_where is None: 
        put_where = options.cache
    out_file_name = '{}_hists.h5'.format(os.path.splitext(root_file)[0])
    out_file_name = os.path.join(put_where,os.path.basename(out_file_name))

    if not os.path.isdir(put_where): 
        os.mkdir(put_where)

    if os.path.isfile(out_file_name): 
        return out_file_name

    print 'making {}'.format(os.path.basename(out_file_name))

    out_file = hyperstack.stacksusy(root_file, mask_list=cuts, 
                                    output_file=out_file_name, 
                                    added_cuts=options.cuts, 
                                    flags='vt')
    return out_file



def sample_name_from_file(file_name): 
    parts = os.path.splitext(os.path.basename(file_name))[0].split('_')
    if parts[-1] == 'hists': 
        return '_'.join(parts[:-1])
    else: 
        return '_'.join(parts)

def print_all_hists(all_hists, save_dir, draw_opts=None): 
    varlist, cutlist = zip(*all_hists.keys())
    variables = sorted(set(varlist))
    cuts = set(filter(None,cutlist))

    for cut in cuts: 
        # assert cut in cut_bits, cut
        for var in variables: 
            print 'printing {} {}'.format(var, cut)
            make_hist(all_hists[(var,cut)], save_dir, draw_opts)

def fix_metpt_like(hist, draw_opts=None): 
    lab = hist.x_label
    lab_first = lab.split('_')[0]
    triggers = [
        lab_first == 'met', 
        '_pt' in lab, 
        lab_first == 'mttop', 
        lab_first == 'htx', 
        ]
    anti_triggers = [ 
        'truth_label' in lab, 
        lab == 'nGoodJets', 
        'leadingCJet' in lab, 
        ]
    if any(triggers): 
        bin_sparse_vars(hist, draw_opts.rebin)
        mev_to_gev(hist)
    elif draw_opts.rebin is not None and not any(anti_triggers): 
        hist.average_bins(draw_opts.rebin)
        hist.scale(draw_opts.rebin)

def bin_sparse_vars(hist, rebin=None): 
    rebin_begin = 1
    if rebin is None: 
        rebin = 2
        rebin_begin = 50
    else: 
        hist.scale(rebin)
    break_pts = range(rebin_begin,70,rebin) + [80, 100]
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

def combine_backgrounds(hists, draw_opts=None): 

    # list is to preserve ordering
    combined_list = []
    combined = {}
    crap_finder = re.compile('Np[0-9]+')
    rep_groups = {
        'Zlljets':'Zll', 
        'Wjets':'Wlnu'
        }
    finders = [
        (r'$Z \to \nu \nu$ + jets', 'Znunujets','orange'), 
        (r'$Z \to ll$ + jets',      'Zll','c'), 
        (r'$W \to l \nu$ + jets',   'Wlnu','red'), 
        (r'$W$ + jets',             'Wjets','green'), 
        (r'$t\bar{t}$',             'ttbar','b'),

        ('charm','charm','g'), 
        ('bottom','bottom','red'), 
        ('light','light','blue'), 
        ('tau','tau','pink'), 
        ('other','other','orange'), 
        
        ]
    
    for h in hists: 
        hist_name = h.title

        if h.group in rep_groups: 
            h.group = rep_groups[h.group]
        fix_metpt_like(h, draw_opts)
        combined_name = crap_finder.split(hist_name)[0]
        color = 'b'
        for rep, group, color_cand in finders: 
            if group in h.group: 
                combined_name = rep
                color = color_cand

        h.title = combined_name
        if combined_name not in combined: 
            combined[combined_name] = h
            combined[combined_name].color = color
            combined_list.append(combined_name)
        else: 
            combined[combined_name] += h

    for name in combined_list: 
        yield combined[name]

def make_hist(hists, save_dir='plots', draw_opts=None): 
    cut = hists[0].cut
    var = hists[0].x_label
    stack = Stack('stack')
    if draw_opts.do_log: 
        stack.ax.set_yscale('log')
        stack.y_min = draw_opts.y_min
    signal_hists = [x for x in hists if 'directCC' in x.title]
    background_hists = [x for x in hists if not 'directCC' in x.title]
    combined_bkg = combine_backgrounds(background_hists, draw_opts)
    combined_bkg = sorted(combined_bkg)
    for sig in signal_hists: 
        fix_metpt_like(sig, draw_opts)
        fix_signame(sig)

    stack.add_signals(signal_hists)
    stack.add_backgrounds(combined_bkg)
    stack.add_legend()
    if not os.path.isdir(save_dir): 
        os.mkdir(save_dir)


    stack.save('{save_dir}/stack_{var}_cut_{cut}{bonus}.{ext}'.format(
            save_dir=save_dir, var=var, cut=cut, 
            bonus='_log' if draw_opts.do_log else '', 
            ext=draw_opts.ext.lstrip('.')))

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
    """
    probably broken
    """
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


def remove_from_name(name, rm): 
    splname = name.split('_')
    splname.remove(rm)
    return '_'.join(splname)    

def hist_iter(f, parent='', truth=False): 
    for name, entry in f.iteritems(): 
        if type(entry) == h5py.Group: 
            if parent: 
                new_parent = '_'.join([parent, name])
            else: 
                new_parent = name
            for subname, cut, array in hist_iter(entry, parent=new_parent, 
                                                 truth=truth): 
                yield subname, cut, array
        else: 
            if truth: 
                if not 'tag' in entry.attrs: 
                    continue
            else: 
                if 'tag' in entry.attrs: 
                    continue
            yield parent, name, entry

def run_main(): 
    parser = argparse.ArgumentParser(
        description=__doc__, epilog='Author: Dan Guest <dguest@cern.ch>')
    parser.add_argument('config', nargs='?', help='default: %(default)s', 
                        default='draw.cfg')
    parser.add_argument('--fast', action='store_true')
    parser.add_argument('--multi', action='store_true')
    parser.add_argument(
        '--truth', nargs='?', 
        default=None, 
        const='backgrounds', 
        help='Group by truth rather than sample. '
        'Can also give <stop mass>-<lsp mass> pair')
    parser.add_argument('--save-dir', nargs='?', default='plots', 
                        help='default: %(default)s')
    parser.add_argument('--nminus','-m', action='store_true', 
                        help='make n-1 histograms')
    parser.add_argument('--log','-l', action='store_true')
    parser.add_argument(
        '--ext', choices=['.png','.pdf'], default='.pdf', 
        help='produce this type of file (default %(default)s)')
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

    meta_file = os.path.join(whisky_path, 'meta.pkl')
    with open(meta_file) as pkl: 
        meta_dict = cPickle.load(pkl)

    lumi_fb = 1.0
    if config_parser.has_option('physics','total_lumi_fb'): 
        lumi_fb = config_parser.getfloat('physics','total_lumi_fb')

    draw_opts = DrawOpts(args)

    if config_parser.has_section('cuts'): 
        plot_opts = dict(config_parser.items('cuts'))
        nminus_cuts = plot_opts['nminus_cuts'].split()
        basic_cuts = plot_opts['basic_cuts'].split()

        draw_opts.cuts = dict(
            leading_jet = float(plot_opts['leading_jet_cut_gev'])*GeV, 
            met = float(plot_opts['met_cut_gev'])*GeV, 
            mttop = float(plot_opts['mttop_cut_gev'])*GeV, 
            j2_anti_b = float(plot_opts['j2_anti_b_cut']), 
            j2_anti_u = float(plot_opts['j2_anti_u_cut']), 
            j3_anti_b = float(plot_opts['j3_anti_b_cut']), 
            j3_anti_u = float(plot_opts['j3_anti_u_cut']), 
            )

        print 'pushing the pony button'
        def the_pony_button(in_file): 
            """
            this here be broken in the multiprocessing
            """
            return build_nminus_hists(in_file, opt_cuts=nminus_cuts, 
                                      basic_cuts=basic_cuts, 
                                      options=draw_opts)
        mappable_nminus = the_pony_button
    else: 
        mappable_nminus = build_nminus_hists

    hist_builder = build_hists
    if args.nminus: 
        hist_builder = mappable_nminus

    if config_parser.has_section('draw'): 
        draw_opts.rebin = config_parser.getint('draw','rebin')
        draw_opts.y_min = config_parser.getfloat('draw','y_min')
        try:
            draw_opts.cache = config_parser.get('draw', 'cache')
        except ConfigParser.NoOptionError: 
            draw_opts.cache = 'cache'

    if args.multi:
        pool = Pool(cpu_count())
        h5s = pool.map(hist_builder, distillates)
    else: 
        h5s = map(hist_builder, distillates)

    do_truth = bool(args.truth)
    if do_truth and args.truth != 'backgrounds': 
        stop_mass, lsp_mass = args.truth.split('-')

    used_xsecs = collections.defaultdict(lambda: 0)
    hists_by_group = collections.defaultdict(list)
    for number, histofile in enumerate(h5s): 
        sys.stdout.write(
            '\rloading files ({} of {})'.format(number + 1, len(h5s)))
        sys.stdout.flush()
        sample = sample_name_from_file(histofile)
        if do_truth: 
            if args.truth == 'backgrounds': 
                if 'directCC' in sample: 
                    continue
            else: 
                if not 'directCC_{}_{}'.format(stop_mass, lsp_mass) in sample:
                    continue

        if not sample in meta_dict: 
            warnings.warn(
                '{} is missing from scal file, skipping'.format(sample))
            continue
        with h5py.File(histofile) as hist_file: 
            for short_var, cut, h5_array in hist_iter(hist_file, 
                                                      truth=do_truth): 
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
                scale_factor = meta_dict[sample].xsec_per_evt
                array = array * scale_factor * lumi_fb * 1e3
                total_area = array.sum()
                used_xsecs[sample] = max(used_xsecs[sample],
                                             total_area)

                if not do_truth:
                    group = grouping[sample]
                else: 
                    group = h5_array.attrs['tag']
                    short_var = remove_from_name(short_var,group)
                    
                hist = Hist1d(array, extent=extent, x_label=short_var, 
                              title=sample, group=group)
                hist.cut = cut
                hist.y_label = (
                    r'$n$ events (${}\,\mathrm{{fb}}^{{-1}}$)'.format(
                        lumi_fb))
                if do_truth and args.truth != 'backgrounds': 
                    hist.title='garbage'
                hists_by_group[(short_var, cut)].append(hist)

    # write_xsec_corrections(used_xsecs)
    

    print_all_hists(hists_by_group, save_dir=args.save_dir, 
                    draw_opts=draw_opts)

        
if __name__ == '__main__': 
    run_main()
    # cProfile.run('run_main()','profile.txt')
