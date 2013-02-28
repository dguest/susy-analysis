#!/usr/bin/env bash

USE_LIBS="
 SUSYTools ApplyJetCalibration CalibrationDataInterface egammaAnalysisUtils\
 egammaEvent GoodRunsLists JetResolution JetSelectorTools JetUncertainties\
 MissingETUtility MuonEfficiencyCorrections MuonMomentumCorrections\
 ObjectSelectorCore PileupReweighting ReweightUtils\
 TrigRootAnalysis TauCorrections ApplyJetResolutionSmearing\
 TrigMuonEfficiency ElectronEfficiencyCorrection PATCore"

USE_HEADER="SUSYTools MissingETUtility egammaAnalysisUtils GoodRunsLists\
 CalibrationDataInterface"


if [[ $1 == '-l' ]]
then
    for l in $USE_LIBS
    do
	echo -n -L$2/${l}/StandAlone -Wl,-rpath,$2/${l}/StandAlone -l${l}\ 
    done
elif [[ $1 == '-i' ]]
then
    for i in $USE_HEADER
    do 
	echo -n $2/$i\ 
    done
else 
    echo "ERROR: $0 needs flag -i or -l" >&2 
    false 
fi

