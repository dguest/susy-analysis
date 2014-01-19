// -*- c++ -*-
#ifndef SUSY_BUFFER_HH
#define SUSY_BUFFER_HH

class TChain; 
class ITreeBranch; 

#include <vector> 
#include <map>
#include <string> 
#include <stdexcept>

class SusyBuffer { 
public: 
  SusyBuffer(TChain&, const std::vector<std::string>&); 
  std::vector<std::vector<double> >* mcevt_weight; 
private: 
  std::map<std::string, ITreeBranch*> m_tree_branches; 
}; 


#endif
