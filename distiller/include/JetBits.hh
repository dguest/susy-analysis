#ifndef JET_BITS_HH
#define JET_BITS_HH

namespace jetbit { 
  const unsigned preselection         = 1u << 0; 
  const unsigned low_pt               = 1u << 1; 
  // const unsigned el_overlap           = 1u << 2; 
  const unsigned signal               = 1u << 3; 
  const unsigned leading              = 1u << 4; 
  const unsigned high_eta             = 1u << 5; 
  const unsigned taggable_eta         = 1u << 6; 
  const unsigned signal_pt            = 1u << 7; 
  const unsigned cnn_loose_anti_u     = 1u << 8; 
  const unsigned cnn_medium_anti_u    = 1u << 9; 
  const unsigned cnn_tight_anti_u     = 1u << 10; 
  const unsigned cnn_anti_b           = 1u << 11; 
  const unsigned jfc_loose_anti_u     = 1u << 12; 
  const unsigned jfc_medium_anti_u    = 1u << 13; 
  const unsigned jfc_tight_anti_u     = 1u << 14; 
  const unsigned jfc_anti_b           = 1u << 15; 
}

#endif 
