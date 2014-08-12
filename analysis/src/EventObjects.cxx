#include "EventObjects.hh"
#include "HistBuilderFlags.hh"
#include "ObjectFactory.hh"
#include "systematic_defs.hh"
#include <stdexcept>

namespace {
  typedef std::vector<Jet> Jets;
  Jets remove_electron_jets(Jets);
}


EventObjects::EventObjects(const ObjectFactory* factory,
			   unsigned build_flags):
  met(factory->met()),
  weight(factory->event_weight()),
  reco(factory->event_reco_parameters()),
  leading_cjet_pos(0),
  subleading_cjet_pos(0),
  jets(factory->jets()),
  leptons(factory->leptons()),
  event_scalefactors(0),
  true_boson_pt(factory->boson_pt()),
  marks_boson_pt_weight(factory->marks_boson_pt_weight()),
  boson_child(factory->boson_child())
{
  if ( !(build_flags & buildflag::is_data)) {
    leading_cjet_pos = factory->leading_cjet_pos();
    subleading_cjet_pos = factory->subleading_cjet_pos();
    event_scalefactors = factory->event_scalefactors();
  }

}

namespace {
  Jets remove_electron_jets(Jets jets) {
    Jets repl_jets;
    for (auto jet = jets.begin(); jet != jets.end(); jet++) {
      if (! jet->is_electron_jet()) {
	repl_jets.push_back(*jet);
      }
    }
    return repl_jets;
  }

}
