#ifndef VRMCC_SELECTION_HH
#define VRMCC_SELECTION_HH

#include "ISelection.hh"

struct RegionConfig;

// this is a looser version of the selection below,
// used for n-1 hists (and maybe optimization)
class NMinusVRMCCSelection : public ISelection {
public:
  NMinusVRMCCSelection(const RegionConfig&);
  virtual ~NMinusVRMCCSelection();
  virtual bool pass(const EventObjects&) const;
};


#endif
