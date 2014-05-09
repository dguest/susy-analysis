#include "CR1MSelection.hh"
#include "RegionConfig.hh"
#include "EventObjects.hh"
#include "constants_scharmcuts.hh"
#include "trigger_logic.hh"	// trig namespace

#include <set>

// _______________________________________________________________________
// loose

NMinusCR1MSelection::NMinusCR1MSelection(const RegionConfig& reg):
  m_stream(reg.stream){
  using namespace reg;
  const std::set<Stream> valid_streams {Stream::MUON, Stream::SIMULATED};
  if (!valid_streams.count(reg.stream)) {
    throw std::invalid_argument(
      "got bad stream, electron or simulated stream required");
  }

}

NMinusCR1MSelection::~NMinusCR1MSelection() {

}

bool NMinusCR1MSelection::pass(const EventObjects& obj) const {
  const EventRecoParameters& reco = obj.reco;

  // check trigger
  if (!trig::pass_single_lepton_trigger(reco, m_stream)) return false;

  // check object counts
  auto veto_leptons = reco.n_baseline_muons;
  if (veto_leptons != 1) return false;
  auto total_leptons = reco.n_signal_muons;
  if (total_leptons != 1) return false;

  return true;
}

// _______________________________________________________________________
// tight

CR1MSelection::CR1MSelection(const RegionConfig& config) :
  m_nminus(config)
{
}

CR1MSelection::~CR1MSelection() {

}

bool CR1MSelection::pass(const EventObjects& obj) const {

  if (!m_nminus.pass(obj)) return false;

  const EventRecoParameters& reco = obj.reco;

  // other parameters
  if (obj.jets.size() > 2) {
    if (obj.jets.at(2).Pt() > SIGNAL_JET_3_MAX_PT) return false;
  }

  using namespace cr1l;

  if (reco.min_jetmet_dphi < MIN_DPHI_JET_MET) return false;

  if (reco.mct < SR_MCT_MIN) return false;

  if (reco.mt > M_T_MAX) return false;
  if (reco.mt < M_T_MIN) return false;

  return true;
}

