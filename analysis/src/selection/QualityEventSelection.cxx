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

  using namespace reg;
  auto n_leptons = reco.n_baseline_electrons + reco.n_baseline_muons;
  bool zero_lepton_selection = n_leptons == 0 && reco.pass_met_trigger;
  if (m_stream == Stream::JET) {
    return zero_lepton_selection;
  }
  bool lep_trig = trig::pass_any_lepton_trigger(reco, m_stream);
  bool lepton_selection = ( lep_trig && n_leptons > 0);
  if (m_stream == Stream::SIMULATED) {
    return zero_lepton_selection || lepton_selection;
  }
  return lepton_selection;
}
