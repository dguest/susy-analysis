#include "SignalSelection.hh"
#include "RegionConfig.hh"
#include "EventObjects.hh"
#include "constants_scharmcuts.hh"
#include "trigger_logic.hh"

#include <stdexcept>

// ______________________________________________________________________
// Looser version

NMinusSignalSelection::NMinusSignalSelection(const RegionConfig& reg) {
  trig::throw_if_not_jetstream(reg.stream);
}

NMinusSignalSelection::~NMinusSignalSelection() {

}

bool NMinusSignalSelection::pass(const EventObjects& obj) const {

  const EventRecoParameters& reco = obj.reco;
  // check trigger
  if (!reco.pass_met_trigger) return false;

  // check object counts
  if (reco.n_baseline_electrons > 0) return false;
  if (reco.n_baseline_muons > 0) return false;

  return true;
}

// ______________________________________________________________________
// tighter version

SignalSelection::SignalSelection(const RegionConfig& cfg):
  m_nminus(cfg)
{
}

SignalSelection::~SignalSelection() {

}

bool SignalSelection::pass(const EventObjects& obj) const {
  throw std::logic_error("deprecated in " __FILE__);

  // call the above routine
  if (!m_nminus.pass(obj)) return false;

  const EventRecoParameters& reco = obj.reco;

  // other parameters
  if (obj.jets.size() > 2) {
    if (obj.jets.at(2).Pt() > SIGNAL_JET_3_MAX_PT) return false;
  }
  if (reco.min_jetmet_dphi < MIN_DPHI_JET_MET) return false;
  if (reco.mct < SR_MCT_MIN) return false;
  if (reco.met_eff < MET_EFF_MIN) return false;
  if (reco.mcc < M_CC_MIN) return false;

  return true;
}
