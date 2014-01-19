#ifndef SKIMMER_HH
#define SKIMMER_HH

#include <string> 
#include <vector> 

class TChain; 
class TTree; 
class TFile; 

class Skimmer { 
public: 
  Skimmer(const std::vector<std::string>& vars); 
  ~Skimmer(); 
  void addFile(const std::string& file_name); 
  void makeSkim(const std::string& out_file_name); 
private: 
  Skimmer(const Skimmer&) {}
  Skimmer& operator=(const Skimmer&) { return *this; }
  
  const char* pfx(const std::string& word); 
  TTree* copyChain(TFile&); 
  void addMCVairablesTo(TTree*, TFile* = 0); 

  TChain* m_chain; 
  std::vector<std::string> m_variables; 
  std::string m_skimmed_var_prefix; 
}; 


#endif
