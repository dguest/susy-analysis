#ifndef REGION_CONFIG_HH
#define REGION_CONFIG_HH

#include <string> 
#include <vector> 
#include "BtagConfig.hh"
#include "systematic_defs.hh"
#include "typedefs.hh"

struct RegionConfig 
{
  std::string name; 
  std::vector<btag::JetTag> jet_tag_requirements; 
  double leading_jet_pt; 
  double met;  
  ull_t required_bits; 
  ull_t veto_bits; 
  syst::Systematic systematic; 
  std::string output_name; 
};

#endif 
