#include "RegionEventFilter.hh"
#include "EventObjects.hh"
#include "JetFactory.hh"

RegionEventFilter::RegionEventFilter(const RegionConfig& config, unsigned): 
  m_region_config(config)
{
}

bool RegionEventFilter::pass(const EventObjects& obj) const { 
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

  const auto& jet_req = m_region_config.jet_tag_requirements; 
  for (unsigned jet_n = 0; jet_n < jet_req.size(); jet_n++) {
    const auto jet = jets.at(jet_n); 
    const auto requested_tag = jet_req.at(jet_n); 
    bool pass = jet.pass_tag(requested_tag); 
    if (!pass) return false; 
  }

  if (jets.at(0).Pt() < m_region_config.leading_jet_pt) { 
    return false; 
  }
  const TVector2& met = obj.met; 
  if (met.Mod() < m_region_config.met) { 
    return false; 
  }
  return true; 

}
