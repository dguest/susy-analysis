#include "MetSelection.hh"
#include "RegionConfig.hh"
#include "EventObjects.hh"
#include "constants_scharmcuts.hh"
#include "trigger_logic.hh"

#include <stdexcept>



MetSelection::MetSelection(const RegionConfig& reg):
  m_stream(reg.stream)
{
  trig::throw_if_not_jetstream(reg.stream);
}

MetSelection::~MetSelection() {

}

bool MetSelection::pass(const EventObjects& obj) const {
  const EventRecoParameters& reco = obj.reco;

  using namespace reg;
  auto n_leptons = reco.n_baseline_electrons + reco.n_baseline_muons;
  bool zero_lepton_selection = n_leptons == 0 && reco.pass_met_trigger;
  return zero_lepton_selection;
}
