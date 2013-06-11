#!/usr/bin/env python2.7
import yaml, itertools
# from ROOT import kBlack,kWhite,kGray,kRed,kPink,kMagenta,kViolet,kBlue,kAzure,kCyan,kTeal,kGreen,kSpring,kYellow,kOrange
from stop.hists import HistNd
from stop.bullshit import OutputFilter
import h5py
import os, sys
from os.path import isfile, isdir, join
from collections import defaultdict

class CountDict(dict): 
    """
    Stores as (syst, physics, region)
    """
    def __init__(self, kin_dir, quick=False): 

        self.systematics = []
        for d in os.listdir(kin_dir): 
            if isdir(join(kin_dir,d)): 
                self.systematics.append(d)

        for syst in self.systematics: 
            if quick and syst not in ['baseline', 'jer']: 
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

def run(): 
    inf = float('inf')
    counts = CountDict(sys.argv[1], quick=True)
    print counts['baseline','data', 'ttbar0']['sum']['met'].slice(70000.0, inf)['leadingJetPt'].slice(150000.0, inf)
    
    import ROOT
    with OutputFilter(): 
        hf = ROOT.RooStats.HistFactory

    # Set observed and expected number of events in counting experiment
    lumiError = 0.039 	# Relative luminosity uncertainty

    # ****** here be HistFactory ******

    # Create the measurement
    meas = hf.Measurement("meas", "meas")

    result_dir = 'results'
    if not isdir(result_dir): 
        os.mkdir(result_dir)
    meas.SetOutputFilePrefix(join(result_dir,'example'))
    meas.SetPOI("mu_SIG")
    # meas.AddConstantParam("Lumi")
    for syst in ['jer']: 
        meas.AddConstantParam('alpha_{}'.format(syst))

    meas.SetLumi(1.0)
    meas.SetLumiRelErr(lumiError)
    meas.SetExportOnly(False)

    def cut_hist(hist): 
        d_cut = (150000.0,inf)
        return hist['met'].slice(*d_cut)['leadingJetPt'].slice(*d_cut)

    # Create the channel
    for cr in ['ttbar0']: 
        chan = hf.Channel(cr)
        chan.SetData(cut_hist(counts['baseline','data',cr]['sum']))
        chan.SetStatErrorConfig(0.05, "Poisson")

        for bg in ['ttbar']:
            background = hf.Sample('_'.join([cr,bg]))
            base_count = cut_hist(counts['baseline',bg,cr]['sum'])
            background.SetValue(base_count)
            background.AddNormFactor(bg, 1,0,10)

            for syst in ['jer']:
                if syst in {'JES', 'U','C','B','T'}: 
                    syst_up = fit_config.counts[syst + 'UP'][bg][cr]
                    sup_normed = syst_up['normed']
                    sys_down = fit_config.counts[syst + 'DOWN'][bg][cr]
                    sdn_normed = sys_down['normed']
                    
                    background.AddOverallSys(
                        syst, sup_normed / base_count, 
                        sdn_normed / base_count)
                else: 
                    syst_counts = cut_hist(counts[syst,bg,cr]['sum'])
                    rel_syst = syst_counts / base_count - 1
                    background.AddOverallSys(
                        syst, 1 - rel_syst/2, 1 + rel_syst/2)


            chan.AddSample(background)
        meas.AddChannel(chan)

    # Now, do the measurement
    ROOT.gROOT.SetBatch(True)
    with OutputFilter(accept_re='ERROR:'): 
        pass
    workspace = hf.MakeModelAndMeasurementFast(meas)
    print '------------ fitting -------------------'
    with OutputFilter(): 
        ROOT.RooStats.HistFactory.FitModel(workspace)
    valdict = {}
    for v in roo_arg_set_itr(workspace.allVars()): 
        valdict[v.GetName()] =  v.getValV()

    for n, v in sorted(valdict.iteritems()): 
        if not n.startswith('binWidth') and not n.startswith('nom_'): 
            print n, v

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
