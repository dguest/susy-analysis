#ifndef TREE_BRANCH_HH
#define TREE_BRANCH_HH

#include "ITreeBranch.hh"
#include "misc_func.hh"

#include "TTree.h"
#include <string>

// --- flat branch for pointers to simple types ---

template<typename T>
class FlatBranch: public ITreeBranch {
public:
  FlatBranch(T* in_ptr, const std::string& branch_name);
  void addToTree(TTree& tree);
  void dump() const;
private:
  std::string m_branch_name;
  T* m_value;
};

template<typename T>
FlatBranch<T>::FlatBranch(T* in_ptr, const std::string& branch_name) :
  m_branch_name(branch_name),
  m_value(in_ptr)
{
}

template<typename T>
void FlatBranch<T>::addToTree(TTree& tree) {
  tree.Branch(m_branch_name.c_str(), m_value);
}

template<typename T>
void FlatBranch<T>::dump() const {
  puts("NADA");
}

// --- owner branch type for simple types passed through ---

template<typename T>
class FlatOwnedBranch: public ITreeBranch {
public:
  FlatOwnedBranch(TTree& in_tree, const std::string& branch_name);
  void addToTree(TTree& tree);
  void dump() const;
private:
  std::string m_branch_name;
  T m_value;
};

template<typename T>
FlatOwnedBranch<T>::FlatOwnedBranch(TTree& in_tree,
				    const std::string& branch_name):
  m_branch_name(branch_name),
  m_value(0)
{
  setOrThrow(in_tree, branch_name, &m_value);
}

template<typename T>
void FlatOwnedBranch<T>::addToTree(TTree& tree) {
  tree.Branch(m_branch_name.c_str(), &m_value);
}

template<typename T>
void FlatOwnedBranch<T>::dump() const {
  puts("NADA");
}




// obj branch for more complicated objects
template<typename T>
class ObjBranch: public ITreeBranch {
public:
  ObjBranch(TTree& in_tree, const std::string& branch_name);
  ObjBranch(T* in_ptr, const std::string& branch_name,
	     const std::string& branch_class = "");
  ~ObjBranch();
  void addToTree(TTree& tree);
  void dump() const;
private:
  std::string m_branch_name;
  T* m_value;
  std::string m_branch_class;
  bool m_owner;
};

template<typename T>
ObjBranch<T>::ObjBranch(TTree& in_tree, const std::string& branch_name):
  m_branch_name(branch_name),
  m_value(new T),
  m_owner(true)
{
  setOrThrow(in_tree, branch_name, &m_value);
  m_branch_class = in_tree.FindBranch(branch_name.c_str())->GetClassName();
}

template<typename T>
ObjBranch<T>::ObjBranch(T* in_ptr, const std::string& branch_name,
			const std::string& branch_class) :
  m_branch_name(branch_name),
  m_value(in_ptr),
  m_branch_class(branch_class),
  m_owner(false)
{
}

template<typename T>
ObjBranch<T>::~ObjBranch() {
  if (m_owner) {
    delete m_value;
  }
}

template<typename T>
void ObjBranch<T>::addToTree(TTree& tree) {
  tree.Branch(m_branch_name.c_str(), m_branch_class.c_str(), m_value);
}

template<typename T>
void ObjBranch<T>::dump() const {
  puts("object");
}


#endif
