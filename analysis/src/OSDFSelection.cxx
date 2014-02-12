#include "OSDFSelection.hh"
#include "RegionConfig.hh"
#include "EventObjects.hh"
#include "constants_scharmcuts.hh"

#include <cassert>

OSDFSelection::OSDFSelection(const RegionConfig& ) { 
  
}

OSDFSelection::~OSDFSelection() { 

}

bool OSDFSelection::pass(const EventObjects& obj) const { 
  
  const EventRecoParameters& reco = obj.reco; 
  // check trigger
  if (! (reco.pass_mu_trigger || reco.pass_el_trigger) ) return false; 

  if (!reco.pass_osdf) return false; 

  // check object counts
  if (reco.n_signal_jets < 2) return false; 

  // other parameters
  if (obj.jets.size() > 2) { 
    if (obj.jets.at(2).Pt() > SIGNAL_JET_3_MAX_PT) return false; 
  }
  assert(false); 
  if (reco.min_jetmet_dphi < MIN_DPHI_JET_MET) return false; 
  if (reco.mct < SR_MCT_MIN) return false; 
  if (reco.met_eff < MET_EFF_MIN) return false; 
  if (reco.mcc < M_CC_MIN) return false; 
  

  return true; 
}

