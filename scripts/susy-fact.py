#!/usr/bin/env python2.7
import yaml, itertools
from ROOT import kBlack,kWhite,kGray,kRed,kPink,kMagenta,kViolet,kBlue,kAzure,kCyan,kTeal,kGreen,kSpring,kYellow,kOrange
from stop.bullshit import OutputFilter
import tempfile, os, re

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

    # Set observed and expected number of events in counting experiment
    ndata     =  3. 	# Number of events observed in data
    lumiError = 0.039 	# Relative luminosity uncertainty
    

    # ****** here be HistFactory ******

    # Create the measurement
    meas = ROOT.RooStats.HistFactory.Measurement("meas", "meas")

    meas.SetOutputFilePrefix("./results/example")
    meas.SetPOI("SigXsecOverSM")
    meas.AddConstantParam("Lumi")
    # meas.AddConstantParam("alpha_syst1")

    meas.SetLumi(1.0)
    meas.SetLumiRelErr(lumiError)
    meas.SetExportOnly(False)

    # Create a channel

    chan = ROOT.RooStats.HistFactory.Channel("channel1")
    chan.SetData(ndata)
    chan.SetStatErrorConfig(0.05, "Poisson")

    # Now, create some samples

    # Create the signal sample
    signal = ROOT.RooStats.HistFactory.Sample("signal")
    signal.SetValue(1.0)
    signal.AddOverallSys("syst1",  0.95, 1.05)
    signal.AddNormFactor("SigXsecOverSM", 5, 0, 5)
    chan.AddSample(signal)


    # Background 1
    background1 = ROOT.RooStats.HistFactory.Sample("background1")
    background1.SetValue(1.0)
    background1.AddOverallSys("syst2", 0.95, 1.05 )
    chan.AddSample(background1)


    # # Background 1
    # background2 = ROOT.RooStats.HistFactory.Sample("background2")
    # background2.SetValue(2.0)
    # background2.AddOverallSys("syst3", 0.95, 1.05 )
    # chan.AddSample(background2)


    # Done with this channel
    # Add it to the measurement:

    meas.AddChannel(chan)

    # Collect the histograms from their files,
    # print some output, 
    # meas.PrintTree();

    # One can print XML code to an
    # output directory:
    # meas.PrintXML("xmlFromCCode", meas.GetOutputFilePrefix());

    # Now, do the measurement
    ROOT.gROOT.SetBatch(True)
    with OutputFilter(): 
        workspace = ROOT.RooStats.HistFactory.MakeModelAndMeasurementFast(meas)
    print '------------ fitting -------------------'
    with OutputFilter(): 
        ROOT.RooStats.HistFactory.FitModel(workspace)
    print workspace.var("SigXsecOverSM").getValV()
    for v in roo_arg_set_itr(workspace.allVars()): 
        print v.GetName(), v.getValV()

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
            self.counts_file = config['files']['counts']
            self.signal_regions = config['fit']['signal_regions']
            self.control_regions = config['fit']['control_regions']
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
