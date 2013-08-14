#!/usr/bin/env python2.7
import itertools, tempfile
from stop.bullshit import OutputFilter
import os, sys, re, yaml, fnmatch
from os.path import dirname, join, isfile
import argparse
from pyroot.fitter import CountDict, Workspace, UpperLimitCalc
from pyroot.fitter import sr_from_path, path_from_sr
from time import time
    
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
    upper_lim.add_argument(
        'workspace', help='can also be a textfile pointing to the workspace')
    upper_lim.add_argument(
        '-y', '--save-yaml', nargs='?', const='fit-result.yml', 
        help='save output as yaml in the workspace directory '
        '(with no args: %(const)s)')

    # TODO: move this to susy-postfit
    agg = subparsers.add_parser('agg', description=_aggregate.__doc__)
    agg.add_argument('root_dir_name')
    agg.add_argument('-o','--output-file')
    agg.add_argument('--fit-result-name', default='fit-result.yml')

    multispace = subparsers.add_parser('ms')
    multispace.add_argument('kinematic_stat_dir')
    multispace.add_argument(
        '-s','--signal-point', default='stop-225-150', 
        help='default: %(default)s, allows globbing, and numbering')
    
    config = parser.parse_args(sys.argv[1:])
    opts = {'ws':_setup_workspace, 'fit':_new_histfit, 'pval':_get_p_value,
            'ul':_get_upper_limit, 'ms':_multispaces, 'agg': _aggregate}
    opts[config.which](config)

class ProgressMeter(object): 
    def __init__(self, level=3): 
        self.last_epoch = None
        self.level = level
        print 'walking'
        self.n_query = 0
        self.start = time()

    def get_walk_progress(self,root): 
        self.n_query += 1
        level = self.level
        dir_path = root.split('/')
        if len(dir_path) >= level:
            if dir_path[-level] != self.last_epoch: 
                self.last_epoch = dir_path[-level]
                rate = self.n_query / float(time() - self.start) 
                self.start = time()
                self.n_query = 0
                print 'working on {} ({:.1f} Hz)'.format(
                    dir_path[-level], rate)

def _aggregate(config): 
    """
    aggregate yaml output files from fits
    """
    all_points = []
    prog = ProgressMeter(3)
    for root, dirs, files in os.walk(config.root_dir_name): 
        if config.fit_result_name in files: 
            prog.get_walk_progress(root)
            met, ljpt, sp, tag = sr_from_path(root)
            with open(join(root, config.fit_result_name)) as result: 
                res_dict = yaml.load(result)
            new_info = { 
                'met': met, 
                'leading_jet_pt': ljpt, 
                'signal_point': sp, 
                'tag_config': tag
                }
            new_info.update(res_dict)
            all_points.append(new_info)
    if config.output_file: 
        if config.output_file.endswith('.yml'): 
            with open(config.output_file,'w') as yml: 
                yml.writelines(yaml.dump(all_points))
        elif config.output_file.endswith('h5'): 
            from stop.postfit import kinematic_plane_from_pointlist
            import h5py
            

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

    if config.workspace.endswith('.txt'): 
        with open(config.workspace) as txt_file: 
            workspace_name = list(line.strip() for line in txt_file)[0]
    else: 
        workspace_name = config.workspace

    ul_calc = UpperLimitCalc()
    lower_limit, mean_limit, upper_limit = ul_calc.lim_range(workspace_name)

    ul_dict = {
        'upper': upper_limit, 
        'lower': lower_limit, 
        'mean': mean_limit, 
        }

    if config.save_yaml: 
        yaml_dict = { 'upper_limit': ul_dict } 
        yaml_path = join(dirname(workspace_name), config.save_yaml)
        with open(yaml_path,'w') as yml: 
            yml.write(yaml.dump(yaml_dict))
        return 

    fits_db = 'all-fit-results.db'
    if isfile(fits_db): 
        met, pt, sp, tag = sr_from_path(workspace_name)
        insert_sql(met, pt, sp, tag, ul_dict, fits_db)

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
    # hardcoded for now, consider freeing up
    systematics = [
        'jer',
        'jes',
        'b',
        'c',
        'u',
        ]

    counts = CountDict(config.kinematic_stat_dir, systematics=systematics)
    all_sp = sorted({k[1] for k in counts if 'stop' in k[1]})
    try: 
        signal_points = [all_sp[int(config.signal_point)]]
    except ValueError: 
        signal_points = fnmatch.filter(all_sp, config.signal_point)

    for sp in signal_points: 
        print 'booking signal point {}'.format(sp)
        _book_signal_point(counts, sp, systematics)

def _book_signal_point(counts, signal_point, systematics): 
    import ROOT
    backgrounds = ['ttbar','Wjets','Zjets','diboson']
    GeV = 1000.0

    met_values = xrange(150,500,20)
    ljpt_values = xrange(150,500,20)
    for met_gev, ljpt_gev in itertools.product(met_values, ljpt_values): 
        print 'building met {:.0f}, ljpt {:.0f}'.format(met_gev, ljpt_gev)
        # TODO: this leaks memory like crazy, not sure why but bug reports
        # have been filed. For now just using output filters. 
        fit = Workspace(counts, systematics, backgrounds)
        fit.set_signal(signal_point)
        for cr in ['ttbar0','Wenu0','Wmunu0','Znunu0']: 
            fit.add_cr(cr, 150000.0, 150000.0)

        for sr in ['SR0']: 
            fit.add_sr(sr, met_cut=met_gev*GeV, ljpt_cut=ljpt_gev*GeV)
    
        out_dir = path_from_sr(
            met_gev=met_gev, pt_gev=ljpt_gev, signal_point=signal_point)
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
