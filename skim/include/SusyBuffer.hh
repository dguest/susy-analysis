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
  bool hasMc() const; 

  // we can cut this branch down to one entry per event
  std::vector<std::vector<double> >* mcevt_weight; 

  // for boson truth filter
  int mc_n; 
  std::vector<float>* mc_pt; 
  std::vector<float>* mc_eta; 
  std::vector<float>* mc_phi; 
  std::vector<float>* mc_m; 
  std::vector<int>* mc_status; 
  std::vector<int>* mc_pdgId; 

private: 
  template<typename T> 
  void set(TChain& chain, const std::string& name, T ptr); 
  void setInternal(TChain& chain, const std::string& name, void* dest); 
  std::map<std::string, ITreeBranch*> m_tree_branches; 
  std::set<std::string> m_set_inputs; 
  bool m_has_mc; 
}; 


#endif
