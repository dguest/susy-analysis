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
  ~SusyBuffer(); 
  std::vector<std::vector<double> >* mcevt_weight; 
  std::vector<std::vector<float> >* dummy; 
private: 
  std::map<std::string, ITreeBranch*> m_tree_branches; 
}; 

// #include "TChain.h"
// #include "TreeBranch.hh"

// namespace {
//   using namespace std;
//   class Dummy {
//     vector<vector<float> > vfloat; 
//     TChain chain; 
//     TreeBranch<vector<vector<float> > > bf; 
//   }; 
// }

#endif
