#!/usr/bin/env bash

# USE_LIBS="SUSYTools CalibrationDataInterface MuonEfficiencyCorrections "

USE_LIBS="ApplyJetCalibration CalibrationDataInterface egammaAnalysisUtils\
 egammaEvent GoodRunsLists JetResolution JetSelectorTools JetUncertainties\
 MissingETUtility MuonEfficiencyCorrections MuonMomentumCorrections\
 ObjectSelectorCore PileupReweighting ReweightUtils SUSYTools\
 TrigRootAnalysis"


for l in $USE_LIBS
do
    echo -n -L$1/${l}/StandAlone -Wl,-rpath,$1/${l}/StandAlone -l${l}\ 
done

