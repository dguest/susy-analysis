#ifndef DISTILLER_CONSTANTS_HH
#define DISTILLER_CONSTANTS_HH

#include <cstddef>

const static float MIN_DPHI_JET_MET = 0.4; 
const static float REMOVE_JET_CONE = 0.2; 
const static float REMOVE_EL_CONE = 0.4; 
const static float REMOVE_MU_CONE = 0.4; 

const static size_t N_JETS_TO_SAVE = 4; 
const static size_t N_SR_JETS = 3; 

const static float VETO_ELECTRON_REL_ISOLATION = 0.1; 
const static float CONTROL_ELECTRON_PT = 20e3; 

const static float VETO_MUON_ISOLATION = 1.8e3; 
const static float CONTROL_MUON_PT = 20e3; 

const static float Z_MASS = 91.188e3; 
const static float Z_MASS_WINDOW = 15.0e3; 

const static float BOSON_PT_OVERLAP_CUT_MEV = 40.0e3; 

#endif 
