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
  const ull_t bad_tile           = 1ULL << 15; 
  const ull_t jet_chf            = 1ULL << 16; 
  const ull_t tile_error         = 1ULL << 17; 
  const ull_t tile_trip          = 1ULL << 18; 

  // requirements for control regions / cutflow
  const ull_t met_eff            = 1ULL << 20; 
  const ull_t control_muon       = 1ULL << 21; 
  const ull_t control_electron   = 1ULL << 22;    
  const ull_t sr_mct             = 1ULL << 23; 
  const ull_t crof_mct           = 1ULL << 24; 
  const ull_t m_cc               = 1ULL << 25;
  const ull_t tagged             = 1ULL << 26; 
  const ull_t double_tagged      = 1ULL << 27; 

  // saved in the ntuple
  const ull_t ossf               = 1ULL << 28;
  const ull_t osdf               = 1ULL << 29;
  const ull_t event_quality      = 1ULL << 30; 

  // only for the cutflow
  const ull_t cutflow_leading    = 1ULL <<  31; 
  const ull_t cutflow_met        = 1ULL <<  32; 
  const ull_t cutflow_jet2      = 1ULL <<  33; 
  const ull_t cutflow_jet3      = 1ULL <<  34; 

  // --- trigger ---
  // met
  const ull_t EF_xe80_tclcw_loose   = 1ULL << 35;
  const ull_t EF_xe80T_tclcw_loose  = 1ULL << 36;
  const ull_t EF_xe80_tclcw_tight   = 1ULL << 37;
  // dilep triggers
  const ull_t EF_mu18_tight_mu8_EFFS  = 1ULL << 38;
  const ull_t EF_2e12Tvh_loose1       = 1ULL << 39;
  // single lepton triggers
  const ull_t EF_mu24i_tight     = 1ULL << 40; 
  const ull_t EF_e24vhi_medium1  = 1ULL << 41;
  const ull_t EF_mu36_tight      = 1ULL << 42;
  const ull_t EF_e60_medium1     = 1ULL << 43;

  // compound trigger
  const ull_t met_trigger        = 1ULL << 44; 
  const ull_t dilep_trigger      = 1ULL << 45; 
  const ull_t single_lep_trigger = 1ULL << 46; 
  
}

#endif
