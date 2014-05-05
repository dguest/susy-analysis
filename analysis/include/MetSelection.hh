#ifndef MET_SELECTION_HH
#define MET_SELECTION_HH

#include "ISelection.hh"
#include "region_enums.hh"

struct RegionConfig;

class MetSelection : public ISelection {
public:
  MetSelection(const RegionConfig&);
  virtual ~MetSelection();
  virtual bool pass(const EventObjects&) const;
private:
  reg::Stream m_stream;
};

#endif
