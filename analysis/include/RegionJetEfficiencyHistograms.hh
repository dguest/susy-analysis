#ifndef REGION_JET_EFFICIENCY_HISTOGRAMS_HH
#define REGION_JET_EFFICIENCY_HISTOGRAMS_HH

#include "IRegionHistograms.hh"
#include <map>
#include <string> 
#include <boost/noncopyable.hpp>
#include "Flavor.hh"

class RegionConfig; 
class RegionEventFilter; 
class Histogram; 
namespace H5 { 
  class CommonFG; 
}
class Jet; 

class JetEfficiencyHists: boost::noncopyable { 
public: 
  JetEfficiencyHists(double max_pt_mev); 
  ~JetEfficiencyHists(); 
  void fill(const Jet& jet); 
  void write_to(H5::CommonFG&, std::string group_name) const;
private: 
  Histogram* m_jet_pt_all; 
  Histogram* m_jet_pt_loose; 
  Histogram* m_jet_pt_medium; 
  Histogram* m_jet_pt_antiloose; 
}; 

class RegionJetEfficiencyHistograms: 
  public IRegionHistograms
{
  RegionJetEfficiencyHistograms(const RegionConfig& config, 
				const unsigned hist_builder_flags = 0); 
  ~RegionJetEfficiencyHistograms(); 
  virtual void fill(const EventObjects& objects); 
  virtual void write_to(H5::CommonFG&) const; 
private: 
  static const double MAX_PT_MEV = 1000; 
  typedef RegionJetEfficiencyHistograms ThisClass; 
  ThisClass& operator=(const ThisClass&) = delete; 
  RegionJetEfficiencyHistograms(const ThisClass&) = delete; 
  const unsigned m_build_flags; 
  const RegionConfig* m_region_config; 
  const RegionEventFilter* m_event_filter; 
  
  std::map<Flavor, JetEfficiencyHists*> m_jet_pt_hists; 
}; 

#endif 
