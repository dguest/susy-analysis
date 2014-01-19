#include "misc_func.hh"
#include "TTree.h"

#include <vector> 
#include <string> 

void setOrThrow(TTree& chain, const std::string& name, void* variable) { 
  unsigned branches_found = 0; 
  chain.SetBranchStatus(name.c_str(), 1, &branches_found);
  if (branches_found != 1) { 
    throw MissingBranchError("can't find branch: " + name); 
  }
  chain.SetBranchAddress(name.c_str(), variable); 
}

MissingBranchError::MissingBranchError(const std::string& what): 
  std::runtime_error(what)
{ 
}
