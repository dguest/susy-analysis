#include "OSDFSelection.hh"
#include "RegionConfig.hh"
#include "EventObjects.hh"
#include "constants_scharmcuts.hh"

#include <cassert>
#include <set>

// _______________________________________________________________________
// loose

NMinusOSDFSelection::NMinusOSDFSelection(const RegionConfig& reg): 
  m_stream(reg.stream)
{ 
  using namespace reg;
  const std::set<Stream> valid_streams {
    Stream::MUON, Stream::ELECTRON, Stream::SIMULATED};
  if (!valid_streams.count(m_stream)) {
    throw std::invalid_argument(
      "got bad stream in " __FILE__);
  }
}

NMinusOSDFSelection::~NMinusOSDFSelection() { 

}

bool NMinusOSDFSelection::pass(const EventObjects& obj) const { 
  
  const EventRecoParameters& reco = obj.reco; 
  // check trigger
  if (m_stream == reg::Stream::MUON) { 
    if (! reco.pass_single_mu_trigger ) return false; 
  } else if (m_stream == reg::Stream::ELECTRON ) { 
    if (! reco.pass_single_el_trigger ) return false;
  }
  if (m_stream != reg::Stream::SIMULATED) { 
    // stream overlap removal
    if ( reco.pass_single_el_trigger && reco.pass_single_mu_trigger ) { 
      // if the event fires both triggers, we only take the one from the 
      // electron stream
      if (m_stream == reg::Stream::MUON) return false;
    }
  }

  if (!reco.pass_osdf) return false; 

  // check object counts
  if (reco.n_signal_jets < 2) return false; 

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

