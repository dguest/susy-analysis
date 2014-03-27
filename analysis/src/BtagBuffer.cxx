#include "BtagBuffer.hh"
#include "TTree.h"
#include <stdexcept>

BtagBuffer::BtagBuffer(TTree* tree, std::string branch):
  m_has_err(false)
{
  set(tree, branch, &m_scale_factor);
  std::string err_branch = branch + "_err";
  if (tree->GetBranch(err_branch.c_str())) {
    set(tree, branch + "_err", &m_scale_factor_err);
    m_has_err = true;
  }
}

double BtagBuffer::sf(TagSF tag_sf) const {
  if (tag_sf == TagSF::NOMINAL) return m_scale_factor;
  if (!m_has_err) throw std::runtime_error("no tagging SF error provided");
  switch (tag_sf) {
  case TagSF::UP: return m_scale_factor + m_scale_factor_err;
  case TagSF::DOWN: return m_scale_factor - m_scale_factor_err;
  default: throw std::logic_error("soemthign wrong in " __FILE__);
  }
}

void BtagBuffer::set(TTree* tree, std::string branch, void* address) {
  unsigned ret_code;
  tree->SetBranchStatus(branch.c_str(), 1, &ret_code);
  if (ret_code != 1) {
    throw std::runtime_error("branch: " + branch +
			     ", where the fuck is it?");
  }
  tree->SetBranchAddress(branch.c_str(), address);
}

