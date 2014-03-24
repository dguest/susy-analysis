#include "CRZSelection.hh"
#include "RegionConfig.hh"
#include "EventObjects.hh"
#include "constants_scharmcuts.hh"
#include "trigger_logic.hh"	// trig namespace

#include <stdexcept>

// _______________________________________________________________________
// loose

NMinusCRZSelection::NMinusCRZSelection(const RegionConfig& reg): 
  m_stream(reg.stream)
{
  trig::throw_if_not_lepstream(m_stream);
}

NMinusCRZSelection::~NMinusCRZSelection() { 

}

bool NMinusCRZSelection::pass(const EventObjects& obj) const {
  const EventRecoParameters& reco = obj.reco;

  // check trigger
  if (!trig::pass_any_lepton_trigger(reco, m_stream)) return false;

  if (!reco.pass_ossf) return false; 

  // check object counts
  if (reco.n_signal_jets < 2) return false; 
  if (reco.n_baseline_electrons + reco.n_baseline_muons != 2) return false;

  return true;
}


