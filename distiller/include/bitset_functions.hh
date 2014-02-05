#ifndef BITSET_FUNCTIONS_HH
#define BITSET_FUNCTIONS_HH

class Mets; 
class SelectedJet;
class Muon; 
class Electron; 

#include "typedefs.hh"

#include <vector> 

// ---- cutflow functions ----
// TODO: move these into a new file, they depend on a bunch of crap
ull_t control_lepton_bits(const std::vector<Electron*>&, 
			  const std::vector<Muon*>&);

ull_t z_control_bits(const std::vector<Electron*>&, 
		     const std::vector<Muon*>&); 

ull_t signal_jet_bits(const std::vector<SelectedJet*>& signal_jets); 

ull_t met_bits(const Mets& met); 

ull_t bad_tile_bits(const Mets& mets, const std::vector<SelectedJet*> jets); 


#endif 
