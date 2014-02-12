#include "CR1LSelection.hh"
#include "RegionConfig.hh"
#include "EventObjects.hh"
#include "constants_scharmcuts.hh"

#include <cassert>

CR1LSelection::CR1LSelection(const RegionConfig& ) { 
  
}

CR1LSelection::~CR1LSelection() { 

}

bool CR1LSelection::pass(const EventObjects& obj) const { 
  
  const EventRecoParameters& reco = obj.reco; 
  // check trigger
  if (! (reco.pass_mu_trigger || reco.pass_el_trigger) ) return false; 

  auto total_leptons = reco.n_control_electrons + reco.n_control_muons; 
  if (total_leptons != 1) return false; 

  // check object counts
  if (reco.n_signal_jets < 2) return false; 

  // other parameters
  if (obj.jets.size() > 2) { 
    if (obj.jets.at(2).Pt() > SIGNAL_JET_3_MAX_PT) return false; 
  }

  using namespace cr1l; 

  if (reco.min_jetmet_dphi < MIN_DPHI_JET_MET) return false; 

  if (reco.mct < SR_MCT_MIN) return false;

  if (reco.mt > M_T_MAX) return false;
  if (reco.mt < M_T_MIN) return false;

  return true;
}

