#ifndef EVENT_BITS_HH
#define EVENT_BITS_HH

// NOTE: This is (to be) parsed by a python function to extract bit 
//       definitions. Please restrict the below code to bit defs. 

#include "typedefs.hh"

namespace pass { 
  const ull_t grl                = 1ULL <<  0; 
  const ull_t trigger            = 1ULL <<  1; 
  const ull_t lar_error          = 1ULL <<  2; 
  const ull_t core               = 1ULL <<  3; 
  const ull_t jet_clean          = 1ULL <<  4; 
  const ull_t vxp_gt_4trk        = 1ULL <<  5; 
  const ull_t leading_jet        = 1ULL <<  6; 
  const ull_t met                = 1ULL <<  7; 
  const ull_t n_jet              = 1ULL <<  8; 
  const ull_t dphi_jetmet_min    = 1ULL <<  9; 
  const ull_t electron_veto      = 1ULL << 10; 
  const ull_t muon_veto          = 1ULL << 11; 
  const ull_t ctag_mainz         = 1ULL << 12; 

  // requirements for control regions
  const ull_t control_muon       = 1ULL << 15; 
  const ull_t control_electron   = 1ULL << 16;    
  const ull_t multi_lepton       = 1ULL << 17; 
  const ull_t os_zmass_el_pair   = 1ULL << 18; 
  const ull_t os_zmass_mu_pair   = 1ULL << 19; 

  // must be set in the Analysis routines
  const ull_t mttop              = 1ULL << 21; 
  // const ull_t j2_anti_b_opt      = 1ULL << 22; 
  // const ull_t j2_anti_u_opt      = 1ULL << 23; 
  // const ull_t j3_anti_b_opt      = 1ULL << 24; 
  // const ull_t j3_anti_u_opt      = 1ULL << 25; 

  // late additons to event cleaning 
  const ull_t tile_error         = 1ULL <<  35; 
  const ull_t tile_trip          = 1ULL <<  36; 

  // only for the cutflow
  const ull_t cutflow_leading    = 1ULL <<  37; 
  const ull_t cutflow_met        = 1ULL <<  38; 
  const ull_t cutflow_tag_1      = 1ULL <<  39; 
  const ull_t cutflow_tag_2      = 1ULL <<  40; 
  
}

#endif
