#include "RegionConfig.hh"

RegionConfig::RegionConfig() : 
  name(""), 
  leading_jet_pt(0), 
  met(0), 
  required_bits(0), 
  veto_bits(0), 
  systematic(syst::NONE), 
  output_name(""), 
  type(reg::SIGNAL), 		// will be blinded by default
  hists(reg::HISTMILL)
{
}
