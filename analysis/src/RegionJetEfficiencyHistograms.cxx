#include "RegionJetEfficiencyHistograms.hh"
#include "RegionConfig.hh"
#include "RegionEventFilter.hh"
#include "Histogram.hh"
#include "PhysicalConstants.hh"
#include "H5Cpp.h"
#include "Jet.hh"
#include "BtagConfig.hh"
#include "EventObjects.hh"

JetEfficiencyHists::JetEfficiencyHists(double max_pt_mev )
{ 
  m_jet_pt_all = new Histogram(100, 0, max_pt_mev, "MeV"); 
  m_jet_pt_tagged = new Histogram(100, 0, max_pt_mev, "MeV"); 
}
JetEfficiencyHists::~JetEfficiencyHists() { 
  delete m_jet_pt_all; 
  delete m_jet_pt_tagged; 
}
void JetEfficiencyHists::fill(const Jet& jet) { 
  double pt = jet.Pt(); 
  m_jet_pt_all->fill(pt); 
  if (jet.pass_tag(btag::MEDIUM)) { 
    m_jet_pt_tagged->fill(pt); 
  }
}
void JetEfficiencyHists::write_to(H5::CommonFG& group) const { 
  m_jet_pt_all->write_to(group, "all"); 
  m_jet_pt_tagged->write_to(group, "medium"); 
}



RegionJetEfficiencyHistograms
::RegionJetEfficiencyHistograms(const RegionConfig& config, 
				const unsigned flags): 
  m_build_flags(flags), 
  m_region_config(new RegionConfig(config)), 
  m_event_filter(new RegionEventFilter(config))
{ 
}
RegionJetEfficiencyHistograms::~RegionJetEfficiencyHistograms() { 
  delete m_region_config; 
  delete m_event_filter; 
  for (auto itr = m_jet_pt_hists.begin(); itr!= m_jet_pt_hists.end(); itr++) { 
    delete itr->second; 
    itr->second = 0; 
  }
}
void RegionJetEfficiencyHistograms::fill(const EventObjects& objects) { 
  if (!m_event_filter->pass(objects)) return; 
  const auto& jets = objects.jets; 
  for (auto jitr = jets.cbegin(); jitr != jets.cend(); jitr++){
    // **** WORK DO HERE
  }
}
void RegionJetEfficiencyHistograms::write_to(H5::CommonFG& group) const { 
  // *** work do here
}
