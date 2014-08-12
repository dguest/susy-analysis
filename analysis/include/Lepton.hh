#ifndef LEPTON_HH
#define LEPTON_HH

#include "TLorentzVector.h"

class Lepton: public TLorentzVector
{
public:
  Lepton(float pt, float eta, float phi, int pdgid);
  bool sf(const Lepton&) const;
  bool ss(const Lepton&) const;
  bool ossf(const Lepton&) const;
private:
  int m_pdgid;
};

#endif
