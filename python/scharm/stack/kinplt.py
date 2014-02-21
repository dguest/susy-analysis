from scharm.stack.aggregator import HistDict
from scharm.stack.draw import h2_from_hn
from collections import defaultdict
from os.path import isfile, join, isdir
import os

def run_kinematic_plot(args): 
    """
    Plots S, B, S/B for values of kinematic cuts in the plane. 
    """
    from stop.runtypes import marks_types
    bg_set = set(marks_types)
    phys_set = set()
    if args.background or args.s_over_b or args.all: 
        phys_set |= bg_set

    sp_list = filter(None, [args.s_over_b, args.signal_point, args.all])
    if sp_list: 
        signal_point = sp_list[0]
        phys_set.add(signal_point)
    else: 
        signal_point = ''

    region_set = None
    if args.filt: 
        region_set = { args.filt.replace('/','-') } # TODO: make less hackish
    hists = HistDict(args.aggregate, 
                     filt='kinematics', 
                     physics_set=phys_set, 
                     cut_set=region_set, 
                     )
    hists_by_region = defaultdict(dict)
    for (phys, var, region), hist in hists.iteritems(): 
        hists_by_region[region][phys,var] = hist
    out_args = dict(odir=args.output_dir, ext=args.ext, 
                    sys_factor=args.sys_factor)
    if args.fix_z_range: 
        out_args['z_range'] = True
    preselection_hists = _get_preselection(hists_by_region)
    for region, reg_hists in hists_by_region.iteritems(): 
        _plot_region_kinematics(
            region, reg_hists, signal_point, bg_set, **out_args)
        if args.all: 
            print 'plotting bg'
            _plot_region_kinematics(
                region, reg_hists, '', bg_set, **out_args)
            print 'plotting signal'
            _plot_region_kinematics(
                region, reg_hists, signal_point, set(), **out_args)
        if preselection_hists: 
            no_sysfactor = {
                a:v for a, v in out_args.iteritems() if a != 'sys_factor'}
            _plot_preselection(
                region, reg_hists, preselection_hists, 
                signal_point, **no_sysfactor)

def _get_preselection(hists_by_region): 
    preselection_reg = [r for r in hists_by_region if 'preselection' in r]
    if preselection_reg and len(preselection_reg) != 1: 
        raise OSError('multiple preselection regions found: {}'.format(
                ', '.join(preselection_reg)))
    if preselection_reg: 
        return hists_by_region[preselection_reg[0]]
    return None
            
def _plot_preselection(region, hists, presel, signal_point, odir, ext): 
    sig_hist, bg_hist = _get_hists(hists, signal_point)
    presel_hist, phoney = _get_hists(presel, signal_point)
    def integrate(hist): 
        int_hist = hist['met'].integrate(reverse=True)
        return int_hist['leadingJetPt'].integrate(reverse=True)
    num = integrate(sig_hist)
    denom = integrate(presel_hist)

    the_plot = h2_from_hn(num / denom)
    the_plot.cb_label = '{} {} / preselection'.format(signal_point, region)
    if not isdir(odir): 
        os.mkdir(odir)
    the_plot.save('{}/{}-{}-over-preselection{}'.format(
            odir, signal_point, region,ext) )

                               
def _get_hists(hists, signal_point, bg_set=set()): 
    bg_hist = None
    sig_hist = None
    for (phys, var), hist in hists.iteritems(): 
        if phys in bg_set: 
            if not bg_hist: 
                bg_hist = hist
            else: 
                bg_hist += hist
        elif phys == signal_point: 
            if sig_hist: 
                phy_keys = ', '.join([k[0] for k in hists])
                raise StandardError(
                    'tried to replace {} with {}, keys: {}'.format(
                        signal_point, phys, phy_keys))
            sig_hist = hist
    return sig_hist, bg_hist

def _integrate(hist, reverse=True): 
    int_hist = hist['met'].integrate(reverse=reverse)
    return int_hist['leadingJetPt'].integrate(reverse=reverse)

def _plot_region_kinematics(region, hists, signal_point, bg_set, 
                            odir='plots', ext='.pdf', sys_factor=0.0, 
                            z_range=False): 

    sig_hist, bg_hist = _get_hists(hists, signal_point, bg_set)

    if bg_hist and sig_hist: 
        num = _integrate(sig_hist)
        denom = _integrate(bg_hist)
        the_hist = num / denom
        meth = '{}-over-bg'.format(signal_point)
        save_args = dict(log=False)
        if z_range: 
            save_args['vrange'] = (0.0, 0.3)
        cb_label = '{} / BG'.format(signal_point)
    elif sig_hist and not bg_hist: 
        the_hist = _integrate(sig_hist)
        meth = signal_point
        save_args = dict(log=True)
        cb_label = signal_point
    elif bg_hist and not sig_hist: 
        the_hist = _integrate(bg_hist)
        meth = 'bg'
        save_args = dict(log=True)
        cb_label = 'Background'
    the_plot = h2_from_hn(the_hist)
    the_plot.cb_label = cb_label

    if not isdir(odir): 
        os.mkdir(odir)
        
    the_plot.save('{}/{}-{}{}'.format(odir, meth, region,ext), **save_args)

    if bg_hist and sig_hist: 
        sosb_plot = _get_s_over_sqrt_b(sig_hist, bg_hist, sys_factor, 
                                       signal_point)
        if z_range: 
            save_args['vrange'] = (0.0, 12.0)
        out_tmp = '{}/{}-over-sqrt-b-{}{}'
        if sys_factor: 
            out_tmp = '{}/{}-over-sqrt-b-{}-sys-{s}{}'
        sosb_plot.save(
            out_tmp.format(odir, signal_point,region,ext, 
                           s=int(sys_factor*100)), 
            **save_args)

def _get_s_over_sqrt_b(sig_hist, bg_hist, sys_factor, signal_point): 
    s = _integrate(sig_hist)
    b = _integrate(bg_hist)
    s_over_sqrt_b =  s /(s + b + (sys_factor * b)**2 )**0.5
    sosb_plot = h2_from_hn(s_over_sqrt_b)
    cb_tmp = '{} $s / \sqrt{{s + b}}$'
    if sys_factor: 
        cb_tmp = '{} $s / \sqrt{{s + b + ({f:.2}b)^2}}$'
    sosb_plot.cb_label = cb_tmp.format(signal_point, f=sys_factor)
    return sosb_plot
