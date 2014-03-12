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
    bool mu_trig = reco.pass_single_mu_trigger;
    bool el_trig = reco.pass_single_el_trigger;
    if (stream == reg::Stream::SIMULATED) { 
      return mu_trig || el_trig;
    }
    if (stream == reg::Stream::MUON) { 
      return mu_trig && !el_trig;
    } else if (stream == reg::Stream::ELECTRON ) { 
      return el_trig;
    }
    throw std::invalid_argument("got unknown stream in lepton trig check");
  }

  // trigger for the Z cr
  bool pass_two_lepton_trigger(const EventRecoParameters& reco,
			       reg::Stream stream){
    bool mu_trig = reco.pass_two_mu_trigger;
    bool el_trig = reco.pass_two_el_trigger;
    if (stream == reg::Stream::SIMULATED) { 
      return mu_trig || el_trig;
    }
    if (stream == reg::Stream::MUON) { 
      return mu_trig && !el_trig;
    } else if (stream == reg::Stream::ELECTRON ) { 
      return el_trig;
    }
    throw std::invalid_argument("got unknown stream in 2 lep trig check");
  }

}
