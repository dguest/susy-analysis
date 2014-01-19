#include "SusyBuffer.h"
#include "misc_func.hh"		// setOrThrow
#include "TreeBranch.hh"

#include <vector> 
#include <string> 

#include "TChain.h"
#include "TBranch.h"

namespace {
  ITreeBranch* getBranchBuffer(TChain&, const std::string& name); 
}

SusyBuffer::SusyBuffer(TChain& chain, 
		       const std::vector<std::string>& variables) { 
  mcevt_weight = 0; 
  getBranchBuffer(chain, "mcevt_weight"); 
  setOrThrow(chain, "mcevt_weight", &mcevt_weight); 
  for (std::vector<std::string>::const_iterator itr = variables.begin(); 
       itr != variables.end(); itr++) { 
    
  }
}

#define TRY_BRANCH_TYPE(TYPE) \
  do { if (branch_type == #TYPE) {			\
      return new TreeBranch<TYPE> (chain, name);	\
    }							\
  } while (0)


namespace {
  ITreeBranch* getBranchBuffer(TChain& chain, const std::string& name) { 
    TBranch* branch = chain.GetBranch(name.c_str()); 
    if (!branch) { 
      throw MissingBranchError("can't find " + name); 
    }
    std::string branch_type = branch->GetClassName(); 

    using namespace std; 
    TRY_BRANCH_TYPE(double); 
    TRY_BRANCH_TYPE(vector<int>); 
    TRY_BRANCH_TYPE(vector<double>); 
    TRY_BRANCH_TYPE(vector<vector<int> >); 
    TRY_BRANCH_TYPE(vector<vector<int> >); 
    
    printf("name: %s, type %s", name.c_str(), branch_type.c_str()); 
    return 0; 
  }
}

