#!/usr/bin/env python2.7
import yaml, itertools
from ROOT import kBlack,kWhite,kGray,kRed,kPink,kMagenta,kViolet,kBlue,kAzure,kCyan,kTeal,kGreen,kSpring,kYellow,kOrange
from stop.bullshit import OutputFilter
import tempfile, os, re
from os.path import isfile

def test_counts(region='UserRegion', data_multiplier=1.0): 
    testcounts = { 
        'NONE': { 
            'Bkg': {
                region: {'normed': 5, 'stats': 25}
                }, 
            'Sig': { 
                region: {'normed': 5, 'stats': 25.0 / 4.0}
                }, 
            'data': { 
                region: 7.0 * data_multiplier
                }
            }, 
        'ucb': { 
            'Bkg': { 
                region: {'normed': 2 * 0.2 * 5}
                }
            }, 
        'ucs': { 
            'Sig': { 
                region: {'normed': 2 * 0.1 * 5}
                }
            }
        }
    return testcounts

def run(): 

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

    # # Create the signal sample
    # signal = hf.Sample("signal")
    # signal.SetValue(5)
    # signal.AddOverallSys("syst1",  0.95, 1.05)
    # signal.AddNormFactor("c1norm", 5, 0, 5)
    # chan.AddSample(signal)


    # Background 1
    # background1 = hf.Sample("background1")
    # background1.SetValue(5)
    # background1.AddOverallSys("syst1", 0.95, 1.05 )
    # background1.AddNormFactor("some_norm", 1,0,10)
    # chan.AddSample(background1)

    # other_chan = hf.Channel('chan2')
    # other_chan.SetData(10.0)
    # other_chan.SetStatErrorConfig(0.05, "Poisson")
    # bg_other = hf.Sample('more_bg')
    # bg_other.SetValue(10.0)
    # bg_other.AddOverallSys("syst1", 0.95, 1.05)
    # bg_other.AddNormFactor("some_norm", 4, 0, 10)
    # other_chan.AddSample(bg_other)

    # meas.AddChannel(chan)
    # meas.AddChannel(other_chan)

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

class FitConfig(object): 
    """
    Reads in the configuration. 
    """
    template = { 
        'fit': { 
            'signal_regions': [], 
            'control_regions': [], 
            },
        
        }
    def __init__(self, file_name): 
        if not isfile(file_name): 
            raise NoFitConfigError('no config file {}'.format(file_name))
        with open(file_name) as config_yml: 
            config = yaml.load(config_yml)
        if not 'fit' in config: 
            raise NoFitConfigError("no 'fit' section in {}".format(file_name))
        try: 
            try: 
                self.counts_file = config['fit']['counts']
            except KeyError: 
                self.counts_file = config['files']['counts']
            self.signal_regions = config['fit']['signal_regions']
            self.control_regions = config['fit']['control_regions']
            self.systematics = config['fit']['systematics']
            self.backgrounds = config['backgrounds']['used']
        except KeyError as err: 
            raise KeyError('{}, raised reading in {}'.format(err, file_name))
        self._counts = None
        
    def load_counts(self): 
        with open(self.counts_file) as counts_yml: 
            self._counts = yaml.load(counts_yml)

    @property
    def counts(self): 
        if not self._counts: 
            raise IOError('counts not loaded')
        return self._counts

class NoFitConfigError(StandardError): 
    def __init__(self, args='no config'): 
        super(NoFitConfigError, self).__init__(args)

if __name__ == "__main__":
    run()
