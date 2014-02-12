#ifndef REGION_CONFIG_HH
#define REGION_CONFIG_HH

#include <string> 
#include <vector> 
#include "btag_enums.hh"
#include "systematic_defs.hh"
#include "typedefs.hh"

namespace reg { 
  enum Type { CONTROL, SIGNAL, VALIDATION }; 
  enum Hists { HISTMILL, KINEMATIC_STAT, TAG_EFFICIENCY, BOSON_PT }; 
  enum BosonPtCorrection {NO_PT_CORRECTION, MARKS}; 
  enum class Selection { SIGNAL, CR_1L, CR_SF, CR_OF, NONE, ERROR}; 
  // region event filter flags
  const unsigned no_extra_jets = 1u << 0; 
  const unsigned throw_for_fun = 1u << 1; 
  const unsigned mu_met        = 1u << 2; 
  const unsigned electron_jet  = 1u << 3; 
}

struct RegionConfig 
{
  std::string name; 
  reg::Selection selection; 
  std::vector<btag::OperatingPoint> jet_tag_requirements; 
  btag::Assignment jet_tag_assignment; 
  double leading_jet_pt; 
  double met;  
  unsigned region_bits; 
  syst::Systematic systematic; 
  std::string output_name; 
  reg::Type type; 
  reg::Hists hists; 
  std::string mc_mc_jet_reweight_file; 
  btag::Tagger tagger; 
  reg::BosonPtCorrection boson_pt_correction; 
  RegionConfig(); 
};

#endif 
