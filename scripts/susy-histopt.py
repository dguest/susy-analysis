#!/usr/bin/env python2.7
import itertools, tempfile
# from ROOT import kBlack,kWhite,kGray,kRed,kPink,kMagenta,kViolet,kBlue,kAzure,kCyan,kTeal,kGreen,kSpring,kYellow,kOrange
from stop.hists import HistNd
from stop.bullshit import OutputFilter
import h5py
import os, sys, re
from os.path import isfile, isdir, join, dirname
from collections import defaultdict, Counter
import argparse
import warnings

class CountDict(dict): 
    """
    Stores as (syst, physics, region)
    """
    def __init__(self, kin_dir, systematics='all'): 

        def has_systematic(syst): 
            if syst in systematics + ['baseline']: 
                return True
            if syst.replace('up','') in systematics:
                return True
            if syst.replace('down','') in systematics: 
                return True
            return False

        self.systematics = []
        for d in os.listdir(kin_dir): 
            if isdir(join(kin_dir,d)): 
                self.systematics.append(d)

        for syst in self.systematics: 
            if not has_systematic(syst): 
                continue
            tmp = defaultdict(dict)
            print 'loading {}'.format(syst)
            agg_path = join(kin_dir, syst, 'aggregate.h5')
            if not isfile(agg_path): 
                raise OSError("{} not found".format(agg_path))
            with h5py.File(agg_path, 'r') as h5file: 
               for phys, var_group in h5file.iteritems(): 
                   for var, reg_group in var_group.iteritems():
                       for region, hist in reg_group.iteritems(): 
                           if var == 'kinematics': 
                               tmp[syst, phys, region]['sum'] = HistNd(hist)
                           elif var == 'kinematicWt2': 
                               tmp[syst, phys, region]['wt2'] = HistNd(hist)
            self.update(tmp)

inf = float('inf')

class Workspace(object): 
    '''
    Organizes the building of workspaces, mainly by providing functions to 
    transfer from a custom Histogram class to a RooFit workspace. 
    '''
    meas_name = 'meas'
    def __init__(self, counts, systematics, backgrounds): 
        import ROOT
        with OutputFilter(): 
            self.hf = ROOT.RooStats.HistFactory

        self.counts = counts
        self.systematics = systematics
        self.backgrounds = backgrounds
        self.meas = self.hf.Measurement(self.meas_name, self.meas_name)

        self.signal_point = None
        for syst in systematics: 
            self.meas.AddConstantParam('alpha_{}'.format(syst))

        self.meas.SetLumi(1.0)
        lumiError = 0.039
        self.meas.SetLumiRelErr(lumiError)
        self.meas.SetExportOnly(False)

        # for blinding / pseudodata
        self.region_sums = Counter()
        self.do_pseudodata = False
        self.blinded = True
        self.pseudodata_regions = {}
        
        # we have to add the channels _after_ adding data
        # so using pseudodata means we have to save the channels
        # and add them later
        self.channels = {}

    def set_signal(self, signal_name): 
        valid_signals = set(zip(*self.counts.keys())[1])
        if not signal_name in valid_signals: 
            possible_sigs = '\n'.join(valid_signals)
            raise ValueError(
                '{} not in:\n {}'.format(signal_name, possible_sigs))
        if self.signal_point: 
            raise ValueError('tried to overwrite {} with {}'.format(
                    self.signal_point, signal_name))
        self.signal_point = signal_name
        self.meas.SetPOI("mu_{}".format(signal_name))

    def _add_mc_to_channel(self, chan, sr, cutfunc): 
        """
        Adds the signal mc and the backgrounds to this channel. 
        Will throw exceptions if the signal isn't set. 
        """
        if self.signal_point: 
            signal = self.hf.Sample('_'.join([self.signal_point,sr]))
            sig_hists = self.counts['baseline', self.signal_point, sr]
            signal_count = cutfunc(sig_hists['sum'])
            signal.SetValue(signal_count)
            if self.do_pseudodata: 
                self.region_sums[sr] += signal_count
            sig_stat_error = cutfunc(sig_hists['wt2'])**0.5
            signal.GetHisto().SetBinError(1,sig_stat_error)
            signal.SetNormalizeByTheory(True)
            signal.AddNormFactor('mu_{}'.format(self.signal_point),1,0,2)
            chan.AddSample(signal)

        for bg in self.backgrounds:
            background = self.hf.Sample('_'.join([sr,bg]))
            base_count = cutfunc(self.counts['baseline',bg,sr]['sum'])
            self.region_sums[sr] += base_count
            if base_count == 0.0: 
                warn_str = ('zero base count found in {}'
                            ' skipping').format(bg)
                warnings.warn(warn_str, stacklevel=2)
                continue

            background.SetValue(base_count)
            stat_error = cutfunc(self.counts['baseline',bg,sr]['wt2'])**0.5
            background.GetHisto().SetBinError(1,stat_error)
            background.AddNormFactor('mu_{}'.format(bg), 1,0,10)
            # background.AddNormFactor('Lumi', 1,0,10)

            for syst in self.systematics:
                if syst in {'jes', 'u','c','b','t'}: 
                    sup_normed = cutfunc(
                        self.counts[syst + 'up',bg,sr]['sum'])
                    sdn_normed = cutfunc(
                        self.counts[syst + 'down',bg,sr]['sum'])
                    
                    background.AddOverallSys(
                        syst, sup_normed / base_count, 
                        sdn_normed / base_count)
                else: 
                    syst_counts = cutfunc(self.counts[syst,bg,sr]['sum'])
                    rel_syst = syst_counts / base_count - 1
                    background.AddOverallSys(
                        syst, 1 - rel_syst/2, 1 + rel_syst/2)

            chan.AddSample(background)

    def add_cr(self, cr, met_cut, ljpt_cut): 
        def cut_hist(hist): 
            m_cut = (met_cut,inf)
            j_cut = (ljpt_cut,inf)
            return hist['met'].slice(*m_cut)['leadingJetPt'].slice(*j_cut)

        chan = self.hf.Channel(cr)
        data_count = cut_hist(self.counts['baseline','data',cr]['sum'])
        if self.do_pseudodata: 
            self.pseudodata_regions[cr] = chan
        else: 
            chan.SetData(data_count)
        chan.SetStatErrorConfig(0.05, "Poisson")

        self._add_mc_to_channel(chan, cr, cut_hist)

        self.channels[cr] = chan

    def add_sr(self, sr, met_cut, ljpt_cut): 
        def cut_hist(hist): 
            m_cut = (met_cut,inf)
            j_cut = (ljpt_cut,inf)
            return hist['met'].slice(*m_cut)['leadingJetPt'].slice(*j_cut)

        chan = self.hf.Channel(sr)
        if self.blinded: 
            self.pseudodata_regions[sr] = chan
        else: 
            data_count = cut_hist(self.counts['baseline','data',sr]['sum'])
            chan.SetData(data_count)
        chan.SetStatErrorConfig(0.05, "Poisson")

        self._add_mc_to_channel(chan, sr, cut_hist)

        self.channels[sr] = chan
        

    def save_workspace(self, results_dir='results', prefix='stop',
                       verbose=False): 
        if not self.signal_point: 
            self.meas.SetPOI("mu_SIG")
        if not isdir(results_dir): 
            os.mkdir(results_dir)

        for chan_name, channel in self.channels.iteritems(): 
            if chan_name in self.pseudodata_regions: 
                pseudo_count = self.region_sums[chan_name]
                channel.SetData(pseudo_count)
            self.meas.AddChannel(channel)

        self.meas.SetOutputFilePrefix(join(results_dir,prefix))
        pass_strings = ['ERROR:','WARNING:']
        if verbose: 
            pass_strings.append('INFO:')
        self.meas.SetExportOnly(True)
        with OutputFilter(accept_re='({})'.format('|'.join(pass_strings))): 
            workspace = self.hf.MakeModelAndMeasurementFast(self.meas)

        
    def fit(self): 
        """
        This isn't actually called anywhere. 
        """
        # Now, do the measurement
        import ROOT
        from ROOT import Util
        # ROOT.gROOT.SetBatch(True)
        raise NotImplementedError("this doesn't work right now")
        with OutputFilter(accept_re='(ERROR:|WARNING:)'): 
            workspace = self.hf.MakeModelAndMeasurementFast(self.meas)
        print '------------ fitting -------------------'
        with OutputFilter(accept_re='(ERROR:|WARNING:)'): 
            ROOT.RooStats.HistFactory.FitModel(workspace)
        valdict = {}
        for v in roo_arg_set_itr(workspace.allVars()): 
            valdict[v.GetName()] =  v.getVal()
    
        for n, v in sorted(valdict.iteritems()): 
            if not n.startswith('binWidth') and not n.startswith('nom_'): 
                print n, v
    
def run(): 
    parser = argparse.ArgumentParser()
    subparsers = parser.add_subparsers(dest='which')

    ws = subparsers.add_parser('ws')
    ws.add_argument('kinematic_stat_dir')

    fit = subparsers.add_parser('fit')
    fit.add_argument('workspace')
    fit.add_argument('-l','--upper-limit',action='store_true')
    
    config = parser.parse_args(sys.argv[1:])
    {'ws':_setup_workspace, 'fit':_new_histfit}[config.which](config)

def _load_susyfit(): 
    """
    Loads the root interfaces with HistFitter
    """
    from distutils import spawn
    hf = dirname(spawn.find_executable('HistFitter.py'))
    import ROOT
    with OutputFilter(): 
        ROOT.gSystem.Load('{}/../lib/libSusyFitter.so'.format(hf))

def _new_histfit(config): 
    import ROOT
    _load_susyfit()

    # import configManager as cf
    # from ROOT import Util
    # import ConfigMgr as mgr
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

    _fit_and_plot(fit_config.m_name, 
                  draw_before=True, draw_after=True, plot_corr_matrix=True)
    raw_input('press ENTER')
    mgr.m_outputFileName = 'upper-lim.root'
    if config.upper_limit: 
        mgr.doUpperLimitAll()
  
    
def _fit_and_plot(name, draw_before=False, draw_after=False, 
                  plot_corr_matrix=False, plot_components=False, 
                  plot_nll=False, minos=False): 
    from ROOT import Util
    Util.GenerateFitAndPlot(
        name, 'ana_name', draw_before, draw_after, plot_corr_matrix, 
        plot_components, plot_nll, minos)
                            
                  

def _setup_workspace(config): 
    inf = float('inf')
    backgrounds = ['ttbar','Wjets','Zjets','diboson']
    systematics = ['jer','jes','b','c','u']
    counts = CountDict(config.kinematic_stat_dir, systematics=systematics)

    GeV = 1000.0

    import ROOT
    with OutputFilter(): 
        hf = ROOT.RooStats.HistFactory

    fit = Workspace(counts, systematics, backgrounds)
    # fit.do_pseudodata = True
    fit.set_signal('stop-150-75')
    for cr in ['ttbar0','Wenu0','Wmunu0','Znunu0']: 
        fit.add_cr(cr, 150000.0, 150000.0)

    for sr in ['SR0']: 
        fit.add_sr(sr, 410*GeV, 270*GeV)
    
    fit.save_workspace('results')
    # histfit('results/example_combined_meas_model.root')

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
