#ifndef TREE_BRANCH_HH
#define TREE_BRANCH_HH

#include "misc_func.hh"

// class TTree; 
#include "TTree.h"
#include <string>

class ITreeBranch { 
public: 
  virtual ~ITreeBranch() {}
  virtual void addToTree(TTree&) = 0; 
}; 

template<typename T>
class TreeBranch: public ITreeBranch { 
public: 
  TreeBranch(TTree& in_tree, const std::string& branch_name); 
  void addToTree(TTree& tree); 
private: 
  std::string m_branch_name; 
  T m_value; 
}; 

// implementation 

template<typename T>
TreeBranch<T>::TreeBranch(TTree& in_tree, const std::string& branch_name): 
  m_branch_name(branch_name),
  m_value(0)
{ 
  setOrThrow(in_tree, branch_name, &m_value); 
}

template<typename T>
void TreeBranch<T>::addToTree(TTree& tree) { 
  tree.Branch(m_branch_name.c_str(), &m_value); 
}


#endif 
