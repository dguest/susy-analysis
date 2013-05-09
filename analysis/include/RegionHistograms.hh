#ifndef REGION_HISTOGRAMS_HH
#define REGION_HISTOGRAMS_HH

#include <boost/noncopyable.hpp>
#include <vector> 

#include "typedefs.hh"
#include "RegionConfig.hh"
#include "RegionEventFilter.hh"
#include "IRegionHistograms.hh"

struct EventObjects; 
class Jet1DHists; 
class TruthJetHists; 
class Histogram; 

namespace H5 { 
  class CommonFG; 
}

namespace { 
  const int N_BINS = 100; 
}; 

class RegionHistograms: public boost::noncopyable, public IRegionHistograms
{
public: 
  RegionHistograms(const RegionConfig& config, const unsigned build = 0); 
  ~RegionHistograms(); 
  virtual void fill(const EventObjects& objects);
  virtual void write_to(H5::CommonFG&) const; 
  void write_to(std::string file_name = "") const; 
private: 
  void add_cjet_rank(); 
  const RegionConfig m_region_config; 
  const RegionEventFilter m_event_filter; 
  const unsigned m_build_flags; 

  std::vector<Jet1DHists*> m_jet_hists; 

  Histogram* m_met; 
  Histogram* m_alt_met; 
  Histogram* m_min_dphi; 
  Histogram* m_mttop; 
  Histogram* m_n_good_jets; 

  Histogram* m_htx; 

  Histogram* m_cjet_rank; 

  std::vector<TruthJetHists*> m_jet_truth_hists; 
}; 

#endif 
