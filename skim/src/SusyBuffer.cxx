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
		       const std::vector<std::string>& variables) { 

  mcevt_weight = 0; 
  setOrThrow(chain, "mcevt_weight", &mcevt_weight); 
  for (std::vector<std::string>::const_iterator itr = variables.begin(); 
       itr != variables.end(); itr++) { 
    m_tree_branches.insert(
      std::make_pair(*itr,getBranchBuffer(chain, *itr))); 
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

