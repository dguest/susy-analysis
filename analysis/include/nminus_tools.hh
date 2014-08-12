#ifndef SELECTION_FACTORY_HH
#define SELECTION_FACTORY_HH

#include "Window.hh"

class ISelection;
class RegionConfig;

#include <map>
#include <string>

namespace nminus {
  const int MIN_SIGNAL_JETS = 2;
  // signal jet hist shows from -0.5 to (SJET_RANGE - 0.5)
  const int SJET_RANGE = 15;
  // this number or higher means "infinite"
  const int SJET_INF_THRESHOLD = 100;

  // all units are in MeV
  const std::string EUNIT = "MeV";

  // names for internal use (and saving)
  const std::string MET = "met";
  const std::string DPHI = "jetmet_dphi";
  const std::string LEP_DPHI = "lepmet_dphi";
  const std::string MCT = "mass_ct";
  const std::string MCT_UNCORR = "mass_ct_uncorr";
  const std::string MET_EFF = "met_eff";
  const std::string MCC = "mass_cc";
  const std::string LLPT = "leading_lepton_pt";
  const std::string SLPT = "second_lepton_pt";
  const std::string MT = "mass_t";
  const std::string MLL = "mass_ll";
  const std::string NSJET = "n_signal_jets";
  // used by the jet naming routines
  const std::string FTL = "flavor_truth_label";

  // naming functions
  std::string jeta(int jn);
  std::string jpt(int jn);
  std::string jantib(int jn);
  std::string jantiu(int jn);
  std::string jftl(int jn);

  // ACHTUNG: figure out a strategy to plot the SF
  // const std::string TAGSF = "tag_sf";
  // std::string jtagsf(int jn);

  ISelection* selection_factory(const RegionConfig& config);
  std::map<std::string, Window> get_windows(const RegionConfig&);
}

#endif
