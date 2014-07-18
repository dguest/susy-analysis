#include "VRMCTSelection.hh"
#include "RegionConfig.hh"
#include "EventObjects.hh"
#include "constants_scharmcuts.hh"
#include "trigger_logic.hh"

#include <stdexcept>

// ______________________________________________________________________
// Looser version

NMinusVRMCTSelection::NMinusVRMCTSelection(const RegionConfig& reg) {
  trig::throw_if_not_jetstream(reg.stream);
}

NMinusVRMCTSelection::~NMinusVRMCTSelection() {

}

bool NMinusVRMCTSelection::pass(const EventObjects& obj) const {

  const EventRecoParameters& reco = obj.reco;
  // check trigger
  if (!reco.pass_met_trigger) return false;

  // check object counts
  if (reco.n_baseline_electrons > 0) return false;
  if (reco.n_baseline_muons > 0) return false;

  // keep signal regions blind
  if (reco.mct > vr::VR_MCT_MAX) return false;

  return true;
}

