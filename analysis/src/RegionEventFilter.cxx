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

#include "ISelection.hh"
#include "SignalSelection.hh"
#include "OSDFSelection.hh"

#include <stdexcept> 

namespace { 
  ISelection* selection_factory(const RegionConfig&); 
}

RegionEventFilter::RegionEventFilter(const RegionConfig& config, unsigned): 
  m_region_config(config), 
  m_jet_rescaler(0), 
  m_jet_tag_filter(0), 
  m_selection(0)
{
  m_selection = selection_factory(config); 
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
  delete m_selection; 
}

bool RegionEventFilter::pass(const EventObjects& obj) const { 
  if (reg::throw_for_fun & m_region_config.region_bits) { 
    throw std::runtime_error("fucking fun"); 
  }

  // assume we'll always want "quality" events..
  if (!obj.reco.pass_event_quality) return false; 

  // selection configured by the region 
  if (!m_selection->pass(obj)) return false; 

  // --- check met ---
  const TVector2& met = obj.met; 
  if (met.Mod() < m_region_config.met) { 
    return false; 
  }

  // --- check leading jet pt ---
  const auto& jets = obj.jets; 
  if (m_region_config.leading_jet_pt > 0.0) { 
    if (jets.size() == 0) return false; 
    if (jets.at(0).Pt() < m_region_config.leading_jet_pt) { 
      return false; 
    }
  }

  // --- check number of tagged jets (must have at least this many) ---
  unsigned n_required_jets = m_region_config.jet_tag_requirements.size(); 
  if (jets.size() < n_required_jets) return false; 
  if (reg::no_extra_jets & m_region_config.region_bits) { 
    if (jets.size() > n_required_jets) return false; 
  }


  return true; 

}

std::vector<Jet> RegionEventFilter::tagged_jets(const std::vector<Jet>& jets) 
  const { 
  return m_jet_tag_filter->tagged_jets(jets); 
}


double RegionEventFilter::jet_scalefactor(const std::vector<Jet>& jets) 
  const { 
  double weight = 1.0; 
  for (auto jet: jets) { 
    weight *= jet.get_scalefactor(m_region_config.systematic);
  }
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

float RegionEventFilter::boson_scalefactor(const EventObjects& obj) const { 
  if (m_region_config.boson_pt_correction == reg::NO_PT_CORRECTION) { 
    return 1.0; 
  }
  if (m_region_config.boson_pt_correction == reg::MARKS) { 
    return obj.marks_boson_pt_weight; 
  }
  throw std::invalid_argument("unknown boson pt correction"); 
}

namespace { 
  ISelection* selection_factory(const RegionConfig& conf) 
  { 
    using namespace reg; 
    Selection sel = conf.selection; 
    switch (sel) { 
    case Selection::ERROR: throw std::logic_error("selection not set"); 
    case Selection::SIGNAL: return new SignalSelection(conf); 
    case Selection::CR_DF: return new OSDFSelection(conf); 
    default: throw std::logic_error("got undefined selection in " __FILE__); 
    }
  }
}
