#!/usr/bin/env python2.7
import ROOT
ROOT.PyConfig.IgnoreCommandLineOptions = True

from ROOT import gROOT,gSystem,gDirectory,RooAbsData,RooRandom,RooWorkspace
gSystem.Load("/data2/dguest/blackbox/HistFitter/lib/libSusyFitter.so")
from ROOT import ConfigMgr
gROOT.Reset()
import os
import argparse
import itertools

# *** copied from config file ***
from configManager import configMgr
import configManager
from ROOT import kBlack,kWhite,kGray,kRed,kPink,kMagenta,kViolet,kBlue,kAzure,kCyan,kTeal,kGreen,kSpring,kYellow,kOrange
from configWriter import fitConfig,Measurement,Channel,Sample
from systematic import Systematic
from math import sqrt

import os

# Setup for ATLAS plotting
from ROOT import gROOT
from os.path import dirname, isfile, isdir
atl_style = '/'.join(configManager.__file__.split('/')[:-2] + [
        'macros/AtlasStyle.C'])
gROOT.LoadMacro(atl_style)
import ROOT
ROOT.SetAtlasStyle()
# --- end of config file copy ---

import yaml


from logger import Logger
log = Logger('HistFitter')


def GenerateFitAndPlotCPP(fc, anaName, drawBeforeFit, drawAfterFit, drawCorrelationMatrix, drawSeparateComponents, drawLogLikelihood, minos, minosPars):
    from ROOT import Util
    
    log.debug('GenerateFitAndPlotCPP: anaName %s ' % anaName)
    log.debug("GenerateFitAndPlotCPP: drawBeforeFit %s " % drawBeforeFit) 
    log.debug("GenerateFitAndPlotCPP: drawAfterFit %s " % drawAfterFit) 
    log.debug("GenerateFitAndPlotCPP: drawCorrelationMatrix %s " % drawCorrelationMatrix) 
    log.debug("GenerateFitAndPlotCPP: drawSeparateComponents %s " % drawSeparateComponents)
    log.debug("GenerateFitAndPlotCPP: drawLogLikelihood %s " % drawLogLikelihood)
    log.debug("GenerateFitAndPlotCPP: minos %s " % minos)
    log.debug("GenerateFitAndPlotCPP: minosPars %s " % minosPars)

    Util.GenerateFitAndPlot(fc.name, anaName, drawBeforeFit, drawAfterFit, drawCorrelationMatrix,
                            drawSeparateComponents, drawLogLikelihood, minos, minosPars)

class ChannelFactory(object): 
    def __init__(self, counts, asym_systematics, sym_systematics): 
        self.counts = counts
        self.asym_systematics = asym_systematics
        self.sym_systematics = sym_systematics
        self.color_itr = itertools.cycle([
                kBlack,kWhite,kGray,kRed,kPink,kMagenta,kViolet,kBlue,
                kAzure,kCyan,kTeal,kGreen,kSpring,kYellow,kOrange])
        self.sample_colors = {}

class SampleFactory(object): 
    color_itr = itertools.cycle([
            kGray,kRed,kPink,kMagenta,kViolet,kBlue,
            kAzure,kCyan,kTeal,kGreen,kSpring,kYellow,kOrange])
    def __init__(self, counts, asym_systematics=[], sym_systematics=[]): 
        self.counts = counts
        self.what_the_fuck_is_this = 'cuts' 
        self.asym_systematics = asym_systematics
        self.sym_systematics = sym_systematics
        
    def get_sample(self, sample, region):
        roo_sample = Sample(name=sample, color=next(self.color_itr))
        nominal = self.counts['NONE'][sample][region]['normed']
        stats = self.counts['NONE'][sample][region]['stats']
        roo_sample.setStatConfig(True)
        roo_sample.buildHisto(binValues=[nominal], region=region, 
                              var=self.what_the_fuck_is_this)
        roo_sample.buildStatErrors(binStatErrors=[1/stats**0.5 * nominal], 
                                   region=region, 
                                   var=self.what_the_fuck_is_this)
        for syst_name in self.sym_systematics: 
            syst_count = self.counts[syst_name][sample][region]['normed']
            high = syst_count / nominal / 2 # TODO: check me 
            low = 1.0 - high
            syst = Systematic(name=syst_name, nominal=1.0, 
                              high=high, low=low, type='user', 
                              method="userOverallSys")
            roo_sample.addSystematic(syst)
        roo_sample.setNormFactor(
            name='mu_{}'.format(sample), val=1.0, low=0.0, high=2.0)
        return roo_sample
    def get_data_sample(self, region): 
        roo_sample = Sample('Data', kBlack)
        roo_sample.setData()
        n_data = self.counts['NONE']['data'][region]
        roo_sample.buildHisto([n_data], region, self.what_the_fuck_is_this)
        return roo_sample

def run(): 
    from configManager import configMgr

    configMgr.readFromTree = False
    configMgr.executeHistFactory = False
    runInterpreter = False
    runFit = False
    printLimits = False
    doHypoTests = False
    doUL = True           # default is exclusion. goes together with doHypoTests
    drawBeforeFit = False
    drawAfterFit = False
    drawCorrelationMatrix = False
    drawSeparateComponents = False
    drawLogLikelihood = False
    pickedSRs = []
    runToys = False
    runMinos = False
    minosPars = ""
    
    print "\n * * * Welcome to HistFitter * * *\n"

    parser = argparse.ArgumentParser()
    parser.add_argument('config')
    parser.add_argument("-L", "--log-level", help="set log level", choices=["VERBOSE", "DEBUG", "INFO", "WARNING", "ERROR", "FATAL", "ALWAYS"])
    parser.add_argument("-w", "--create-workspace", help="re-create workspace from histograms", action="store_true", default=configMgr.executeHistFactory)
    parser.add_argument("-x", "--use-XML", help="write XML files by hand and call hist2workspace on them, instead of directly writing workspaces", action="store_true", default=configMgr.writeXML)
    parser.add_argument("-f", "--fit", help="fit the workspace", action="store_true", default=configMgr.executeHistFactory)
    parser.add_argument("-m", "--minos", help="run minos for asymmetric error calculation, optionally give parameter names for which minos should be run, space separated. For all params, use ALL", metavar="PARAM")
    parser.add_argument("-n", "--num_toys", type=int, help="set the number of toys, <=0 means to use real data", default=configMgr.nTOYs)
    parser.add_argument("-s", "--seed", type=int, help="set the random seed for toy generation", default=configMgr.toySeed)
    parser.add_argument("-a", "--use-asimov", help="use Asimov dataset for fitting and plotting", action="store_true", default=configMgr.useAsimovSet)
    parser.add_argument("-i", "--interactive", help="remain in interactive mode after running", action="store_true", default=runInterpreter)
    parser.add_argument("-l", "--limit-plot", help="make limit plot of workspace", action="store_true", default=printLimits)
    parser.add_argument("-p", "--hypotest", help="run exclusion hypothesis test", action="store_true", default=doHypoTests)
    parser.add_argument("-z", "--discovery-hypotest", help="run discovery hypothesis test", action="store_true", default=not doUL)
    parser.add_argument("-g", "--grid_points", help="grid points to process (comma-seperated)")
    parser.add_argument("-r", "--regions", help="signal regions to process (comma-seperated)", default="all")
    
    parser.add_argument("-d", action="store_true", help="draw before/after plots")
    parser.add_argument("-D", "--draw", help="specify plots to draw, comma separated; choose from "+str(["allPlots", "before","after", "corrMatrix", "sepComponents", "likelihood"]))
    
    parser.add_argument("-b", "--background", help="when doing hypotest, set background levels to values, form of bkgParName,value")
    parser.add_argument("-0", "--no-empty", help="do not draw empty bins when drawing", action="store_true")
    parser.add_argument("-T", "--run-toys", help="run toys (default with mu)", action="store_true")
    parser.add_argument("-c", "--cmd", help="python commands to process (semi-colon-seperated)")
    parser.add_argument("-u", "--userArg", help="arbitrary user argument(s)", default="")

    args = parser.parse_args()
   

    if args.create_workspace:
        configMgr.executeHistFactory = True

    if args.use_XML:
        configMgr.writeXML = True

    if args.fit:
        runFit = True

    configMgr.userArg=args.userArg
    configMgr.nTOYs = args.num_toys

    if args.interactive:
        runInterpreter = True

    if args.log_level:
        log.setLevel(args.log_level, True) #do not add a default to args.log_level or we will always lock it

    if args.limit_plot:
        printLimits = True

    if args.hypotest:
        doHypoTests = True

    if args.discovery_hypotest:
        doHypoTests = True
        doUL = False

    if args.d:
        drawBeforeFit = True
        drawAfterFit = True
    
    if args.draw:
        drawArgs = args.draw.split(",")
        if len(drawArgs) == 1 and drawArgs[0] == "allPlots":
            drawBeforeFit = True
            drawAfterFit = True
            drawCorrelationMatrix = True
            drawSeparateComponents = True
            drawLogLikelihood = True
        elif len(drawArgs)>0:
            for drawArg in drawArgs:
                if drawArg == "before":
                    drawBeforeFit = True
                elif drawArg == "after":
                    drawAfterFit = True
                elif drawArg == "corrMatrix":
                    drawCorrelationMatrix = True
                elif drawArg == "sepComponents":
                    drawSeparateComponents = True
                elif drawArg == "likelihood":
                    drawLogLikelihood = True
                else:
                    log.fatal("Wrong draw argument: %s\n  Possible draw arguments are 'allPlots' or comma separated 'before after corrMatrix sepComponents likelihood'" % drawArg) 

    if args.no_empty:
        configMgr.removeEmptyBins = True

    if args.seed != 0: #0 is default because type is int
        configMgr.toySeedSet = True
        configMgr.toySeed = args.seed

    if args.use_asimov:
        configMgr.useAsimovSet = True

    if args.grid_points and args.grid_points != "":
        sigSamples = args.grid_points.split(",")
        log.info("Grid points specified: %s" % sigSamples)

    if args.regions and args.regions != "" and args.regions != "all":
        pickedSRs = args.regions.split(",")
    else:
        pickedSRs = [] #MB: used by 0-lepton fit

    if len(pickedSRs) > 0: 
        log.info("Selected signal regions: %s" % pickedSRs) 

    if args.run_toys:
        runToys = True

    if args.background:
        bkgArgs = args.background.split(',')
        if len(bkgArgs) == 2:
            configMgr.SetBkgParName(bkgArgs[0])
            configMgr.SetBkgCorrVal(float(bkgArgs[1]))
            configMgr.SetBkgChlName("")
        elif len(bkgArgs) >= 3 and len(bkgArgs) % 3 == 0:
            for iChan in xrange(len(bkgArgs) / 3):
                iCx = iChan * 3
                configMgr.AddBkgChlName(bkgArgs[iCx])
                configMgr.AddBkgParName(bkgArgs[iCx+1])
                configMgr.AddBkgCorrVal(float(bkgArgs[iCx+2]))

    if args.minos:
        runMinos = True
        
        minosArgs = args.minos.split(",")
        for idx, arg in enumerate(minosArgs):
            if arg.lower() == "all":
                minosArgs[idx] = "all"

        minosPars = ",".join(minosArgs)

    if args.cmd:
        log.info("Python commands executed: %s" % args.cmd)
        exec(args.cmd) ## python execute

    gROOT.SetBatch(not runInterpreter)

    # **** mandatory user-defined configuration ****
    # (copied here because HistFitter authors don't know how to program)

    ##########################
    
    # Setting the parameters of the hypothesis test
    #configMgr.nTOYs=5000
    configMgr.calculatorType=2 # 2=asymptotic calculator
    configMgr.testStatType=3   # 3=one-sided profile likelihood test
    # number of values scanned of signal-strength for upper-limit determination of signal strength.
    configMgr.nPoints=20       
    
    # Give the analysis a name
    configMgr.analysisName = "stop"
    configMgr.outputFileName = "results/%s_Output.root"%configMgr.analysisName
    
    # Define cuts
    configMgr.cutsDict["UserRegion"] = "1."
    
    # Define weights
    configMgr.weights = "1."

    ##########################

    fit_config = FitConfig(args.config)
    fit_config.load_counts()
    systematics = {
        'asymmetric': ['JESUP','JESDOWN'], 
        'symmetric': ['JER']
        }

    testcounts = { 
        'NONE': { 
            'Bkg': {
                'UserRegion': {'normed': 5, 'stats': 25}
                }, 
            'Sig': { 
                'UserRegion': {'normed': 5, 'stats': 25.0 / 4.0}
                }, 
            'data': { 
                'UserRegion': 7.0
                }
            }, 
        'ucb': { 
            'Bkg': { 
                'UserRegion': {'normed': 2 * 0.2 * 5}
                }
            }, 
        'ucs': { 
            'Sig': { 
                'UserRegion': {'normed': 2 * 0.1 * 5}
                }
            }
        }
    test_sym_systematics = { 
        
        }
    test_factory = SampleFactory(testcounts, sym_systematics=['ucb'])
    sig_factory = SampleFactory(testcounts, sym_systematics=['ucs'])

    # Set observed and expected number of events in counting experiment
    ndata     =  7. 	# Number of events observed in data
    lumiError = 0.039 	# Relative luminosity uncertainty
    
    ucs = Systematic(
        "ucs", configMgr.weights, 1.1,0.9, "user","userOverallSys")
    
    
    bkgSample = test_factory.get_sample('Bkg', 'UserRegion')
    sigSample = sig_factory.get_sample('Sig', 'UserRegion')
    # not sure what this does... cryptic way to say "use lumi uncertainty"?
    sigSample.setNormByTheory()     

    dataSample = test_factory.get_data_sample('UserRegion')
    
    # Define top-level
    ana = configMgr.addFitConfig('sb')
    ana.addSamples([bkgSample,sigSample,dataSample])
    ana.setSignalSample(sigSample)
    
    # Define measurement
    meas = ana.addMeasurement(name="NormalMeasurement",lumi=1.0,lumiErr=lumiError)
    meas.addPOI("mu_Sig")
    
    # Add the channel
    chan = ana.addChannel("cuts",["UserRegion"],1,0.,1.)
    ana.setSignalChannels([chan])
    
    # These lines are needed for the user analysis to run
    # Make sure file is re-made when executing HistFactory
    if configMgr.executeHistFactory:
        if os.path.isfile("data/%s.root"%configMgr.analysisName):
            os.remove("data/%s.root"%configMgr.analysisName) 

    # ----- end of stuff copied from config file -----

    #standard execution from now on.
    configMgr.initialize()

    # initialize: set the toy seed
    RooRandom.randomGenerator().SetSeed(configMgr.toySeed)

    #runs Trees->histos and/or histos->workspace according to specifications
    if configMgr.readFromTree or configMgr.executeHistFactory:
        configMgr.executeAll()

    if runFit:
        if len(configMgr.fitConfigs) > 0:
            r = GenerateFitAndPlotCPP(configMgr.fitConfigs[0], configMgr.analysisName, drawBeforeFit, drawAfterFit, drawCorrelationMatrix, drawSeparateComponents, drawLogLikelihood, runMinos, minosPars)
        log.info(" GenerateFitAndPlotCPP(configMgr.fitConfigs[0], configMgr.analysisName, drawBeforeFit, drawAfterFit, drawCorrelationMatrix, drawSeparateComponents, drawLogLikelihood)")
        log.info(" GenerateFitAndPlotCPP(configMgr.fitConfigs[1], configMgr.analysisName, drawBeforeFit, drawAfterFit, drawCorrelationMatrix, drawSeparateComponents, drawLogLikelihood)")
        log.info(" GenerateFitAndPlotCPP(configMgr.fitConfigs[2], configMgr.analysisName, drawBeforeFit, drawAfterFit, drawCorrelationMatrix, drawSeparateComponents, drawLogLikelihood)")
        log.info("   where drawBeforeFit, drawAfterFit, drawCorrelationMatrix, drawSeparateComponents, drawLogLikelihood are booleans")

        pass

    if printLimits:
        for fc in configMgr.fitConfigs:
            if len(fc.validationChannels) > 0:
                raise StandardError("Validation regions should be turned off for setting an upper limit!")
            pass
        configMgr.cppMgr.doUpperLimitAll()
        pass

    if doHypoTests:
        for fc in configMgr.fitConfigs:
            if len(fc.validationChannels) > 0 and not (fc.signalSample is None or 'Bkg' in fc.signalSample):
                raise StandardError("Validation regions should be turned off for doing hypothesis test!")
            pass
        configMgr.cppMgr.doHypoTestAll('results/',doUL)
        pass

    if runToys and configMgr.nTOYs > 0 and doHypoTests == False and printLimits == False and runFit == False:
        configMgr.cppMgr.runToysAll()
        pass

    if runInterpreter:
        from code import InteractiveConsole
        from ROOT import Util
        cons = InteractiveConsole(locals())
        cons.interact("Continuing interactive session... press Ctrl+d to exit")
        pass


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
            raise IOError('no config file')
        with open(file_name) as config_yml: 
            config = yaml.load(config_yml)
        if not 'fit' in config: 
            raise NoFitConfigError()
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
    def __init__(self): 
        super(NoFitConfigError, self).__init__('no config')

if __name__ == "__main__":
    run()
