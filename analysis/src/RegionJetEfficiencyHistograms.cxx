#include "RegionJetEfficiencyHistograms.hh"
#include "RegionConfig.hh"
#include "RegionEventFilter.hh"
#include "Histogram.hh"
#include "Flavor.hh"
#include "H5Cpp.h"
#include "Jet.hh"
#include "BtagConfig.hh"
#include "EventObjects.hh"
#include <stdexcept> 

JetEfficiencyHists::JetEfficiencyHists(double max_pt_mev )
{ 
  const std::string units = "MeV"; 
  m_jet_pt_all = new Histogram(100, 0, max_pt_mev, units); 
  m_jet_pt_medium = new Histogram(100, 0, max_pt_mev, units); 
  m_jet_pt_loose = new Histogram(100, 0, max_pt_mev, units); 
  m_jet_pt_antiloose = new Histogram(100, 0, max_pt_mev, units); 
}
JetEfficiencyHists::~JetEfficiencyHists() { 
  delete m_jet_pt_all; 
  delete m_jet_pt_medium; 
  delete m_jet_pt_loose; 
  delete m_jet_pt_antiloose; 
}
void JetEfficiencyHists::fill(const Jet& jet) { 
  double pt = jet.Pt(); 
  m_jet_pt_all->fill(pt); 
  if (jet.pass_tag(btag::MEDIUM)) { 
    m_jet_pt_medium->fill(pt); 
  }
  if (jet.pass_tag(btag::LOOSE)) { 
    m_jet_pt_loose->fill(pt); 
  }
  if (jet.pass_tag(btag::ANTILOOSE)) { 
    m_jet_pt_antiloose->fill(pt); 
  }

}
void JetEfficiencyHists::write_to(H5::CommonFG& group) const { 
  m_jet_pt_all->write_to(group, "all"); 
  m_jet_pt_medium->write_to(group, "medium"); 
  m_jet_pt_loose->write_to(group, "loose"); 
  m_jet_pt_antiloose->write_to(group, "antiloose"); 
}



RegionJetEfficiencyHistograms
::RegionJetEfficiencyHistograms(const RegionConfig& config, 
				const unsigned flags): 
  m_build_flags(flags), 
  m_region_config(new RegionConfig(config)), 
  m_event_filter(new RegionEventFilter(config))
{ 
  add_hists(Flavor::CHARM); 
  add_hists(Flavor::BOTTOM); 
  add_hists(Flavor::LIGHT); 
  add_hists(Flavor::TAU); 
}
RegionJetEfficiencyHistograms::~RegionJetEfficiencyHistograms() { 
  delete m_region_config; 
  delete m_event_filter; 
  for (auto itr = m_jet_pt_hists.begin(); itr!= m_jet_pt_hists.end(); itr++) { 
    delete itr->second; 
    itr->second = 0; 
  }
}
void RegionJetEfficiencyHistograms::add_hists(Flavor flavor) { 
  // ugly static cast used to map flavor types
  int index = static_cast<int>(flavor); 
  m_jet_pt_hists.insert
    (std::make_pair(index, new JetEfficiencyHists(MAX_PT_MEV))); 
}
void RegionJetEfficiencyHistograms::fill(const EventObjects& objects) { 
  if (!m_event_filter->pass(objects)) return; 
  const auto& jets = objects.jets; 
  for (auto jitr = jets.cbegin(); jitr != jets.cend(); jitr++){
    // this is another ugly cast
    int flav_idx = static_cast<int>(jitr->flavor_truth_label()); 
    auto hist_itr = m_jet_pt_hists.find(flav_idx); 
    if (hist_itr == m_jet_pt_hists.end()) { 
      throw std::logic_error("unknown jet flavor label in "__FILE__); 
    }
    // **** WORK DO HERE
  }
}
void RegionJetEfficiencyHistograms::write_to(H5::CommonFG& group) const { 
  // *** work do here
}
