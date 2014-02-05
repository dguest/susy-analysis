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
  const ull_t energy_wt_time     = 1Ull << 12; 
  const ull_t bad_muon           = 1ULL << 13; 
  const ull_t cosmic_muon        = 1ULL << 14; 

  // requirements for control regions
  const ull_t control_muon       = 1ULL << 15; 
  const ull_t control_electron   = 1ULL << 16;    
  // const ull_t multi_lepton       = 1ULL << 17; 
  const ull_t os_zmass_el_pair   = 1ULL << 18; 
  const ull_t os_zmass_mu_pair   = 1ULL << 19; 

  // must be set in the Analysis routines
  // const ull_t mttop              = 1ULL << 21; 

  // late addition 
  const ull_t met_eff            = 1ULL << 21; 
  const ull_t mu_met             = 1ULL << 22; 

  // bad tile veto
  const ull_t bad_tile_stmet       = 1ULL << 23; 
  const ull_t bad_tile_mumet       = 1ULL << 24; 
  // const ull_t bad_tile_stmet_up    = 1ULL << 25; 
  // const ull_t bad_tile_mumet_up    = 1ULL << 26; 
  // const ull_t bad_tile_stmet_down  = 1ULL << 27; 
  // const ull_t bad_tile_mumet_down  = 1ULL << 28; 

  // jet charged fraction 
  const ull_t jet_chf            = 1ULL << 30; 

  // late additons to event cleaning 
  const ull_t tile_error         = 1ULL <<  35; 
  const ull_t tile_trip          = 1ULL <<  36; 

  // only for the cutflow
  const ull_t cutflow_leading    = 1ULL <<  37; 
  const ull_t cutflow_met        = 1ULL <<  38; 
  const ull_t cutflow_jet2      = 1ULL <<  39; 
  const ull_t cutflow_jet3      = 1ULL <<  40; 

  // more trigger
  const ull_t met_trigger        = 1ULL << 41; 
  const ull_t mu_trigger         = 1ULL << 42; 
  const ull_t el_trigger         = 1ULL << 43; 
  
  // tagging 
  const ull_t tagged               = 1ULL << 45; 
  const ull_t double_tagged        = 1ULL << 46; 
}

#endif
