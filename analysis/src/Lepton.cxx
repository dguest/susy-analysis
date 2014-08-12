#include "Lepton.hh"
#include <cmath>

Lepton::Lepton(float pt, float eta, float phi, int pdgid):
  m_pdgid(pdgid)
{
  SetPtEtaPhiM(pt, eta, phi, 0);
}

bool Lepton::sf(const Lepton& lep) const {
  return std::abs(m_pdgid) == std::abs(lep.m_pdgid);
}
bool Lepton::ss(const Lepton& lep) const {
  return m_pdgid * lep.m_pdgid > 0.0;
}
bool Lepton::ossf(const Lepton& lep) const {
  return sf(lep) && !ss(lep);
}
