#ifndef REGION_CONFIG_HH
#define REGION_CONFIG_HH

#include <string>
#include <vector>
#include <utility>
#include "btag_enums.hh"
#include "systematic_defs.hh"
#include "region_enums.hh"

struct RegionConfig
{
  std::string name;
  reg::Selection selection;
  double leading_jet_pt;
  double second_jet_pt;
  double first_lepton_pt;
  double mct;
  double met;
  long max_signal_jets;
  syst::Systematic systematic;
  std::string output_name;
  reg::Type type;
  reg::Hists hists;
  btag::Tagger tagger;
  reg::BosonPtCorrection boson_pt_correction;
  reg::Stream stream;
  bool save_wt2;
  std::vector<std::pair<long, long> > veto_events;
  RegionConfig();
};

#endif
