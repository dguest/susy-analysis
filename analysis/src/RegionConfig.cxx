#include "RegionConfig.hh"

RegionConfig::RegionConfig() :
  name(""),
  selection(reg::Selection::ERROR),
  jet_tag_assignment(btag::Assignment::ERROR),
  leading_jet_pt(0),
  second_jet_pt(0),
  met(0),
  max_signal_jets(-1),
  systematic(syst::NONE),
  output_name(""),
  type(reg::SIGNAL), 		// will be blinded by default
  hists(reg::HISTMILL),
  tagger(btag::Tagger::ERROR),
  boson_pt_correction(reg::NO_PT_CORRECTION),
  stream(reg::Stream::ERROR)
{
}
