#ifndef HIST_BUILDER_FLAGS_H
#define HIST_BUILDER_FLAGS_H

namespace buildflag { 
  const unsigned verbose    = 1u << 0; 
  const unsigned fill_truth = 1u << 1; 

  // flags to turn on/off optimizations
  const unsigned leading_jet_btag = 1u << 10; 
  const unsigned mttop            = 1u << 11; 
  const unsigned disable_c_tags   = 1u << 12; 
}

#endif
