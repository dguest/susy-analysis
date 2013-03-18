#ifndef DISTILLER_CONSTANTS_HH
#define DISTILLER_CONSTANTS_HH

const static float MIN_DPHI_JET_MET = 0.4; 
const static float REMOVE_JET_CONE = 0.2; 
const static float REMOVE_EL_CONE = 0.4; 
const static float REMOVE_MU_CONE = 0.4; 

const static float COMBNN_LOG_CB_CUT = -1.0; 
const static float COMBNN_MED_LOG_CU_CUT = -0.82; 
const static float COMBNN_TIGHT_LOG_CU_CUT = 1.0; 

const static int N_JETS_TO_SAVE = 4; 

const static float VETO_ELECTRON_REL_ISOLATION = 0.1; 
const static float CONTROL_ELECTRON_PT = 20e3; 

const static float VETO_MUON_ISOLATION = 1.8e3; 
const static float CONTROL_MUON_PT = 20e3; 

#endif 
