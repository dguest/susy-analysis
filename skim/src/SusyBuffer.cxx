#include "SusyBuffer.hh"
#include "misc_func.hh"		// setOrThrow
#include "TreeBranch.hh"

#include <vector> 
#include <string> 
#include <utility>

#include "TChain.h"
#include "TLeaf.h"

namespace {
  ITreeBranch* getBranchBuffer(TChain&, const std::string& name); 
}

SusyBuffer::SusyBuffer(TChain& chain, 
		       const std::vector<std::string>& variables): 
  m_has_mc(true)
{ 
  try {
    set(chain, "mcevt_weight", &mcevt_weight); 
  } catch (MissingBranchError& e) { 
    m_has_mc = false; 
  }
  if (m_has_mc) { 
    set(chain, "mc_n", &mc_n); 
    set(chain, "mc_pt", &mc_pt); 
    set(chain, "mc_eta", &mc_eta); 
    set(chain, "mc_phi", &mc_phi); 
    set(chain, "mc_m", &mc_m); 
    set(chain, "mc_status", &mc_status); 
    set(chain, "mc_pdgId", &mc_pdgId); 
  }
  for (std::vector<std::string>::const_iterator itr = variables.begin(); 
       itr != variables.end(); itr++) { 
    if (m_set_inputs.count(*itr) ) { 
      printf("branch %s can't be passed through, it's already in use\n", 
	     itr->c_str()); 
    } else {
      m_tree_branches.insert(
	std::make_pair(*itr,getBranchBuffer(chain, *itr))); 
    }
  }
}

SusyBuffer::~SusyBuffer() { 
  for (std::map<std::string, ITreeBranch*>::iterator 
	 itr = m_tree_branches.begin(); 
       itr != m_tree_branches.end(); 
       itr++) { 
    delete itr->second; 
  }
}

void SusyBuffer::setPassThrough(TTree& target) const { 
  for (std::map<std::string, ITreeBranch*>::const_iterator 
	 itr = m_tree_branches.begin(); itr != m_tree_branches.end(); itr++){
    if (itr->second) { 
      itr->second->addToTree(target); 
    }
  }
}

bool SusyBuffer::hasMc() const { 
  return m_has_mc; 
}

template<typename T> 
void SusyBuffer::set(TChain& chain, const std::string& name, T ptr) { 
  *ptr = 0; 
  setInternal(chain, name, ptr); 
}

void SusyBuffer::setInternal(TChain& chain, const std::string& name, 
			     void* val) { 
  setOrThrow(chain, name, val); 
  m_set_inputs.insert(name); 
}


// macros to use in getBranchBuffer below
#define TRY_BRANCH_TYPE(TYPE) \
  do { if (branch_type == #TYPE) {			\
      return new TreeBranch<TYPE> (chain, name);	\
    }							\
  } while (0)

#define TRY_BRANCH_V_TYPE(TYPE) \
  do { if (branch_type == #TYPE) {			\
      return new TreeBranch<TYPE*> (chain, name);	\
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
    TRY_BRANCH_TYPE(Float_t); 
    TRY_BRANCH_TYPE(UInt_t); 
    TRY_BRANCH_TYPE(Bool_t); 
    TRY_BRANCH_TYPE(Int_t); 
    TRY_BRANCH_V_TYPE(vector<int>); 
    TRY_BRANCH_V_TYPE(vector<unsigned int>); 
    TRY_BRANCH_V_TYPE(vector<float>); 
    TRY_BRANCH_V_TYPE(vector<double>); 
    TRY_BRANCH_V_TYPE(vector<vector<int> >); 
    TRY_BRANCH_V_TYPE(vector<vector<unsigned int> >); 
    TRY_BRANCH_V_TYPE(vector<vector<float> >); 
    TRY_BRANCH_V_TYPE(vector<vector<double> >); 
    
    throw std::logic_error("can't pass branch type " + branch_type); 
    return 0; 
  }


}

