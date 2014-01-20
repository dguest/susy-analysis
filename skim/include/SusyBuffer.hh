// -*- c++ -*-
#ifndef SUSY_BUFFER_HH
#define SUSY_BUFFER_HH

class TChain; 
class ITreeBranch; 
class TTree; 

#include <vector> 
#include <map>
#include <set>
#include <string> 
#include <stdexcept>

class SusyBuffer { 
public: 
  SusyBuffer(TChain&, const std::vector<std::string>&); 
  ~SusyBuffer(); 
  void setPassThrough(TTree&) const; 
  std::vector<std::vector<double> >* mcevt_weight; 
private: 
  void set(TChain& chain, const std::string& name, void* dest); 
  std::map<std::string, ITreeBranch*> m_tree_branches; 
  std::set<std::string> m_set_inputs; 
}; 


#endif
