#ifndef BTAG_BUFFER_HH
#define BTAG_BUFFER_HH

class TTree; 

#include <string> 

struct BtagBuffer 
{
  double scale_factor; 
  double scale_factor_err; 
  BtagBuffer(TTree*, std::string branch); 
private: 
  void set(TTree*, std::string branch, void* branch_address); 
};

#endif
