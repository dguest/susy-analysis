#include "SignalSelection.hh"
#include "RegionConfig.hh"
#include "EventObjects.hh"


SignalSelection::SignalSelection(const RegionConfig& ) { 
  
}

SignalSelection::~SignalSelection() { 

}

bool SignalSelection::pass(const EventObjects& ) const { 
  return true; 
}
