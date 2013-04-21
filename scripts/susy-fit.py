#!/usr/bin/env python2.7
import ROOT
ROOT.PyConfig.IgnoreCommandLineOptions = True

from ROOT import gROOT,gSystem,gDirectory,RooAbsData,RooRandom,RooWorkspace
gSystem.Load("/data2/dguest/blackbox/HistFitter/lib/libSusyFitter.so")
from ROOT import ConfigMgr
gROOT.Reset()
import os
import argparse

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
from os.path import dirname
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
    parser.add_argument('input_counts')
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
                continue

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

    with open(args.input_counts) as inputs_yml: 
        counts = yaml.load(inputs_yml)

    # Set observed and expected number of events in counting experiment
    ndata     =  7. 	# Number of events observed in data
    nbkg      =  5.	 	# Number of predicted bkg events
    nsig      =  5.  	# Number of predicted signal events
    nbkgErr   =  1.  	# (Absolute) Statistical error on bkg estimate
    nsigErr   =  2.  	# (Absolute) Statistical error on signal estimate
    lumiError = 0.039 	# Relative luminosity uncertainty
    
    ucb = Systematic(name="ucb", 
                     nominal=configMgr.weights, # why not 1? 
                     high=1.2,
                     low=0.8, 
                     type="user",   # this should be "user" if not creating tree
                     method="userOverallSys") # no idea what this does
    ucs = Systematic("ucs", configMgr.weights, 1.1,0.9, "user","userOverallSys")
    
    
    ##########################
    
    # Setting the parameters of the hypothesis test
    #configMgr.nTOYs=5000
    configMgr.calculatorType=2 # 2=asymptotic calculator
    configMgr.testStatType=3   # 3=one-sided profile likelihood test
    # number of values scanned of signal-strength for upper-limit determination of signal strength.
    configMgr.nPoints=20       
    
    ##########################
    
    # Give the analysis a name
    configMgr.analysisName = "stop"
    configMgr.outputFileName = "results/%s_Output.root"%configMgr.analysisName
    
    # Define cuts
    configMgr.cutsDict["UserRegion"] = "1."
    
    # Define weights
    configMgr.weights = "1."
    
    # Define samples
    bkgSample = Sample(name="Bkg",  # used to position in the workspace
                       color=kGreen-9)
    bkgSample.setStatConfig(True)
    bkgSample.buildHisto(binValues=[nbkg],
                         region="UserRegion", # also used to position in WS
                         var="cuts")          # no fucking clue
    bkgSample.buildStatErrors(binStatErrors=[nbkgErr],
                              region="UserRegion",
                              var="cuts")
    bkgSample.addSystematic(ucb)
    
    sigSample = Sample("Sig",kPink)
    sigSample.setNormFactor("mu_Sig",1.,0.,10.)
    sigSample.setStatConfig(True)
    # not sure what this does... cryptic way to say "use lumi uncertainty"?
    sigSample.setNormByTheory()     
    
    sigSample.buildHisto([nsig],"UserRegion","cuts")
    sigSample.buildStatErrors([nsigErr],"UserRegion","cuts")
    sigSample.addSystematic(ucs)
    
    dataSample = Sample("Data",kBlack)
    dataSample.setData()
    dataSample.buildHisto([ndata],"UserRegion","cuts")
    
    # Define top-level
    ana = configMgr.addTopLevelXML("SPlusB")
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

if __name__ == "__main__":
    run()
