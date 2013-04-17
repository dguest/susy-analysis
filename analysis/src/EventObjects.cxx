#include "EventObjects.hh"
#include "HistBuilderFlags.hh"
#include "ObjectFactory.hh"

EventObjects::EventObjects(const ObjectFactory* factory, unsigned build_flags): 
  met(factory->met()), 
  mu_met(factory->mu_met()), 
  weight(factory->event_weight()), 
  event_mask(factory->bits()), 
  htx(factory->htx()), 
  leading_cjet_pos(0), 
  subleading_cjet_pos(0), 
  n_signal_jets(factory->n_good()), 
  jets(factory->jets())
{ 
  if ( !(build_flags & buildflag::is_data)) { 
    leading_cjet_pos = factory->leading_cjet_pos(); 
    subleading_cjet_pos = factory->subleading_cjet_pos(); 
  }
  
}
