#ifndef OSDF_SELECTION_HH
#define OSDF_SELECTION_HH

#include "ISelection.hh"

struct RegionConfig; 

class OSDFSelection : public ISelection { 
public: 
  OSDFSelection(const RegionConfig&); 
  virtual ~OSDFSelection(); 
  virtual bool pass(const EventObjects&) const; 
}; 

#endif 
