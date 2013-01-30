#ifndef EVENT_BITS_HH
#define EVENT_BITS_HH

// NOTE: This is (to be) parsed by a python function to extract bit 
//       definitions. Please restrict the below code to bit defs. 

namespace pass { 
  const unsigned grl                = 1u << 2; 
  const unsigned trigger            = 1u << 3; 
  const unsigned lar_error          = 1u << 4; 
  const unsigned core               = 1u << 5; 
  const unsigned jet_clean          = 1u << 6; 
  const unsigned vxp_gt_4trk        = 1u << 7; 
  const unsigned leading_jet        = 1u << 8; 
  const unsigned met                = 1u << 9; 
  const unsigned n_jet              = 1u << 10; 
  const unsigned dphi_jetmet_min    = 1u << 11; 
  // const unsigned dphi_jetmet_sum    = 1u << 12; 
  const unsigned lepton_veto        = 1u << 13; 
  const unsigned ctag_mainz         = 1u << 14; 

  const unsigned jet1_anti_b        = 1u << 15; 
  const unsigned jet1_anti_u_medium = 1u << 16; 
  const unsigned jet1_anti_u_tight  = 1u << 17; 
  const unsigned jet2_anti_b        = 1u << 18; 
  const unsigned jet2_anti_u_medium = 1u << 19; 
  const unsigned jet2_anti_u_tight  = 1u << 20; 
  const unsigned jet3_anti_b        = 1u << 21; 
  const unsigned jet3_anti_u_medium = 1u << 22; 
  const unsigned jet3_anti_u_tight  = 1u << 23; 
}

#endif
