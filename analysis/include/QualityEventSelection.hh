#ifndef QUALITY_EVENT_SELECTION_HH
#define QUALITY_EVENT_SELECTION_HH

#include "ISelection.hh"
#include "region_enums.hh"

struct RegionConfig; 

class QualityEventSelection : public ISelection { 
public: 
  QualityEventSelection(const RegionConfig&); 
  virtual ~QualityEventSelection(); 
  virtual bool pass(const EventObjects&) const; 
private:
  reg::Stream m_stream;
}; 

#endif 
