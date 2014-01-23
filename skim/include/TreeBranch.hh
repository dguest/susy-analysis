#ifndef TREE_BRANCH_HH
#define TREE_BRANCH_HH

#include "ITreeBranch.hh"
#include "misc_func.hh"

#include "TTree.h"
#include <string>

// TreeBranch gets set directly from the input chain
template<typename T>
class TreeBranch: public ITreeBranch { 
public: 
  TreeBranch(TTree& in_tree, const std::string& branch_name); 
  TreeBranch(T in_value, const std::string& branch_name, 
	     const std::string& branch_class = ""); 
  void addToTree(TTree& tree); 
private: 
  std::string m_branch_name; 
  T m_value; 
  std::string m_branch_class; 
}; 

// implementation 

template<typename T>
TreeBranch<T>::TreeBranch(TTree& in_tree, const std::string& branch_name): 
  m_branch_name(branch_name),
  m_value(0)
{ 
  setOrThrow(in_tree, branch_name, &m_value); 
  m_branch_class = in_tree.FindBranch(branch_name.c_str())->GetClassName();
}

template<typename T>
TreeBranch<T>::TreeBranch(T in_value, const std::string& branch_name, 
			  const std::string& branch_class) : 
  m_branch_name(branch_name), 
  m_value(in_value), 
  m_branch_class(branch_class)
{ 
}

template<typename T>
void TreeBranch<T>::addToTree(TTree& tree) { 
  if (m_branch_class.size() > 0){ 
    tree.Branch(m_branch_name.c_str(), m_branch_class.c_str(), m_value); 
  } else { 
    tree.Branch(m_branch_name.c_str(), m_value); 
  }
}

#endif 
