#ifndef CR1L_SELECTION_HH
#define CR1L_SELECTION_HH

#include "ISelection.hh"

struct RegionConfig; 

class CR1LSelection : public ISelection { 
public: 
  CR1LSelection(const RegionConfig&); 
  virtual ~CR1LSelection(); 
  virtual bool pass(const EventObjects&) const; 
}; 

#endif 
