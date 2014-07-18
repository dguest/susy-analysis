#ifndef VRMCT_SELECTION_HH
#define VRMCT_SELECTION_HH

#include "ISelection.hh"

struct RegionConfig;

// this is a looser version of the selection below,
// used for n-1 hists (and maybe optimization)
class NMinusVRMCTSelection : public ISelection {
public:
  NMinusVRMCTSelection(const RegionConfig&);
  virtual ~NMinusVRMCTSelection();
  virtual bool pass(const EventObjects&) const;
};


#endif
