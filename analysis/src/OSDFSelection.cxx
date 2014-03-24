#include "OSDFSelection.hh"
#include "RegionConfig.hh"
#include "EventObjects.hh"
#include "constants_scharmcuts.hh"
#include "trigger_logic.hh"	// trig namespace

#include <stdexcept>

// _______________________________________________________________________
// loose

NMinusOSDFSelection::NMinusOSDFSelection(const RegionConfig& reg): 
  m_stream(reg.stream)
{ 
  trig::throw_if_not_lepstream(m_stream);
}

NMinusOSDFSelection::~NMinusOSDFSelection() { 

}

bool NMinusOSDFSelection::pass(const EventObjects& obj) const { 
  
  const EventRecoParameters& reco = obj.reco; 
  // check trigger
  if (!trig::pass_single_lepton_trigger(reco, m_stream)) return false;

  if (!reco.pass_osdf) return false; 

  // check object counts
  if (reco.n_signal_jets < 2) return false; 
  // veto el / mu are the same as preselected (may add isolation at 
  // some point)
  if (reco.n_baseline_electrons + reco.n_baseline_muons != 2) return false;

  return true;
}


// _______________________________________________________________________
// tight


OSDFSelection::OSDFSelection(const RegionConfig& cfg): 
  m_nminus(cfg)
{ 
}

OSDFSelection::~OSDFSelection() { 

}

bool OSDFSelection::pass(const EventObjects& obj) const { 
  throw std::logic_error("deprecated in " __FILE__);

  if (!m_nminus.pass(obj)) return false;
  
  const EventRecoParameters& reco = obj.reco; 

  // other parameters
  if (obj.jets.size() > 2) { 
    if (obj.jets.at(2).Pt() > SIGNAL_JET_3_MAX_PT) return false; 
  }

  using namespace crdf; 
  if (reco.min_jetmet_dphi < MIN_DPHI_JET_MET) return false; 
  if (reco.mct < MCT_MIN) return false; 
  if (reco.mll < M_LL_MIN) return false; 
  

  return true; 
}

