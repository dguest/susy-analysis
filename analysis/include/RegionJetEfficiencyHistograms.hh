#ifndef REGION_JET_EFFICIENCY_HISTOGRAMS_HH
#define REGION_JET_EFFICIENCY_HISTOGRAMS_HH

#include "IRegionHistograms.hh"
#include <map>
#include <string> 
#include <boost/noncopyable.hpp>
#include "Flavor.hh"

namespace jeteff { 
  const double MAX_PT_MEV = 1e6; 
}

class RegionConfig; 
class RegionEventFilter; 
class Histogram; 
namespace H5 { 
  class CommonFG; 
}
class Jet; 

class Wt2Hist: boost::noncopyable { 
public: 
  Wt2Hist(int bins, double low, double high, std::string units); 
  ~Wt2Hist(); 
  void fill(double value, double wt); 
  void write_to(H5::CommonFG&, std::string basename) const; 
private: 
  Histogram *m_hist; 
  Histogram *m_hist_wt2; 
}; 

class JetEfficiencyHists: boost::noncopyable { 
public: 
  JetEfficiencyHists(double max_pt_mev); 
  ~JetEfficiencyHists(); 
  void fill(const Jet& jet, double weight = 1.0); 
  void write_to(H5::CommonFG&, std::string group_name) const;
private: 
  Wt2Hist* m_jet_pt_all; 
  Wt2Hist* m_jet_pt_loose; 
  Wt2Hist* m_jet_pt_medium; 
  Wt2Hist* m_jet_pt_antiloose; 
}; 

class RegionJetEfficiencyHistograms: 
  public IRegionHistograms
{
public: 
  RegionJetEfficiencyHistograms(const RegionConfig& config, 
				const unsigned hist_builder_flags = 0); 
  ~RegionJetEfficiencyHistograms(); 
  virtual void fill(const EventObjects& objects); 
  virtual void write_to(H5::CommonFG&) const; 
private: 
  typedef RegionJetEfficiencyHistograms ThisClass; 
  ThisClass& operator=(const ThisClass&) = delete; 
  RegionJetEfficiencyHistograms(const ThisClass&) = delete; 
  const unsigned m_build_flags; 
  const RegionConfig* m_region_config; 
  const RegionEventFilter* m_event_filter; 
  
  std::map<Flavor, JetEfficiencyHists*> m_jet_pt_hists; 
}; 

#endif 
