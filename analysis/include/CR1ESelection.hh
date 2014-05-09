#ifndef CR1E_SELECTION_HH
#define CR1E_SELECTION_HH

#include "ISelection.hh"
#include "region_enums.hh"

struct RegionConfig;

// looser variant of the full selection, used for n-1 plots
class NMinusCR1ESelection : public ISelection {
public:
  NMinusCR1ESelection(const RegionConfig&);
  virtual ~NMinusCR1ESelection();
  virtual bool pass(const EventObjects&) const;
private:
  reg::Stream m_stream;
};

class CR1ESelection : public ISelection {
public:
  CR1ESelection(const RegionConfig&);
  virtual ~CR1ESelection();
  virtual bool pass(const EventObjects&) const;
private:
  NMinusCR1ESelection m_nminus;
};

#endif
