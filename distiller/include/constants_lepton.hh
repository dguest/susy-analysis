#ifndef LEPTON_CONSTANTS_HH
#define LEPTON_CONSTANTS_HH

#include "constants.hh"

static const float ELECTRON_ET_CUT = 10*GeV; 
static const float ELECTRON_ETA_CUT = 2.47; 
static const float SIGNAL_ELECTRON_ET_CUT = 25000.0; 
static const float SIGNAL_ELECTRON_ISO_CUT = 0.1; 
static const float SIGNAL_ELECTRON_D0_CUT = 1.0; 
static const float SIGNAL_ELECTRON_Z0_CUT = 2.0; 
// const static float VETO_ELECTRON_REL_ISOLATION = 0.1; 
const static float VETO_ELECTRON_REL_ISOLATION = INFINITY; 
const static float CONTROL_ELECTRON_PT = 20e3; 

static const float MUON_PT_CUT = 10*GeV; 
static const float MUON_ETA_CUT = 2.4; 
// const static float VETO_MUON_ISOLATION = 1.8e3; 
const static float VETO_MUON_ISOLATION = INFINITY; 
const static float CONTROL_MUON_PT = 10e3; 

#endif 
