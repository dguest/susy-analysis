#ifndef CUT_BITS_H
#define CUT_BITS_H

namespace cutflag { 
  const unsigned verbose          = 1u << 0; 
  const unsigned is_data          = 1u << 1; 
  const unsigned is_signal        = 1u << 2;  
  const unsigned use_low_pt_jets  = 1u << 3; 
  const unsigned debug_susy       = 1u << 4; 
  const unsigned raw_evt_info     = 1u << 5; 
  const unsigned get_branches     = 1u << 6; 
  const unsigned save_ratios      = 1u << 7; 
  const unsigned save_flavor_wt   = 1u << 8; 
  const unsigned use_met_reffinal = 1u << 9; 
}

#endif //CUT_BITS_H
