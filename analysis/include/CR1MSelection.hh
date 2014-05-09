#ifndef CR1M_SELECTION_HH
#define CR1M_SELECTION_HH

#include "ISelection.hh"
#include "region_enums.hh"

struct RegionConfig;

// looser variant of the full selection, used for n-1 plots
class NMinusCR1MSelection : public ISelection {
public:
  NMinusCR1MSelection(const RegionConfig&);
  virtual ~NMinusCR1MSelection();
  virtual bool pass(const EventObjects&) const;
private:
  reg::Stream m_stream;
};

class CR1MSelection : public ISelection {
public:
  CR1MSelection(const RegionConfig&);
  virtual ~CR1MSelection();
  virtual bool pass(const EventObjects&) const;
private:
  NMinusCR1MSelection m_nminus;
};

#endif
