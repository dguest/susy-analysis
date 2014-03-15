#ifndef LEPTON_CONSTANTS_HH
#define LEPTON_CONSTANTS_HH

#include "constants.hh"

static const float ELECTRON_ET_CUT = 7*GeV; 
static const float ELECTRON_ETA_CUT = 2.47; 

// use these?
static const float SIGNAL_ELECTRON_ET_CUT = 25000.0; 
static const float SIGNAL_ELECTRON_ISO_CUT = 0.1; 
static const float SIGNAL_ELECTRON_D0_CUT = 1.0; 
static const float SIGNAL_ELECTRON_Z0_CUT = 2.0; 

const static float CONTROL_ELECTRON_REL_ISOLATION = 0.1; 
const static float CONTROL_ELECTRON_PT = 25e3; 

static const float MUON_PT_CUT = 6*GeV; 
static const float MUON_ETA_CUT = 2.4; 

const static float CONTROL_MUON_ISOLATION = 1.8*GeV; 
const static float CONTROL_MUON_PT = 20e3; 

const static float SINGLE_EL_TRIGGER_PT_MIN = 25*GeV;
const static float DOUBLE_EL_TRIGGER_PT_MIN = 25*GeV;
const static float SINGLE_MU_TRIGGER_PT_MIN = 25*GeV;
const static float DOUBLE_MU_TRIGGER_PT_MIN = 20*GeV;

#endif 
