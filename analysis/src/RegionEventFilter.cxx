#include "RegionEventFilter.hh"
#include "RegionConfig.hh"
#include "EventObjects.hh"
#include "JetTagRescaler.hh"
#include "enum_converters.hh"
#include "IJetTagFilter.hh"
#include "OrderedJetTagFilter.hh"
#include "UnorderedJetTagFilter.hh"
#include "EventScalefactors.hh"
#include "Jet.hh"
#include <stdexcept> 

RegionEventFilter::RegionEventFilter(const RegionConfig& config, unsigned): 
  m_region_config(config), 
  m_jet_rescaler(0), 
  m_jet_tag_filter(0)
{
  if (config.mc_mc_jet_reweight_file.size()) { 
    m_jet_rescaler = new JetTagRescaler(config.mc_mc_jet_reweight_file); 
  }
  switch (config.jet_tag_assignment) { 
  case btag::PT_ORDERED: 
    m_jet_tag_filter = new OrderedJetTagFilter(config.jet_tag_requirements,
					       config.systematic); 
    return; 
  case btag::TAG_ORDERED: 
    m_jet_tag_filter = new UnorderedJetTagFilter(config.jet_tag_requirements,
						 config.systematic); 
    return; 
  }
}
RegionEventFilter::~RegionEventFilter() { 
  delete m_jet_rescaler; 
  delete m_jet_tag_filter; 
}

bool RegionEventFilter::pass(const EventObjects& obj) const { 
  if (reg::throw_for_fun & m_region_config.region_bits) { 
    throw std::runtime_error("fucking fun"); 
  }
  typedef std::vector<Jet> Jets; 
  if (obj.event_mask & m_region_config.veto_bits) return false; 
  const ull_t req_bits = m_region_config.required_bits; 
  if ( (obj.event_mask & req_bits) != req_bits){
    return false; 
  }

  bool do_mu_met = m_region_config.region_bits & reg::mu_met; 
  const TVector2& met = do_mu_met ? obj.mu_met : obj.met; 
  if (met.Mod() < m_region_config.met) { 
    return false; 
  }

  bool use_electron_jet = m_region_config.region_bits & reg::electron_jet; 
  const Jets jets = use_electron_jet ? obj.jets_with_eljet : obj.jets; 
  if (m_region_config.leading_jet_pt > 0.0) { 
    if (jets.at(0).Pt() < m_region_config.leading_jet_pt) { 
      return false; 
    }
  }

  unsigned n_required_jets = m_region_config.jet_tag_requirements.size(); 
  if (jets.size() < n_required_jets) return false; 
  if (reg::no_extra_jets & m_region_config.region_bits) { 
    if (jets.size() > n_required_jets) return false; 
  }

  if (! m_jet_tag_filter->pass(jets) ) return false; 
  return true; 

}

double RegionEventFilter::jet_scalefactor(const EventObjects& obj) const { 
  typedef std::vector<Jet> Jets; 
  bool use_electron_jet = m_region_config.region_bits & reg::electron_jet; 
  const Jets jets = use_electron_jet ? obj.jets_with_eljet : obj.jets; 
  double weight = m_jet_tag_filter->jet_scalefactor(jets); 
  return weight; 
}

float RegionEventFilter::lepton_scalefactor(const EventObjects& obj) const {
  float weight = 1.0; 
  if (obj.event_scalefactors) { 
    weight *= obj.event_scalefactors->lepton_sf(
      EventSyst::ELECTRON, m_region_config.systematic); 
    weight *= obj.event_scalefactors->lepton_sf(
      EventSyst::MUON, m_region_config.systematic); 
  }
  return weight; 
}
