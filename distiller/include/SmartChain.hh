#ifndef SMART_CHAIN_H
#define SMART_CHAIN_H

#include "TChain.h"
#include <string> 
#include <vector> 
#include <set> 
#include "typedefs.hh"

class SmartChain: public TChain { 
public: 
  using TChain::Add; 
  SmartChain(std::string tree_name); 
  virtual int add(std::string file_name, long long nentries = -1); 
  virtual int GetEntry(long long int entry_n, int getall = 0); 
  template<typename T, typename Z>
  void SetBranch(T name, Z branch, 
		 bool turnon = true); 

  std::vector<std::string> get_all_branch_names() const; 
  std::string get_current_file() const; 
private: 
  typedef std::vector<std::string> Strings; 
  void SetBranchAddressPrivate(std::string name, void* branch, 
			       bool turnon = false); 
  void throw_bad_branch(std::string name) const; 
  std::string get_files_string() const; 
  Strings m_set_branches; 
  std::set<std::string> m_set_branch_set; 
  Strings m_files; 
  int m_last_tree; 		// to trigger branch check 
}; 

// -------- implementation -----


template<typename T, typename Z>
void SmartChain::SetBranch(T name, Z branch, bool turnon) { 
  *branch = 0; 
  SetBranchAddressPrivate(name, branch, turnon); 
}


#endif //SMART_CHAIN_H
