#include "BtagBuffer.hh"
#include "TTree.h"
#include <stdexcept>

BtagBuffer::BtagBuffer(TTree* tree, std::string branch) { 
  set(tree, branch, &scale_factor); 
  set(tree, branch + "_err", &scale_factor_err); 
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

