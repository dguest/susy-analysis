#include "SignalSelection.hh"
#include "RegionConfig.hh"
#include "EventObjects.hh"
#include "constants_stopcuts.hh"


SignalSelection::SignalSelection(const RegionConfig& ) { 
  
}

SignalSelection::~SignalSelection() { 

}

bool SignalSelection::pass(const EventObjects& obj) const { 

  // --- check dphi(jet, met) ---
  auto& jets = obj.jets; 
  auto& met = obj.met; 
  size_t n_check = std::min(jets.size(), DPHI_JET_MET_NJET); 
  for (size_t jet_n = 0; jet_n < n_check; jet_n++) { 
    const auto& jet = jets.at(jet_n); 
    if (std::abs(jet.Vect().XYvector().DeltaPhi(met)) < MIN_DPHI_JET_MET){
      return false; 
    }
  }

  return true; 
}
