#include "CR1LSelection.hh"
#include "RegionConfig.hh"
#include "EventObjects.hh"
#include "constants_scharmcuts.hh"
#include "trigger_logic.hh"	// trig namespace

// _______________________________________________________________________
// loose

NMinusCR1LSelection::NMinusCR1LSelection(const RegionConfig& reg):
  m_stream(reg.stream){ 
  trig::throw_if_not_lepstream(m_stream);
}

NMinusCR1LSelection::~NMinusCR1LSelection() { 

}

bool NMinusCR1LSelection::pass(const EventObjects& obj) const { 
  const EventRecoParameters& reco = obj.reco; 

  // check trigger
  if (!trig::pass_single_lepton_trigger(reco, m_stream)) return false;

  // check object counts
  auto veto_leptons = reco.n_veto_electrons + reco.n_veto_muons; 
  if (veto_leptons != 1) return false;
  auto total_leptons = reco.n_control_electrons + reco.n_control_muons; 
  if (total_leptons != 1) return false; 
  if (reco.n_signal_jets < 2) return false; 

  return true; 
}

// _______________________________________________________________________
// tight

CR1LSelection::CR1LSelection(const RegionConfig& config) : 
  m_nminus(config)
{ 
}

CR1LSelection::~CR1LSelection() { 

}

bool CR1LSelection::pass(const EventObjects& obj) const { 

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

