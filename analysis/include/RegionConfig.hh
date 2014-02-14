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
  enum class Selection { 
    SIGNAL, CR_1L, CR_SF, CR_DF, ERROR, QUALITY_EVENT}; 
}

struct RegionConfig 
{
  std::string name; 
  reg::Selection selection; 
  std::vector<btag::OperatingPoint> jet_tag_requirements; 
  btag::Assignment jet_tag_assignment; 
  double leading_jet_pt; 
  double met;  
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
