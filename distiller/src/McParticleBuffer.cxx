#include "McParticleBuffer.hh"
#include "SmartChain.hh"

McParticleBuffer::McParticleBuffer():
  m_has_skimmed_info(true)
{}

void McParticleBuffer::set_branches(SmartChain* chain) {
  try {
    chain->SetBranch("skimmed_boson_pt", &skimmed_boson_pt);
  } catch (MissingBranchError& err) {
    m_has_skimmed_info = false;
    chain->SetBranch("mc_n", &mc_n);
    chain->SetBranch("mc_pt", &mc_pt);
    chain->SetBranch("mc_eta", &mc_eta);
    chain->SetBranch("mc_phi", &mc_phi);
    chain->SetBranch("mc_m", &mc_m);

    chain->SetBranch("mc_status", &mc_status);
    chain->SetBranch("mc_pdgId", &mc_pdgId);
  }
}

bool McParticleBuffer::has_skimmed_info() const {
  return m_has_skimmed_info;
}
