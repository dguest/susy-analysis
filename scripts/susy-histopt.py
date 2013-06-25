#!/usr/bin/env python2.7
import itertools, tempfile
# from ROOT import kBlack,kWhite,kGray,kRed,kPink,kMagenta,kViolet,kBlue,kAzure,kCyan,kTeal,kGreen,kSpring,kYellow,kOrange
from stop.hists import HistNd
from stop.bullshit import OutputFilter
import h5py
import os, sys, re
from os.path import isfile, isdir, join
from collections import defaultdict
import argparse

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
    meas_name = 'meas'
    def __init__(self, counts, systematics, backgrounds): 
        import ROOT
        with OutputFilter(): 
            self.hf = ROOT.RooStats.HistFactory

        self.counts = counts
        self.systematics = systematics
        self.backgrounds = backgrounds
        self.meas = self.hf.Measurement(self.meas_name, self.meas_name)

        self.meas.SetPOI("mu_SIG")
        for syst in systematics: 
            self.meas.AddConstantParam('alpha_{}'.format(syst))

        self.meas.SetLumi(1.0)
        lumiError = 0.039
        self.meas.SetLumiRelErr(lumiError)
        self.meas.SetExportOnly(False)

    def add_cr(self, cr, met_cut, ljpt_cut): 
        def cut_hist(hist): 
            m_cut = (met_cut,inf)
            j_cut = (ljpt_cut,inf)
            return hist['met'].slice(*m_cut)['leadingJetPt'].slice(*j_cut)

        chan = self.hf.Channel(cr)
        data_count = cut_hist(self.counts['baseline','data',cr]['sum'])
        print data_count
        chan.SetData(data_count)
        chan.SetStatErrorConfig(0.05, "Poisson")

        for bg in self.backgrounds:
            background = self.hf.Sample('_'.join([cr,bg]))
            base_count = cut_hist(self.counts['baseline',bg,cr]['sum'])
            print base_count
            background.SetValue(base_count)
            stat_error = cut_hist(self.counts['baseline',bg,cr]['wt2'])**0.5
            background.GetHisto().SetBinError(1,stat_error)
            background.AddNormFactor('mu_{}'.format(bg), 1,0,10)

            for syst in self.systematics:
                if syst in {'jes', 'u','c','b','t'}: 
                    sup_normed = cut_hist(
                        self.counts[syst + 'up',bg,cr]['sum'])
                    sdn_normed = cut_hist(
                        self.counts[syst + 'down',bg,cr]['sum'])
                    
                    background.AddOverallSys(
                        syst, sup_normed / base_count, 
                        sdn_normed / base_count)
                else: 
                    syst_counts = cut_hist(self.counts[syst,bg,cr]['sum'])
                    rel_syst = syst_counts / base_count - 1
                    background.AddOverallSys(
                        syst, 1 - rel_syst/2, 1 + rel_syst/2)


            chan.AddSample(background)
        self.meas.AddChannel(chan)

    def add_sr(sr, met_cut, ljpt_cut): 
        def cut_hist(hist): 
            m_cut = (met_cut,inf)
            j_cut = (ljpt_cut,inf)
            return hist['met'].slice(*m_cut)['leadingJetPt'].slice(*j_cut)
        

    def save_workspace(self, results_dir='results', prefix='stop'): 
        if not isdir(results_dir): 
            os.mkdir(results_dir)
        self.meas.SetOutputFilePrefix(join(results_dir,prefix))
        with OutputFilter(accept_re='(ERROR:|WARNING:)'): 
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
    
    config = parser.parse_args(sys.argv[1:])
    {'ws':_setup_workspace, 'fit':_histfit}[config.which](config)

def _histfit(config): 
    import ROOT
    ROOT.gSystem.Load("/data2/dguest/blackbox/HistFitter/lib/libSusyFitter.so")
    import configManager as cf
    from ROOT import Util

    workspace = Util.GetWorkspaceFromFile(config.workspace,"combined")
    sim_pdf = workspace.pdf('simPdf')
    model_config = Util.GetModelConfig(workspace)
    observable_set = model_config.GetObservables()
    float_parameters = Util.getFloatParList(sim_pdf, observable_set)
    print float_parameters
    sys.exit('you got work to do here')

    result = Util.FitPdf(workspace)
    for region in ['ttbar0']: 
        comp = Util.GetComponent(workspace,"ttbar",region) 
        print comp.getVal()
        print comp.getPropagatedError(result)
    # nbkgerrinRegionList = [ 
    #     Util.GetPropagatedError(bkginRegion, result) for bkginRegion in bkginRegionList]

def _setup_workspace(config): 
    inf = float('inf')
    backgrounds = ['ttbar','Wjets','Zjets','diboson']
    systematics = ['jer','jes','b','c','u']
    counts = CountDict(config.kinematic_stat_dir, systematics=systematics)

    import ROOT
    with OutputFilter(): 
        hf = ROOT.RooStats.HistFactory

    fit = Workspace(counts, systematics, backgrounds)
    for cr in ['ttbar0','Wenu0','Wmunu0','Znunu0']: 
        fit.add_cr(cr, 150000.0, 150000.0)
    
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
