#include "trigger_logic.hh"
#include "EventRecoParameters.hh"

#include <set>
#include <stdexcept>

namespace trig {

  void throw_if_not_lepstream(reg::Stream stream) {
    using namespace reg;
    const std::set<Stream> valid_streams {
      Stream::MUON, Stream::ELECTRON, Stream::SIMULATED};
    if (!valid_streams.count(stream)) {
      throw std::invalid_argument(
	"got bad stream, lepton or simulated stream required");
    }
  }

  void throw_if_not_jetstream(reg::Stream stream) {
    using namespace reg;
    const std::set<Stream> valid_streams {Stream::JET, Stream::SIMULATED};
    if (!valid_streams.count(stream)) {
      throw std::invalid_argument(
	"got bad stream, jet or simulated stream required");
    }
  }


  // trigger for the W and t control regions
  bool pass_single_lepton_trigger(const EventRecoParameters& reco, 
				  reg::Stream stream){
    if (stream == reg::Stream::MUON) { 
      if (! reco.pass_single_mu_trigger ) return false; 
    } else if (stream == reg::Stream::ELECTRON ) { 
      if (! reco.pass_single_el_trigger ) return false;
    }
    if (stream != reg::Stream::SIMULATED) { 
      // stream overlap removal
      if ( reco.pass_single_el_trigger && reco.pass_single_mu_trigger ) { 
	// if the event fires both triggers, we only take the one from the 
	// electron stream
	if (stream == reg::Stream::MUON) return false;
      }
    }
    return true;
  }

  // trigger for the Z cr
  bool pass_two_lepton_trigger(const EventRecoParameters& reco,
			       reg::Stream stream){
    if (stream == reg::Stream::MUON) { 
      if (! reco.pass_two_mu_trigger) return false;
    } else if (stream == reg::Stream::ELECTRON ) { 
      if (! reco.pass_two_el_trigger) return false;
    }

    if (stream != reg::Stream::SIMULATED) { 
      // stream overlap removal
      if ( reco.pass_two_el_trigger && reco.pass_two_mu_trigger ) { 
	// if the event fires both triggers, we only take the one from the 
	// electron stream
	if (stream == reg::Stream::MUON) return false;
      }
    }
    return true;
  }

}
