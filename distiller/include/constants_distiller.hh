#ifndef DISTILLER_CONSTANTS_HH
#define DISTILLER_CONSTANTS_HH

// home to all the multi-object constants 
// (specific files exist for jets and leptons)

#include <cstddef>
#include "constants.hh"

const static float DPHI_JET_MET_MIN = 0.4; 
const static float DPHI_JET_MET_NJET = 3; 

// overlap removal stuff (should be pretty standard)
const static float REMOVE_JET_CONE = 0.2; 
const static float REMOVE_EL_CONE = 0.4; 
const static float REMOVE_MU_CONE = 0.4; 

const static size_t N_JETS_TO_SAVE = 4; 
const static size_t N_SR_JETS = 2; 

const static size_t ENERGY_WEIGHTED_TIME_NJET = 2; 
const static double ENERGY_WEIGHTED_TIME_MAX = 5.0; 

const static size_t CHF_CHECK_NJET = 2; 

const float CUTFLOW_MET            = 150*GeV; 
const float CUTFLOW_JET1_PT = 130*GeV; 
const float CUTFLOW_JET2_PT  = 50*GeV; 
const float CUTFLOW_JET3_PT_VETO = 50*GeV;

const float FILTER_LEADING_JET_PT = 120*GeV; 
const float FILTER_MET            = 120*GeV; 

const float MET_EFF_MIN = 0.25; 

const float SR_MCT_MIN = 150*GeV; 
const float CROF_MCT_MIN = 75*GeV; 
const float M_CC_MIN = 200*GeV; 
const float M_LL_MIN = 50*GeV; 

#endif 
