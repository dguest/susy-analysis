#ifndef N_MINUS_ONE_HISTOGRAMS_HH
#define N_MINUS_ONE_HISTOGRAMS_HH

#include "constants_physical.hh"
#include "IRegionHistograms.hh"
#include "NMinusHist.hh"

#include <vector>
#include <string>

class RegionConfig;
class ISelection;
class EventVeto;

namespace nminus {
  const size_t N_BINS = 100;
  const double MAX_ENERGY = 1_TeV;
  // key for saving event weight (used in axis label / save)
  const std::string EVT_WT = "event_weight";
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
  nminus::NMinusHist* m_weight_hist;
  EventVeto* m_event_veto;
};


#endif
