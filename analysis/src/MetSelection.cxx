#include "MetSelection.hh"
#include "RegionConfig.hh"
#include "EventObjects.hh"
#include "constants_scharmcuts.hh"
#include "constants_tagging.hh"
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

  // veto events in SR
  const size_t jets_to_check = std::min<size_t>(obj.jets.size(), 2);
  int passing = 0;
  for (auto jet_n = 0U; jet_n < jets_to_check; jet_n++) {
    bool pass = obj.jets.at(jet_n).pass_tag(btag::JFC_MEDIUM);
    if (pass) passing++;
  }
  return zero_lepton_selection && (passing < 2);
}
