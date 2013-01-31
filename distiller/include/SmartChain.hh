#ifndef SMART_CHAIN_H
#define SMART_CHAIN_H

#include "TChain.h"
#include <string> 
#include <vector> 
#include <set> 

class SmartChain: public TChain { 
public: 
  SmartChain(std::string tree_name); 
  int Add(std::string file_name, int nentries = -1); 
  int GetEntry(int entry_n); 
  template<typename T, typename Z>
  void SetBranchAddress(T name, Z branch, 
  			bool turnon = false); 

  std::vector<std::string> get_all_branch_names() const; 
private: 
  typedef std::vector<std::string> Strings; 
  void SetBranchAddressPrivate(std::string name, void* branch, 
			       bool turnon = false); 
  Strings get_bad_files() const; 
  void throw_bad_branch(std::string name) const; 
  Strings m_set_branches; 
  std::set<std::string> m_set_branch_set; 
  Strings m_files; 
  int m_last_tree; 		// to trigger branch check 
}; 

// -------- implementation -----


template<typename T, typename Z>
void SmartChain::SetBranchAddress(T name, Z branch, 
				  bool turnon) { 
  *branch = 0; 
  SetBranchAddressPrivate(name, branch, turnon); 
}


#endif //SMART_CHAIN_H
