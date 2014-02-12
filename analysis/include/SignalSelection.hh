#ifndef SIGNAL_SELECTION_HH
#define SIGNAL_SELECTION_HH

#include "ISelection.hh"

struct RegionConfig; 

class SignalSelection : public ISelection { 
public: 
  SignalSelection(const RegionConfig&); 
  virtual ~SignalSelection(); 
  virtual bool pass(const EventObjects&) const; 
}; 

#endif 
