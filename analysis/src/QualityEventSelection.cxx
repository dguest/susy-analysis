#include "QualityEventSelection.hh"
#include "RegionConfig.hh"
#include "EventObjects.hh"
#include "constants_scharmcuts.hh"


QualityEventSelection::QualityEventSelection(const RegionConfig& ) { 
  
}

QualityEventSelection::~QualityEventSelection() { 

}

bool QualityEventSelection::pass(const EventObjects& obj) const { 
  

  return true; 
}
