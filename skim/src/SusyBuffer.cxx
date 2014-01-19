#include "SusyBuffer.hh"

#include <vector> 
#include <string> 

#include "TChain.h"

namespace { 
  void setOrThrow(TChain& chain, const std::string& name, void* variable); 
}

SusyBuffer::SusyBuffer(TChain& chain) { 
  mcevt_weight = 0; 
  setOrThrow(chain, "mcevt_weight", &mcevt_weight); 
}

namespace { 
  void setOrThrow(TChain& chain, const std::string& name, void* variable) { 
    unsigned branches_found = 0; 
    chain.SetBranchStatus(name.c_str(), 1, &branches_found);
    if (branches_found != 1) { 
      throw MissingBranchError("can't find branch: " + name); 
    }
    chain.SetBranchAddress(name.c_str(), variable); 
  }
}


MissingBranchError::MissingBranchError(const std::string& what): 
  std::runtime_error(what)
{ 
}
