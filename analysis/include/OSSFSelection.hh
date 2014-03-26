#ifndef OSSF_SELECTION_HH
#define OSSF_SELECTION_HH

#include "ISelection.hh"
#include "region_enums.hh"

struct RegionConfig;

class NMinusOSSFSelection : public ISelection {
public:
  NMinusOSSFSelection(const RegionConfig&);
  virtual ~NMinusOSSFSelection();
  virtual bool pass(const EventObjects&) const;
private:
  reg::Stream m_stream;
};


class OSSFSelection : public ISelection {
public:
  OSSFSelection(const RegionConfig&);
  virtual ~OSSFSelection();
  virtual bool pass(const EventObjects&) const;
private:
  NMinusOSSFSelection m_nminus;
};

#endif
