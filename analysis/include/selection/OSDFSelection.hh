#ifndef OSDF_SELECTION_HH
#define OSDF_SELECTION_HH

#include "ISelection.hh"
#include "region_enums.hh"

struct RegionConfig;

class NMinusOSDFSelection : public ISelection {
public:
  NMinusOSDFSelection(const RegionConfig&);
  virtual ~NMinusOSDFSelection();
  virtual bool pass(const EventObjects&) const;
private:
  reg::Stream m_stream;
};


class OSDFSelection : public ISelection {
public:
  OSDFSelection(const RegionConfig&);
  virtual ~OSDFSelection();
  virtual bool pass(const EventObjects&) const;
private:
  NMinusOSDFSelection m_nminus;
};

#endif
