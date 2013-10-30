#include "EventObjects.hh"
#include "HistBuilderFlags.hh"
#include "ObjectFactory.hh"
#include "systematic_defs.hh"
#include <stdexcept>

namespace { 
  typedef std::vector<Jet> Jets; 
  Jets remove_electron_jets(Jets); 
}

MetFlavors::MetFlavors(const ObjectFactory* obj, syst::Systematic syst): 
  bare(obj->met(syst)), 
  muon(obj->mu_met(syst))
{
}

MetSystematics::MetSystematics(const ObjectFactory* obj, bool is_data): 
  m_nominal(obj, syst::NONE), 
  m_up(0), 
  m_down(0), 
  m_res(0)
{
  if (!is_data) { 
    m_up = new MetFlavors(obj, syst::METUP);
    m_down = new MetFlavors(obj, syst::METDOWN); 
    m_res = new MetFlavors(obj, syst::METRES); 
  }
}
MetSystematics::~MetSystematics() { 
  delete m_up; 
  delete m_down; 
  delete m_res; 
}
const MetFlavors& MetSystematics::get_syst(syst::Systematic syst) const { 
  if ( (syst == syst::METUP || 
	syst == syst::METDOWN || 
	syst == syst::METRES ) && !m_up) { 
    throw std::logic_error("problem in " __FILE__); 
  }
  switch (syst) { 
  case syst::METUP: return *m_up; 
  case syst::METDOWN: return *m_down; 
  case syst::METRES: return *m_res; 
  default: return m_nominal; 
  }
}

EventObjects::EventObjects(const ObjectFactory* factory, 
			   unsigned build_flags): 
  met(factory, build_flags & buildflag::is_data), 
  weight(factory->event_weight()), 
  event_mask(factory->bits()), 
  htx(factory->htx()), 
  leading_cjet_pos(0), 
  subleading_cjet_pos(0), 
  n_signal_jets(factory->n_signal_jets()), 
  jets_with_eljet(factory->jets()), 
  event_scalefactors(0), 
  true_boson_pt(factory->boson_pt()), 
  marks_boson_pt_weight(factory->marks_boson_pt_weight()), 
  boson_child(factory->boson_child())
{ 
  if ( !(build_flags & buildflag::is_data)) { 
    leading_cjet_pos = factory->leading_cjet_pos(); 
    subleading_cjet_pos = factory->subleading_cjet_pos(); 
    event_scalefactors = factory->event_scalefactors(); 
  }
  jets = remove_electron_jets(jets_with_eljet); 
  for (size_t rank = 0; rank < jets.size(); rank++) { 
    jets.at(rank).set_rank(rank); 
  }
  for (size_t rank = 0; rank < jets_with_eljet.size(); rank++) { 
    jets_with_eljet.at(rank).set_rank(rank); 
  }

}

namespace { 
  Jets remove_electron_jets(Jets jets) { 
    Jets repl_jets; 
    for (auto jet = jets.begin(); jet != jets.end(); jet++) { 
      if (! jet->is_electron_jet()) { 
	repl_jets.push_back(*jet); 
      }
    }
    return repl_jets; 
  }
  
}
