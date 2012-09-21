#ifndef EVENT_BITS_HH
#define EVENT_BITS_HH

// NOTE: This is (to be) parsed by a python function to extract bit 
//       definitions. Please restrict the below code to bit defs. 

namespace evt { 
  const unsigned pass_leading_pt  = 1u << 0; 
  const unsigned pass_met         = 1u << 1; 
  const unsigned pass_el_veto     = 1u << 2; 
  const unsigned pass_mu_veto     = 1u << 3; 
  const unsigned pass_jetmet_dphi = 1u << 4; 
  const unsigned pass_mainz_ctag  = 1u << 5; 
}

#endif
