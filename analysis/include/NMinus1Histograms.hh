#ifndef N_MINUS_ONE_HISTOGRAMS_HH
#define N_MINUS_ONE_HISTOGRAMS_HH

#include "constants_physical.hh"
#include "IRegionHistograms.hh"
#include "NMinusHist.hh"

#include <vector>
#include <string>

struct Axis;
class RegionConfig;
class ISelection;

namespace nminus {
  const size_t N_BINS = 100;
  const double MAX_ENERGY = 1_TeV;
  // for now the upstream filters remove events with less than 2 signal
  // jets (may break code here to allow fewer)
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
  const std::string MCT = "mass_ct";
  const std::string MET_EFF = "met_eff";
  const std::string MCC = "mass_cc";
  const std::string LLPT = "leading_lepton_pt";
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

};

class NMinus1Histograms:
  public IRegionHistograms
{
public:
  NMinus1Histograms(const RegionConfig& config,
		    const unsigned build = 0);
  NMinus1Histograms(NMinus1Histograms&) = delete;
  ~NMinus1Histograms();
  NMinus1Histograms& operator=(NMinus1Histograms&) = delete;
  virtual void fill(const EventObjects& objects);
  virtual void write_to(H5::CommonFG&) const;
private:
  const RegionConfig* m_region_config;
  const ISelection* m_selection;
  const unsigned m_build_flags;
  bool m_make_lepton_plots;
  bool m_make_dilep_plots;

  std::vector<nminus::NMinusHist> m_hists;
};


#endif
