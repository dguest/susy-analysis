#ifndef EVENT_OBJECTS_HH
#define EVENT_OBJECTS_HH

// Structure that can be built once per event and passed into all the 
// histograming routines. 
// Constructed in HistBuilder

#include "TVector2.h"
#include "Jet.hh"
#include <vector> 
// #include <unordered_map>
#include "typedefs.hh"
#include "systematic_defs.hh"
#include "EventRecoParameters.hh"

class ObjectFactory; 
class EventScalefactors; 

struct EventObjects
{
  TVector2 met; 
  double weight; 
  ull_t event_mask; 
  EventRecoParameters reco; 
  int leading_cjet_pos; 
  int subleading_cjet_pos; 
  int n_signal_jets; 
  std::vector<Jet> jets; 
  EventScalefactors* event_scalefactors; 
  float true_boson_pt; 
  float marks_boson_pt_weight; 
  TVector2 boson_child; 
  EventObjects(const ObjectFactory*, unsigned build_flags); 
}; 

#endif 
