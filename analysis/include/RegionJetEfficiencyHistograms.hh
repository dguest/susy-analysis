#ifndef REGION_JET_EFFICIENCY_HISTOGRAMS_HH
#define REGION_JET_EFFICIENCY_HISTOGRAMS_HH

#include "IRegionHistograms.hh"
#include <map>
#include <boost/noncopyable.hpp>
#include "PhysicalConstants.hh"

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
  void write_to(H5::CommonFG&) const;
private: 
  Histogram* m_jet_pt_all; 
  Histogram* m_jet_pt_tagged; 
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
  typedef RegionJetEfficiencyHistograms ThisClass; 
  ThisClass& operator=(const ThisClass&) = delete; 
  RegionJetEfficiencyHistograms(const ThisClass&) = delete; 
  const unsigned m_build_flags; 
  const RegionConfig* m_region_config; 
  const RegionEventFilter* m_event_filter; 
  
  std::map<Flavor, Histogram*> m_jet_pt_hists; 
}; 

#endif 
