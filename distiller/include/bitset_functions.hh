#ifndef BITSET_FUNCTIONS_HH
#define BITSET_FUNCTIONS_HH

class TVector2; 
class SelectedJet;
class Muon; 
class Electron; 
class ObjectComposites; 
class EventObjects; 

#include "typedefs.hh"

#include <vector> 

// ---- cutflow functions ----

// master of all objects
namespace bits { 
  // mother of all event object checkers
  ull_t event_object_bits(const EventObjects& obj); 
  // called within the above function 
  ull_t signal_jet_bits(const std::vector<SelectedJet*>& signal_jets); 
  ull_t control_lepton_bits(const std::vector<Electron*>&, 
			    const std::vector<Muon*>&);

  // other stuff
  ull_t object_composit_bits(const ObjectComposites& par); 

  ull_t met_bits(const TVector2& met); 

  ull_t bad_tile_bits(const TVector2& mets, 
		      const std::vector<SelectedJet*> jets); 
}


#endif 
