#ifndef DISTILLER_CONSTANTS_HH
#define DISTILLER_CONSTANTS_HH

#include <cstddef>

const static float MIN_DPHI_JET_MET = 0.4; 
const static float REMOVE_JET_CONE = 0.2; 
const static float REMOVE_EL_CONE = 0.4; 
const static float REMOVE_MU_CONE = 0.4; 

const static size_t N_JETS_TO_SAVE = 4; 
const static size_t N_SR_JETS = 3; 

const static float Z_MASS = 91.188e3; 
const static float Z_MASS_WINDOW = 15.0e3; 

const static size_t ENERGY_WEIGHTED_TIME_NJET = 2; 
const static double ENERGY_WEIGHTED_TIME_MAX = 5.0; 

const static size_t CHF_CHECK_NJET = 2; 

#endif 
