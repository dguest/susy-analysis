#ifndef CONSTANTS_SCHARM_HH
#define CONSTANTS_SCHARM_HH

#include "constants_physical.hh"

// all jets are above some threshold pt
const float JET_PT_MIN = 50_GeV;

// third jet veto above some pt
const float SIGNAL_JET_3_MAX_PT = 50_GeV; 

// event composite variables
const float MIN_DPHI_JET_MET = 0.4; 
const float SR_MCT_MIN = 150_GeV; 
const float MET_EFF_MIN = 0.25; 

const float M_CC_MIN = 200_GeV; 

namespace crdf {
  const float MCT_MIN = 75_GeV; 
  const float M_LL_MIN = 50_GeV; 
}
namespace crsf { 
  const float LEPTON_PT_MIN = 90_GeV; 
  const float M_LL_MIN = 75_GeV; 
  const float M_LL_MAX = 105_GeV; 
}
namespace cr1l { 
  const float M_T_MIN = 40_GeV;
  const float M_T_MAX = 100_GeV;
}
#endif 
