#ifndef LEPTON_BUFFER_HH
#define LEPTON_BUFFER_HH

#include <string>

class TTree;
class Lepton;

class LeptonBuffer
{
public:
  LeptonBuffer(TTree* tree, std::string base_name);
  LeptonBuffer(LeptonBuffer&) = delete;
  LeptonBuffer& operator=(LeptonBuffer&) = delete;
  bool has_lepton() const;
  Lepton lepton() const;
private:
  float m_pt;
  float m_eta;
  float m_phi;
  int m_pdgid;
};

#endif
