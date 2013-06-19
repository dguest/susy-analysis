#include "EventObjects.hh"
#include "HistBuilderFlags.hh"
#include "ObjectFactory.hh"

namespace { 
  typedef std::vector<Jet> Jets; 
  Jets remove_electron_jets(Jets); 
}


EventObjects::EventObjects(const ObjectFactory* factory, unsigned build_flags): 
  met(factory->met()), 
  mu_met(factory->mu_met()), 
  weight(factory->event_weight()), 
  event_mask(factory->bits()), 
  htx(factory->htx()), 
  leading_cjet_pos(0), 
  subleading_cjet_pos(0), 
  n_signal_jets(factory->n_good()), 
  jets_with_eljet(factory->jets())
{ 
  if ( !(build_flags & buildflag::is_data)) { 
    leading_cjet_pos = factory->leading_cjet_pos(); 
    subleading_cjet_pos = factory->subleading_cjet_pos(); 
  }
  jets = remove_electron_jets(jets_with_eljet); 
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
