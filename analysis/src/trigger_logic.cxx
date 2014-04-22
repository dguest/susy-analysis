#include "trigger_logic.hh"
#include "EventRecoParameters.hh"

#include <set>
#include <stdexcept>
#include <string>

namespace {
  // for debugging
  std::string stream_name(reg::Stream);

  // stream overlap removal logic
  bool passes_overlap(reg::Stream, bool el_trig, bool mu_trig);
}

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
    return passes_overlap(stream, el_trig, mu_trig);
  }

  // trigger for the Z cr
  bool pass_two_lepton_trigger(const EventRecoParameters& reco,
			       reg::Stream stream){
    bool mu_trig = reco.pass_two_mu_trigger;
    bool el_trig = reco.pass_two_el_trigger;
    return passes_overlap(stream, el_trig, mu_trig);
  }

  bool pass_any_lepton_trigger(const EventRecoParameters& reco,
			       reg::Stream stream){
    bool mu_trig = reco.pass_two_mu_trigger || reco.pass_single_mu_trigger;
    bool el_trig = reco.pass_two_el_trigger || reco.pass_single_el_trigger;
    return passes_overlap(stream, el_trig, mu_trig);
  }

}

namespace {
#define NAME_STREAM(name) case Stream::name: return #name
  std::string stream_name(reg::Stream str) {
    using namespace reg;
    switch (str) {
      NAME_STREAM(ELECTRON);
      NAME_STREAM(MUON);
      NAME_STREAM(JET);
      NAME_STREAM(SIMULATED);
      NAME_STREAM(ERROR);
    default: return "unknown";
    }
  }
#undef NAME_STREAM

  bool passes_overlap(reg::Stream stream, bool el_trig, bool mu_trig) {
    if (stream == reg::Stream::SIMULATED) {
      return mu_trig || el_trig;
    }
    if (stream == reg::Stream::MUON) {
      return mu_trig && !el_trig;
    } else if (stream == reg::Stream::ELECTRON ) {
      return el_trig;
    }
    throw std::invalid_argument(
      "lepton trig check doesnt do " + stream_name(stream));
  }

}
