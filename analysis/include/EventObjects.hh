#ifndef EVENT_OBJECTS_HH
#define EVENT_OBJECTS_HH

#include "TVector2.h"
#include "JetFactory.hh"
#include <vector> 
#include "typedefs.hh"

struct EventObjects
{
  TVector2 met; 
  double weight; 
  ull_t event_mask; 
  double htx; 
  int leading_cjet_pos; 
  int subleading_cjet_pos; 
  int n_signal_jets; 
  std::vector<Jet> jets; 
  EventObjects(const JetFactory*, unsigned build_flags); 
}; 

#endif 
