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
  if (m_stream == Stream::JET) {
    return reco.pass_met_trigger;
  }

  if (reco.pass_met_trigger) return true;

  if (trig::pass_any_lepton_trigger(reco, m_stream)) return true;
  return false;
}
