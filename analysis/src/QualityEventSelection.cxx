#include "QualityEventSelection.hh"
#include "RegionConfig.hh"
#include "EventObjects.hh"
#include "constants_scharmcuts.hh"
#include "trigger_logic.hh"

#include <stdexcept>



QualityEventSelection::QualityEventSelection(const RegionConfig& reg):
  m_stream(reg.stream)
{ 
}

QualityEventSelection::~QualityEventSelection() { 

}

bool QualityEventSelection::pass(const EventObjects& obj) const { 
  const EventRecoParameters& reco = obj.reco;
  // all regions require 2 jets
  if (reco.n_signal_jets < 2) return false;

  using namespace reg;
  auto n_lep = reco.n_baseline_electrons + reco.n_baseline_muons;
  if (m_stream == Stream::JET) {
    return reco.pass_met_trigger;
  }

  if (reco.pass_met_trigger) return true;

  bool one_lep_trigger = trig::pass_single_lepton_trigger(reco, m_stream);
  bool two_lep_trigger = trig::pass_two_lepton_trigger(reco, m_stream);
  if (n_lep == 1) {
    return one_lep_trigger;
  } else if (n_lep == 2) {
    return two_lep_trigger;
  }
  return false;
}
