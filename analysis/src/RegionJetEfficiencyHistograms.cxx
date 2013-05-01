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
void JetEfficiencyHists::write_to(H5::CommonFG& group, 
				  std::string group_name) const { 
  H5::Group subgroup(group.createGroup(group_name)); 
  m_jet_pt_all->write_to(subgroup, "all"); 
  m_jet_pt_medium->write_to(subgroup, "medium"); 
  m_jet_pt_loose->write_to(subgroup, "loose"); 
  m_jet_pt_antiloose->write_to(subgroup, "antiloose"); 
}

RegionJetEfficiencyHistograms
::RegionJetEfficiencyHistograms(const RegionConfig& config, 
				const unsigned flags): 
  m_build_flags(flags), 
  m_region_config(new RegionConfig(config)), 
  m_event_filter(new RegionEventFilter(config))
{ 
  using namespace jeteff; 
  m_jet_pt_hists[Flavor::CHARM] = new JetEfficiencyHists(MAX_PT_MEV); 
  m_jet_pt_hists[Flavor::BOTTOM] = new JetEfficiencyHists(MAX_PT_MEV); 
  m_jet_pt_hists[Flavor::LIGHT] = new JetEfficiencyHists(MAX_PT_MEV); 
  m_jet_pt_hists[Flavor::TAU] = new JetEfficiencyHists(MAX_PT_MEV); 
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
    auto hist_itr = m_jet_pt_hists.find(jitr->flavor_truth_label()); 
    if (hist_itr == m_jet_pt_hists.end()) { 
      throw std::logic_error("unknown jet flavor label in "__FILE__); 
    }
    hist_itr->second->fill(*jitr); 
  }
}
void RegionJetEfficiencyHistograms::write_to(H5::CommonFG& group) const { 
  using namespace H5; 
  Group region(group.createGroup(m_region_config->name)); 
  for (auto flav_itr = m_jet_pt_hists.cbegin(); 
       flav_itr != m_jet_pt_hists.cend(); flav_itr++) { 
    flav_itr->second->write_to(region, as_char(flav_itr->first)); 
  }
}
