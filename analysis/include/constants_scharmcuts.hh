#ifndef CONSTANTS_SCHARM_HH
#define CONSTANTS_SCHARM_HH

#include "constants_physical.hh"

// third jet veto above some pt
const float SIGNAL_JET_3_MAX_PT = 50*GeV; 

// event composite variables
const float MIN_DPHI_JET_MET = 0.4; 
const float SR_MCT_MIN = 150*GeV; 
const float MET_EFF_MIN = 0.25; 

const float M_CC_MIN = 200*GeV; 

const float CRDF_MCT_MIN = 75*GeV; 
const float CRDF_M_LL_MIN = 50*GeV; 

const float CRSF_LEPTON_PT_MIN = 90*GeV; 
const float CRSF_M_LL_MIN = 75*GeV; 
const float CRSF_M_LL_MAX = 105*GeV; 

const float CR1L_M_T_MIN = 40*GeV;
const float CR1L_M_T_MAX = 100*GeV;

#endif 
