#ifndef REGION_CONFIG_HH
#define REGION_CONFIG_HH

#include <string>
#include <vector>
#include "btag_enums.hh"
#include "systematic_defs.hh"
#include "region_enums.hh"

struct RegionConfig
{
  std::string name;
  reg::Selection selection;
  std::vector<btag::OperatingPoint> jet_tag_requirements;
  btag::Assignment jet_tag_assignment;
  double leading_jet_pt;
  double second_jet_pt;
  double met;
  syst::Systematic systematic;
  std::string output_name;
  reg::Type type;
  reg::Hists hists;
  btag::Tagger tagger;
  reg::BosonPtCorrection boson_pt_correction;
  reg::Stream stream;
  RegionConfig();
};

#endif
