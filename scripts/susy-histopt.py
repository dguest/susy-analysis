#!/usr/bin/env python2.7
"""
Top level for all fitting / workspace creation. Also serves to keep 
PyROOT crap out of the rest of the framework. 
"""
import itertools, tempfile
from stop.bullshit import OutputFilter
import os, sys, re, yaml, fnmatch
from os.path import dirname, join, isfile
import argparse
from pyroot.fitter import CountDict, Workspace, UpperLimitCalc
from pyroot.fitter import sr_from_path, path_from_sr, insert_sql
import linecache
import time
import warnings
    
def run(): 
    parser = argparse.ArgumentParser(description=__doc__)
    subparsers = parser.add_subparsers(dest='which')

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
    upper_lim.add_argument(
        'workspace', help='can also be a textfile pointing to the workspace')
    upper_lim.add_argument(
        '-y', '--yaml-dir', default='fit-results', 
        help='save output as yaml in the workspace directory '
        '(default: %(default)s)')
    upper_lim.add_argument(
        '-n', '--ws-number', type=int, default=0, 
        help='skip to this line (or slice) in the workspaces text file')
    upper_lim.add_argument(
        '-m', '--n-workspaces', type=int, default=1, 
        help='fit slice of workspaces (assumes -n is given)')


    multispace = subparsers.add_parser('ms')
    multispace.add_argument('kinematic_stat_dir')
    multispace.add_argument('-y','--fit-config', required=True)
    multispace.add_argument('-n','--job-number',type=int)
    multispace.add_argument('-t','--total-jobs',type=int)
    
    config = parser.parse_args(sys.argv[1:])
    opts = {'fit':_new_histfit, 'pval':_get_p_value,
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
    if config.workspace.endswith('.txt'): 
        n_ws = config.n_workspaces
        sl_number = config.ws_number
        sl_start = sl_number * n_ws
        sl_end = (sl_number + 1) * n_ws
        with open(config.workspace) as txt_file: 
            workspaces = txt_file.readlines()[sl_start:sl_end]
    else: 
        workspaces = [config.workspace]
    for workspace_name in workspaces: 
        _ul_from_workspace(workspace_name.strip(), config.yaml_dir)

def _ul_from_workspace(workspace_name, yaml_dir): 
    import ROOT
    from pyroot import utils
    utils.load_susyfit()
    from ROOT import Util
    from ROOT import RooStats

    ul_calc = UpperLimitCalc()
    lower_limit, mean_limit, upper_limit = ul_calc.lim_range(workspace_name)

    ul_dict = {
        'upper': upper_limit, 
        'lower': lower_limit, 
        'mean': mean_limit, 
        }

    met, pt, sp, tag = sr_from_path(workspace_name)
    utils.make_dir_if_none(yaml_dir)
        
    bname = 'met{}_pt{}_{}_{}_fit-result.yml'.format(met, pt, sp, tag)
    yaml_dict = { 
        'upper_limit': ul_dict,  
        'met': met, 
        'leading_jet_pt': pt, 
        'signal_point': sp, 
        'tag_config': tag} 
    yaml_path = join(yaml_dir, bname)
    with open(yaml_path,'w') as yml: 
        yml.write(yaml.dump(yaml_dict))
    
def _fit_and_plot(name, draw_before=False, draw_after=False, 
                  plot_corr_matrix=False, plot_components=False, 
                  plot_nll=False, minos=False): 
    from ROOT import Util
    Util.GenerateFitAndPlot(
        name, 'ana_name', draw_before, draw_after, plot_corr_matrix, 
        plot_components, plot_nll, minos)
                  
def _multispaces(config): 
    # hardcoded for now, consider freeing up
    systematics = [
        'jer',
        'jes',
        'b',
        'c',
        'u',
        'el', 
        'mu', 
        'met', 
        ]

    counts = CountDict(config.kinematic_stat_dir, systematics=systematics)
    all_sp = sorted({k[1] for k in counts if 'stop' in k[1]})

    with open(config.fit_config) as yml: 
        fit_configs = yaml.load(yml)

    # get indices from the total and other parameters
    total_jobs = config.total_jobs
    job_number = config.job_number
    sp_index = job_number % len(all_sp)
    sp_remain = job_number // len(all_sp)
    fit_index = sp_remain % len(fit_configs)
    fit_remain = sp_remain // len(fit_configs)
    n_kin_subjobs = total_jobs // len(all_sp) // len(fit_configs)
    assert n_kin_subjobs > 0

    signal_point = all_sp[sp_index]
    this_fit = list(fit_configs)[fit_index]

    print 'booking signal point {}'.format(signal_point)
    _book_signal_point(counts, signal_point, systematics, this_fit, 
                       fit_configs[this_fit], n_kin_subjobs, fit_remain)

def _book_signal_point(counts, signal_point, systematics, 
                       config_name, fit_config, n_kin_subjobs, selected_job): 
    assert selected_job < n_kin_subjobs
    import ROOT
    backgrounds = fit_config['backgrounds']
    GeV = 1000.0

    met_values = xrange(150,500,20)
    ljpt_values = xrange(150,500,20)
    kin_iter = enumerate(itertools.product(met_values, ljpt_values))
    for jobn, (met_gev, ljpt_gev) in kin_iter: 
        if jobn % n_kin_subjobs != selected_job: 
            continue
        print 'building met {:.0f}, ljpt {:.0f}'.format(met_gev, ljpt_gev)
        # TODO: this leaks memory like crazy, not sure why but bug reports
        # have been filed. For now just using output filters. 
        fit = Workspace(counts, systematics, backgrounds)
        fit.set_signal(signal_point)
        for cr in fit_config['control_regions']: 
            fit.add_cr(cr, 150000.0, 150000.0)

        sr = fit_config['signal_region']
        fit.add_sr(sr, met_cut=met_gev*GeV, ljpt_cut=ljpt_gev*GeV)
    
        out_dir = path_from_sr(
            met_gev=met_gev, pt_gev=ljpt_gev, signal_point=signal_point, 
            tag_config=config_name)
        if not os.path.isdir(out_dir): 
            os.makedirs(out_dir)
        
        fit.save_workspace(out_dir)
        ROOT.gDirectory.GetList().Delete()
    

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
