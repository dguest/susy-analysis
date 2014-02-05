#ifndef PILEUPREWEIGHTING_HH
#define PILEUPREWEIGHTING_HH

class SusyBuffer; 
class RunInfo; 
namespace Root { 
  class TPileupReweighting; 
}

#include <string> 

class PileupReweighting 
{
public: 
  PileupReweighting(const RunInfo&, unsigned run_flags); 
  PileupReweighting(PileupReweighting&) = delete; 
  PileupReweighting& operator=(PileupReweighting&) = delete; 
  ~PileupReweighting(); 
  float get_pileup_weight(const SusyBuffer&); 
  void write_to(const std::string& file_name); 
private: 
  Root::TPileupReweighting* m_prw; 
  bool m_generate; 
}; 


#endif 
