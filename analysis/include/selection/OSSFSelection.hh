#ifndef OSSF_SELECTION_HH
#define OSSF_SELECTION_HH

#include "ISelection.hh"
#include "region_enums.hh"

struct RegionConfig;

class NMinusZ2LSelection : public ISelection {
public:
  NMinusZ2LSelection(const RegionConfig&);
  virtual ~NMinusZ2LSelection();
  virtual bool pass(const EventObjects&) const;
private:
  reg::Stream m_stream;
};

class NMinusZ1LSelection : public ISelection {
public:
  NMinusZ1LSelection(const RegionConfig&);
  virtual ~NMinusZ1LSelection();
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
  NMinusZ2LSelection m_nminus;
};

#endif
