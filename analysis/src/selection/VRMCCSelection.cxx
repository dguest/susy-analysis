#include "VRMCCSelection.hh"
#include "RegionConfig.hh"
#include "EventObjects.hh"
#include "constants_scharmcuts.hh"
#include "trigger_logic.hh"

#include <stdexcept>

// ______________________________________________________________________
// Looser version

NMinusVRMCCSelection::NMinusVRMCCSelection(const RegionConfig& reg) {
  trig::throw_if_not_jetstream(reg.stream);
}

NMinusVRMCCSelection::~NMinusVRMCCSelection() {

}

bool NMinusVRMCCSelection::pass(const EventObjects& obj) const {

  const EventRecoParameters& reco = obj.reco;
  // check trigger
  if (!reco.pass_met_trigger) return false;

  // check object counts
  if (reco.n_baseline_electrons > 0) return false;
  if (reco.n_baseline_muons > 0) return false;

  // keep signal regions blind
  if (reco.mcc > vr::VR_MCC_MAX) return false;

  // also blind the low met region to keep people from asking about
  // multijet stuff
  if (obj.met.Mod() < vr::VR_MET_MIN) return false;

  return true;
}

