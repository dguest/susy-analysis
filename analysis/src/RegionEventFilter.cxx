#include "RegionEventFilter.hh"
#include "RegionConfig.hh"
#include "EventObjects.hh"
#include "JetTagRescaler.hh"
#include "enum_converters.hh"
#include "Jet.hh"
#include <stdexcept> 

RegionEventFilter::RegionEventFilter(const RegionConfig& config, unsigned): 
  m_region_config(config), 
  m_jet_rescaler(0)
{
  if (config.mc_mc_jet_reweight_file.size()) { 
    m_jet_rescaler = new JetTagRescaler(config.mc_mc_jet_reweight_file); 
  }
}
RegionEventFilter::~RegionEventFilter() { 
  delete m_jet_rescaler; 
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

  const Jets jets = obj.jets; 
  unsigned n_required_jets = m_region_config.jet_tag_requirements.size(); 
  if (jets.size() < n_required_jets) { 
    return false; 
  }
  else if (reg::no_extra_jets & m_region_config.region_bits) { 
    if (jets.size() > n_required_jets) return false; 
  }

  const auto& jet_req = m_region_config.jet_tag_requirements; 
  for (unsigned jet_n = 0; jet_n < jet_req.size(); jet_n++) {
    const auto jet = jets.at(jet_n); 
    const auto requested_tag = jet_req.at(jet_n); 
    bool pass = jet.pass_tag(requested_tag); 
    if (!pass) return false; 
    if (! (m_region_config.region_bits & reg::electron_jet) ) { 
      if (jet.is_electron_jet()) return false; 
    }
  }

  if (m_region_config.leading_jet_pt > 0.0) { 
    if (jets.at(0).Pt() < m_region_config.leading_jet_pt) { 
      return false; 
    }
  }
  bool do_mu_met = m_region_config.region_bits & reg::mu_met; 
  const TVector2& met = do_mu_met ? obj.mu_met : obj.met; 
  if (met.Mod() < m_region_config.met) { 
    return false; 
  }
  return true; 

}

double RegionEventFilter::jet_scalefactor(const EventObjects& obj) const { 
  double weight = 1; 
  const auto& jet_req = m_region_config.jet_tag_requirements; 
  for (unsigned jet_n = 0; jet_n < jet_req.size(); jet_n++) {
    const auto jet = obj.jets.at(jet_n); 
    const auto requested_tag = jet_req.at(jet_n); 
    weight *= jet.get_scalefactor(requested_tag, 
				  m_region_config.systematic);
    if (m_jet_rescaler) { 
      jettag::TaggingPoint rescale_op = jettag_from_btag(requested_tag); 
      double jpt = jet.Pt(); 
      int ftl = static_cast<int>(jet.flavor_truth_label()); 
      weight *= m_jet_rescaler->get_sf(jpt, ftl, rescale_op); 
    }
    
  }
  return weight; 
}
