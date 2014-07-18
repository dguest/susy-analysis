#ifndef CRZ_SELECTION_HH
#define CRZ_SELECTION_HH

// this is like the OSSF selection, but we're allowing single lepton triggers
// in addition to the dilepton triggers used in the "old" ZCR

#include "ISelection.hh"
#include "region_enums.hh"

struct RegionConfig;

class NMinusCRZSelection : public ISelection {
public:
  NMinusCRZSelection(const RegionConfig&);
  virtual ~NMinusCRZSelection();
  virtual bool pass(const EventObjects&) const;
private:
  reg::Stream m_stream;
};

#endif
