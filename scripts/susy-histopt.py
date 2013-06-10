#!/usr/bin/env python2.7
import yaml, itertools
# from ROOT import kBlack,kWhite,kGray,kRed,kPink,kMagenta,kViolet,kBlue,kAzure,kCyan,kTeal,kGreen,kSpring,kYellow,kOrange
from stop.hists import HistNd
import h5py
import os, sys
from os.path import isfile, isdir, join
from collections import defaultdict

class CountDict(dict): 
    def __init__(self, kin_dir, region): 

        self.systematics = []
        for d in os.listdir(kin_dir): 
            if isdir(join(kin_dir,d)): 
                self.systematics.append(d)

        for syst in self.systematics: 
            tmp = defaultdict(dict)
            print 'loading {}'.format(syst)
            agg_path = join(kin_dir, syst, 'aggregate.h5')
            if not isfile(agg_path): 
                raise OSError("{} not found".format(agg_path))
            with h5py.File(agg_path, 'r') as h5file: 
               for phys, var_group in h5file.iteritems(): 
                   for var, reg_group in var_group.iteritems():
                       hist = reg_group[region]
                       if var == 'kinematics': 
                           tmp[syst, phys]['sum'] = HistNd(hist)
                       elif var == 'kinematicWt2': 
                           tmp[syst, phys]['wt2'] = HistNd(hist)
            self.update(tmp)

def run(): 
    inf = float('inf')
    ext = CountDict(sys.argv[1], 'anti0')
    print ext['baseline','ttbar']['sum']['met'].slice(70000.0, inf)['leadingJetPt'].slice(100000.0, inf)
    
def other(): 
    import ROOT
    with OutputFilter(): 
        hf = ROOT.RooStats.HistFactory

    # Set observed and expected number of events in counting experiment
    lumiError = 0.039 	# Relative luminosity uncertainty

    # ****** here be HistFactory ******

    fit_config = FitConfig('stack.yml')
    print '--- loading counts ---'
    fit_config.load_counts()
    print 'done'
    # Create the measurement
    meas = hf.Measurement("meas", "meas")

    meas.SetOutputFilePrefix("./results/example")
    meas.SetPOI("c1norm")
    meas.AddConstantParam("Lumi")
    for syst in fit_config.systematics: 
        meas.AddConstantParam('alpha_{}'.format(syst))

    meas.SetLumi(1.0)
    meas.SetLumiRelErr(lumiError)
    meas.SetExportOnly(False)

    # Create the channel
    for cr in fit_config.control_regions: 
        chan = hf.Channel(cr)
        chan.SetData(fit_config.counts['NONE']['data'][cr])
        chan.SetStatErrorConfig(0.05, "Poisson")

        for bg in fit_config.backgrounds: 
            background = hf.Sample('_'.join([cr,bg]))
            base_count = fit_config.counts['NONE'][bg][cr]['normed']
            background.SetValue(base_count)
            background.AddNormFactor(bg, 1,0,10)

            for syst in fit_config.systematics: 
                if syst in {'JES', 'U','C','B','T'}: 
                    syst_up = fit_config.counts[syst + 'UP'][bg][cr]
                    sup_normed = syst_up['normed']
                    sys_down = fit_config.counts[syst + 'DOWN'][bg][cr]
                    sdn_normed = sys_down['normed']
                    
                    background.AddOverallSys(
                        syst, sup_normed / base_count, 
                        sdn_normed / base_count)
                else: 
                    syst_counts = fit_config.counts[syst][bg][cr]
                    syst_normed = syst_counts['normed']
                    rel_syst = syst_normed / base_count - 1
                    background.AddOverallSys(
                        syst, 1 - rel_syst/2, 1 + rel_syst/2)


            chan.AddSample(background)
        meas.AddChannel(chan)

    # Now, do the measurement
    ROOT.gROOT.SetBatch(True)
    with OutputFilter(accept_re='ERROR:'): 
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
