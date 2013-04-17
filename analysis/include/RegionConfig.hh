#ifndef REGION_CONFIG_HH
#define REGION_CONFIG_HH

#include <string> 
#include <vector> 
#include "BtagConfig.hh"
#include "systematic_defs.hh"
#include "typedefs.hh"

namespace reg { 
  enum Type { CONTROL, SIGNAL, VALIDATION }; 
  enum Hists { HISTMILL, KINEMATIC_STAT }; 
  // region event filter flags
  const unsigned no_extra_jets = 1u << 0; 
  const unsigned throw_for_fun = 1u << 1; 
  const unsigned mu_met        = 1u << 2; 
}

struct RegionConfig 
{
  std::string name; 
  std::vector<btag::JetTag> jet_tag_requirements; 
  double leading_jet_pt; 
  double met;  
  ull_t required_bits; 
  ull_t veto_bits; 
  unsigned region_bits; 
  syst::Systematic systematic; 
  std::string output_name; 
  reg::Type type; 
  reg::Hists hists; 
  RegionConfig(); 
};

#endif 
