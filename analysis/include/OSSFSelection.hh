#ifndef OSSF_SELECTION_HH
#define OSSF_SELECTION_HH

#include "ISelection.hh"

struct RegionConfig; 

class OSSFSelection : public ISelection { 
public: 
  OSSFSelection(const RegionConfig&); 
  virtual ~OSSFSelection(); 
  virtual bool pass(const EventObjects&) const; 
}; 

#endif 
