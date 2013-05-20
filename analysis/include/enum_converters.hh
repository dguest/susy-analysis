#ifndef ENUM_CONVERTERS_HH
#define ENUM_CONVERTERS_HH

#include "BtagConfig.hh"
#include "JetTagRescaler.hh"

btag::JetTag btag_from_jettag(jettag::TaggingPoint); 
jettag::TaggingPoint jettag_from_btag(btag::JetTag); 

#endif 
