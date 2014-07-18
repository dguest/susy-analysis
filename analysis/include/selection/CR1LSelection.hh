#ifndef CR1L_SELECTION_HH
#define CR1L_SELECTION_HH

#include "ISelection.hh"
#include "region_enums.hh"

struct RegionConfig;

// looser variant of the full selection, used for n-1 plots
class NMinusCR1LSelection : public ISelection {
public:
  NMinusCR1LSelection(const RegionConfig&);
  virtual ~NMinusCR1LSelection();
  virtual bool pass(const EventObjects&) const;
private:
  reg::Stream m_stream;
};

class CR1LSelection : public ISelection {
public:
  CR1LSelection(const RegionConfig&);
  virtual ~CR1LSelection();
  virtual bool pass(const EventObjects&) const;
private:
  NMinusCR1LSelection m_nminus;
};

#endif
