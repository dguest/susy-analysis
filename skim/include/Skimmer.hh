#ifndef SKIMMER_HH
#define SKIMMER_HH

#include <string> 
#include <vector> 

class TChain; 
class TTree; 
class TFile; 
class SusyBuffer;

struct SummaryParameters { 
  double total_event_weight; 
  long long total_events; 
  long long skimmed_events; 
  bool has_bosons; 
  const bool has_mc; 
  SummaryParameters(bool has_mc); 
  void writeTo(TFile&) const; 
}; 

class Skimmer { 
public: 
  Skimmer(const std::vector<std::string>& vars); 
  ~Skimmer(); 
  void addFile(const std::string& file_name); 
  void makeSkim(const std::string& out_file_name); 
private: 
  Skimmer(const Skimmer&) {}
  Skimmer& operator=(const Skimmer&) { return *this; }

  bool isGoodFile(const std::string& file_name); 
  const char* pfx(const std::string& word); 
  void copyVariablesTo(TTree*, TFile* = 0);

  TChain* m_chain; 
  std::vector<std::string> m_variables; 
  std::string m_skimmed_var_prefix; 
}; 


#endif
