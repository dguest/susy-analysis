#include "SignalSelection.hh"
#include "RegionConfig.hh"
#include "EventObjects.hh"
#include "constants_scharmcuts.hh"


SignalSelection::SignalSelection(const RegionConfig& ) { 
  
}

SignalSelection::~SignalSelection() { 

}

bool SignalSelection::pass(const EventObjects& obj) const { 
  
  const EventRecoParameters& reco = obj.reco; 
  // check trigger
  if (!reco.pass_met_trigger) return false; 

  // check object counts
  if (reco.n_signal_jets < 2) return false; 
  if (reco.n_veto_electrons > 0) return false; 
  if (reco.n_veto_muons > 0) return false; 

  // other parameters
  if (obj.jets.size() >= 3) { 
    if (obj.jets.at(3).Pt() > SIGNAL_JET_3_MAX_PT) return false; 
  }
  if (reco.min_jetmet_dphi < MIN_DPHI_JET_MET) return false; 
  if (reco.mct < SR_MCT_MIN) return false; 
  if (reco.met_eff < MET_EFF_MIN) return false; 
  if (reco.mcc < M_CC_MIN) return false; 
  

  return true; 
}
