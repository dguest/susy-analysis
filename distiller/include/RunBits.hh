#ifndef RUN_BITS_H
#define RUN_BITS_H

namespace cutflag { 
  const unsigned verbose          = 1u << 0; 
  const unsigned is_data          = 1u << 1; 
  const unsigned save_all_events  = 1u << 2;  
  const unsigned use_low_pt_jets  = 1u << 3; 
  const unsigned debug_susy       = 1u << 4; 
  const unsigned raw_evt_info     = 1u << 5; 
  const unsigned get_branches     = 1u << 6; 
  const unsigned save_ratios      = 1u << 7; 
  // const unsigned save_flavor_wt   = 1u << 8; 
  // const unsigned use_met_reffinal = 1u << 9; 
  const unsigned no_jet_recal     = 1u << 10; 
  const unsigned is_atlfast       = 1u << 11; 
  const unsigned debug_cutflow    = 1u << 12; 
  // const unsigned save_truth       = 1u << 13; 
  const unsigned jetfitter_charm  = 1u << 14; 
  const unsigned mv3              = 1u << 15; 

  const unsigned spartid          = 1u << 22; 
  const unsigned truth            = 1u << 23; 

  const unsigned can_set_at_runtime = (spartid | truth); 

  const unsigned unleash_sharktopus = 1u << 31; 
}

#endif //RUN_BITS_H
