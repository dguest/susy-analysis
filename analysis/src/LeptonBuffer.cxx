#include "LeptonBuffer.hh"
#include "Lepton.hh"
#include "TTree.h"

#include "common_functions.hh" // set_branch

#include <cassert>

LeptonBuffer::LeptonBuffer(TTree* tree, std::string base_name) {

#define SET_BRANCH(NAME) \
  set_branch(tree,base_name + #NAME , &m_ ## NAME);

  SET_BRANCH(pt);
  SET_BRANCH(eta);
  SET_BRANCH(phi);
  SET_BRANCH(pdgid);

#undef SET_BRANCH
}

bool LeptonBuffer::has_lepton() const {
  return m_pt > 0;
}

Lepton LeptonBuffer::lepton() const {
  assert(has_lepton());
  return Lepton(m_pt, m_eta, m_phi, m_pdgid);
}
