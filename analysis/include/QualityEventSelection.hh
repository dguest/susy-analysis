#ifndef QUALITY_EVENT_SELECTION_HH
#define QUALITY_EVENT_SELECTION_HH

#include "ISelection.hh"

struct RegionConfig; 

class QualityEventSelection : public ISelection { 
public: 
  QualityEventSelection(const RegionConfig&); 
  virtual ~QualityEventSelection(); 
  virtual bool pass(const EventObjects&) const; 
}; 

#endif 
