#ifndef SIGNAL_SELECTION_HH
#define SIGNAL_SELECTION_HH

#include "ISelection.hh"

struct RegionConfig;

// this is a looser version of the selection below,
// used for n-1 hists (and maybe optimization)
class NMinusSignalSelection : public ISelection {
public:
  NMinusSignalSelection(const RegionConfig&);
  virtual ~NMinusSignalSelection();
  virtual bool pass(const EventObjects&) const;
};


class SignalSelection : public ISelection {
public:
  SignalSelection(const RegionConfig&);
  virtual ~SignalSelection();
  virtual bool pass(const EventObjects&) const;
private:
  NMinusSignalSelection m_nminus;
};

#endif
