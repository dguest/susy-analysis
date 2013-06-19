#ifndef EVENT_OBJECTS_HH
#define EVENT_OBJECTS_HH

// Structure that can be built once per event and passed into all the 
// histograming routines. 
// Constructed in HistBuilder

#include "TVector2.h"
#include "Jet.hh"
#include <vector> 
#include "typedefs.hh"

class ObjectFactory; 

struct EventObjects
{
  TVector2 met; 
  TVector2 mu_met; 
  double weight; 
  ull_t event_mask; 
  double htx; 
  int leading_cjet_pos; 
  int subleading_cjet_pos; 
  int n_signal_jets; 
  std::vector<Jet> jets; 
  std::vector<Jet> jets_with_eljet; 
  EventObjects(const ObjectFactory*, unsigned build_flags); 
}; 

#endif 
