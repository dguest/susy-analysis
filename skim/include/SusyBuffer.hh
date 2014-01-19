#ifndef SUSY_BUFFER_HH
#define SUSY_BUFFER_HH

class TChain; 

#include <vector> 
#include <stdexcept>

class SusyBuffer { 
public: 
  SusyBuffer(TChain&); 
  std::vector<std::vector<double> >* mcevt_weight; 
}; 

class MissingBranchError: public std::runtime_error
{
public: 
  MissingBranchError(const std::string&); 
}; 


#endif
