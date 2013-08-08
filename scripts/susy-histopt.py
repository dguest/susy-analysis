#!/usr/bin/env python2.7
import itertools, tempfile
from stop.bullshit import OutputFilter
import os, sys, re
import argparse
from pyroot.fitter import CountDict, Workspace, sr_path


inf = float('inf')

    
def run(): 
    parser = argparse.ArgumentParser()
    subparsers = parser.add_subparsers(dest='which')

    ws = subparsers.add_parser('ws')
    ws.add_argument('kinematic_stat_dir')

    fit = subparsers.add_parser('fit')
    fit.add_argument('workspace')
    fit.add_argument('-l','--upper-limit',action='store_true')
    fit.add_argument('-p','--p-value', action='store_true')
    fit.add_argument('-d','--draw-plots', action='store_true')

    pval = subparsers.add_parser('pval')
    pval.add_argument('workspace')
    pval.add_argument(
        '-n','--n-toys', type=int, default=None, const=300, nargs='?',
        help='defaults to asymptotic, %(const)s with no args')

    upper_lim = subparsers.add_parser('ul')
    upper_lim.add_argument('workspace')

    multispace = subparsers.add_parser('ms')
    multispace.add_argument('kinematic_stat_dir')
    
    config = parser.parse_args(sys.argv[1:])
    opts = {'ws':_setup_workspace, 'fit':_new_histfit, 'pval':_get_p_value,
            'ul':_get_upper_limit, 'ms':_multispaces}
    opts[config.which](config)


def _new_histfit(config): 
    import ROOT
    from pyroot import utils
    utils.load_susyfit()

    from ROOT import ConfigMgr
    from ROOT import Util
    mgr = ConfigMgr.getInstance()
    mgr.initialize()
    mgr.setNToys(500)
    fit_config = mgr.addFitConfig('testFitConfig')
    fit_config.m_inputWorkspaceFileName = config.workspace
    fit_config.lumi = 21.0      # FIXME
    for bg_chan in ['ttbar0','Wenu0','Wmunu0','Znunu0']: 
        fit_config.m_bkgConstrainChannels.push_back(bg_chan)
    for sig_chan in ['SR0']:
        fit_config.m_signalChannels.push_back(sig_chan)

    draw_opts = {}
    if config.draw_plots: 
        draw_opts = dict(draw_before=True, 
                         draw_after=True, 
                         plot_corr_matrix=True)

    _fit_and_plot(fit_config.m_name, **draw_opts)
    
    if draw_opts: 
        raw_input('press ENTER')

    mgr.m_outputFileName = 'upper-lim.root'
    if config.upper_limit: 
        mgr.doUpperLimitAll()

def _get_p_value(config): 
    import ROOT
    from pyroot import utils
    utils.load_susyfit()
    from ROOT import Util
    from ROOT import RooStats
    workspace = Util.GetWorkspaceFromFile(config.workspace, 'combined')

    Util.SetInterpolationCode(workspace,4)
    if config.n_toys is None: 
        calc_type = 2           # asymtotic
        n_toys = 1
    else:
        calc_type = 0
        n_toys = config.n_toys
    with OutputFilter(): 
        p_val = RooStats.get_Pvalue(
            workspace,
            True,    # doUL (exclusion rather than discovery)
            n_toys, 
            calc_type)  
    print '{} -- {} -- {}'.format(
        p_val.GetCLsd1S(), p_val.GetCLsexp(), p_val.GetCLsu1S())

def _get_upper_limit(config): 
    import ROOT
    from pyroot import utils
    utils.load_susyfit()
    from ROOT import Util
    from ROOT import RooStats
    workspace = Util.GetWorkspaceFromFile(config.workspace, 'combined')

    Util.SetInterpolationCode(workspace,4)
    with OutputFilter(): 
        inverted = RooStats.DoHypoTestInversion(
            workspace, 
            1,                      # n_toys
            2,                      # asymtotic calculator
            3,                      # test type (3 is atlas standard)
            True,                   # use CLs
            20,                     # number of points
            0,                      # POI min
            -1,                     # POI max (why -1?)
            )

    # one might think that inverted.GetExpectedLowerLimit(-1) would do 
    # something different from GetExpectedUpperLimit(-1), but then one would 
    # be wrong...
    mean_limit = inverted.GetExpectedUpperLimit(0)
    lower_limit = inverted.GetExpectedUpperLimit(-1)
    upper_limit = inverted.GetExpectedUpperLimit(1)
    print '{} -- {} -- {}'.format(
        lower_limit, mean_limit, upper_limit)

    
def _fit_and_plot(name, draw_before=False, draw_after=False, 
                  plot_corr_matrix=False, plot_components=False, 
                  plot_nll=False, minos=False): 
    from ROOT import Util
    Util.GenerateFitAndPlot(
        name, 'ana_name', draw_before, draw_after, plot_corr_matrix, 
        plot_components, plot_nll, minos)
                  

def _setup_workspace(config): 
    inf = float('inf')
    backgrounds = [
        'ttbar',
        'Wjets',
        'Zjets','diboson'
        ]
    systematics = [
        'jer',
        'jes',
        'b','c','u',
        ]
    counts = CountDict(config.kinematic_stat_dir, systematics=systematics)

    GeV = 1000.0

    import ROOT
    with OutputFilter(): 
        hf = ROOT.RooStats.HistFactory

    fit = Workspace(counts, systematics, backgrounds)
    fit.set_signal('stop-225-150')
    for cr in ['ttbar0','Wenu0','Wmunu0','Znunu0']: 
        fit.add_cr(cr, 150000.0, 150000.0)

    for sr in ['SR0']: 
        fit.add_sr(sr, 410*GeV, 270*GeV)
    
    fit.save_workspace('results')

def _multispaces(config): 
    inf = float('inf')
    
    # hardcoded for now, consider freeing up
    backgrounds = ['ttbar','Wjets','Zjets','diboson']
    systematics = [
        'jer',
        # 'jes',
        # 'b',
        # 'c',
        # 'u',
        ]
    signal_point = 'stop-225-150'

    counts = CountDict(config.kinematic_stat_dir, systematics=systematics)

    GeV = 1000.0

    import ROOT
    with OutputFilter(): 
        hf = ROOT.RooStats.HistFactory

    met_values = xrange(150,500,20)
    ljpt_values = xrange(200,300,20)
    for met_gev, ljpt_gev in itertools.product(met_values, ljpt_values): 

        # TODO: this leaks memory like crazy because it rebooks histograms
        # for now just using output filters
        fit = Workspace(counts, systematics, backgrounds)
        fit.set_signal(signal_point)
        for cr in ['ttbar0','Wenu0','Wmunu0','Znunu0']: 
            fit.add_cr(cr, 150000.0, 150000.0)

        for sr in ['SR0']: 
            fit.add_sr(sr, met_cut=met_gev*GeV, ljpt_cut=ljpt_gev*GeV)
    
        out_dir = sr_path(
            met_gev=met_gev, pt_gev=ljpt_gev, signal_point=signal_point)
        if not os.path.isdir(out_dir): 
            os.makedirs(out_dir)
        
        fit.save_workspace(out_dir)
        ROOT.gDirectory.GetList().Delete()
        # close method is supposed to delete all the histograms, need to 
        # figure it out... 
        # fit.close()

# --- utility functions ---

def roo_arg_set_itr(all_vars): 
    """
    incredibly hackish way to iterate over workspace variables
    """
    temp = tempfile.NamedTemporaryFile()
    old_out = os.dup(1)
    os.dup2(temp.fileno(), 1)
    all_vars.dump()
    os.dup2(old_out, 1)
    temp.seek(0)
    var_getter = re.compile('\((.*)\)')
    for line in temp: 
        var_search = var_getter.search(line)
        if var_search: 
            variable = all_vars[var_search.group(1)]
            yield variable


if __name__ == "__main__":
    run()
