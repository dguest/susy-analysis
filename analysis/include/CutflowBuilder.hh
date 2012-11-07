#ifndef CUTFLOW_BUILDER_HH
#define CUTFLOW_BUILDER_HH

#include <string> 
#include <map>
#include <vector> 
#include <boost/noncopyable.hpp>

class TTree; 
class TFile; 

class CutflowBuilder : public boost::noncopyable
{
public: 
  CutflowBuilder(std::string input, const unsigned flags = 0); 
  ~CutflowBuilder(); 
  void add_cut_mask(std::string, unsigned bits); 
  int build(); 
  void save(std::string output = ""); 
private: 
  const unsigned m_flags; 
  TFile* m_file; 
  TTree* m_tree; 
  unsigned m_mask; 
  unsigned m_pass_bits; 

};


#endif 
