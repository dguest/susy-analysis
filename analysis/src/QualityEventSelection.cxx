#include "QualityEventSelection.hh"
#include "RegionConfig.hh"
#include "EventObjects.hh"
#include "constants_scharmcuts.hh"
#include <stdexcept>

QualityEventSelection::QualityEventSelection(const RegionConfig& reg)
{ 
  if (reg.stream != reg::Stream::JET) { 
    throw std::invalid_argument(
      "streams other than JET aren't defined in " __FILE__);
  }
}

QualityEventSelection::~QualityEventSelection() { 

}

bool QualityEventSelection::pass(const EventObjects& obj) const { 
  return obj.reco.pass_met_trigger; 
}
