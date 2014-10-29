#include "BranchBuffer.hh"

#include "TChain.h"

namespace {
  ITreeBranch* getBranchBuffer(TChain&, const std::string& name);
}

BranchBuffer::BranchBuffer() {}

BranchBuffer::~BranchBuffer() {
  for (auto itr: m_out_branches) {
    delete itr.second;
  }
}

void BranchBuffer::addInputs(TChain& chain,
			     const std::vector<std::string>& variables) {
  // loop over requrested variables
  for (const auto itr: variables) {
    // if they haven't already been processed in the above setting routines
    // we need to add them to the outputs here.
    if (!m_exposed_inputs.count(itr) && !m_out_branches.count(itr) ) {
      m_out_branches.insert(
	std::make_pair(itr,getBranchBuffer(chain, itr)));
      if (!m_out_branches.find(itr)->second) {
	m_out_branches.erase(itr);
	m_missing_inputs.insert(itr);
      }
    }
  }

}

std::set<std::string> BranchBuffer::getExposedInputs() const {
  return m_exposed_inputs;
}

std::set<std::string> BranchBuffer::getMissingBranches() const {
  return m_missing_inputs;
}

void BranchBuffer::setPassThrough(TTree& target) const {
  for (const auto itr: m_out_branches) {
    itr.second->addToTree(target);
  }
}

void BranchBuffer::dump() const {
  puts("dumping...");
  for (auto pair: m_out_branches) {
    printf("branch %s ", pair.first.c_str());
    pair.second->dump();
  }
}


// ========== private stuff ==========

void SusyBuffer::setInternal(TChain& chain, const std::string& name,
			     void* val) {
  setOrThrow(chain, name, val);
  m_exposed_inputs.insert(name);
}

// macro to use in getBranchBuffer below
#define TRY_BRANCH_TYPE(TYPE) \
  do { if (branch_type == #TYPE) {			\
      return new ObjBranch<TYPE> (chain, name);	\
    }							\
  } while (0)

// macro to use in getBranchBuffer below
#define TRY_FLAT_TYPE(TYPE) \
  do { if (branch_type == #TYPE) {			\
      return new FlatOwnedBranch<TYPE> (chain, name);	\
    }							\
  } while (0)


namespace {
  ITreeBranch* getBranchBuffer(TChain& chain, const std::string& name) {
    TLeaf* leaf = chain.GetLeaf(name.c_str());
    if (!leaf) {
      return 0;
    }
    std::string branch_type = leaf->GetTypeName();

    using namespace std;
    TRY_FLAT_TYPE(Float_t);
    TRY_FLAT_TYPE(UInt_t);
    TRY_FLAT_TYPE(Bool_t);
    TRY_FLAT_TYPE(Int_t);

    TRY_BRANCH_TYPE(vector<int>);
    TRY_BRANCH_TYPE(vector<unsigned int>);
    TRY_BRANCH_TYPE(vector<float>);
    TRY_BRANCH_TYPE(vector<double>);
    TRY_BRANCH_TYPE(vector<vector<int> >);
    TRY_BRANCH_TYPE(vector<vector<unsigned int> >);
    TRY_BRANCH_TYPE(vector<vector<float> >);
    TRY_BRANCH_TYPE(vector<vector<double> >);

    throw std::domain_error("branch type " + branch_type +
			    " is not defined in " __FILE__);
    return 0;
  }
}
