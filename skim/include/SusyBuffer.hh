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

enum class Save { IF_LISTED, ALWAYS, NEVER }; 

struct Triggers { 
  bool EF_xe80_tclcw_tight; 	// from stop -> charm search (not sure why..)
  bool EF_xe80T_tclcw_loose; 	// missing the first few bunch trains
  bool EF_xe80_tclcw_loose; 	// missing in the beginning of year
  bool EF_xe80T_tclcw; // not sure if we need this

  bool EF_e24vhi_medium1; 
  bool EF_e60_medium1; 
  bool EF_2e12Tvh_loose1; 

  bool EF_mu18_tight_mu8_EFFS; 	// for Z cr?
  bool EF_mu24i_tight; 		// isolated muon
  bool EF_mu36_tight;          	// generic muon
}; 

struct Met { 
  float etx; 
  float ety; 
  float muon_etx; 
  float muon_ety; 
}; 

class SusyBuffer { 
public: 
  SusyBuffer(TChain&, const std::vector<std::string>&); 
  ~SusyBuffer(); 
  std::set<std::string> getExposedInputs() const; 
  std::set<std::string> getMissingBranches() const; 
  void setPassThrough(TTree&) const; 
  bool hasMc() const; 
  void dump() const; 

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

  // skimming triggers
  Triggers triggers; 

  // met (we check to make sure there's also met in skimmed events which 
  // fire the met trigger)
  Met met; 

private: 
  // generic branch setters
  template<typename T> 
  void set(TChain& chain, const std::string& name, T* ptr, 
	   Save save = Save::IF_LISTED); 
  void setInternal(TChain& chain, const std::string& name, void* dest); 

  // specific branch setters (call the above functions)
  void setMcBranches(TChain& chain); 
  void setTriggerBranches(TChain& chain); 
  void setMetBranches(TChain& chain); 

  // all "out branches" are set with setPassThrough 
  std::map<std::string, ITreeBranch*> m_out_branches; 
  // all requested branches (via text file)
  const std::set<std::string> m_requested_passthrough; 
  // exposed inputs are set to some local buffer (not just passing though)
  std::set<std::string> m_exposed_inputs; 
  std::set<std::string> m_missing_inputs; 
  bool m_has_mc; 
  bool m_has_xe80_tclcw_tight; 
}; 

class VariableTranscriptionError: public std::logic_error
{
public: 
  VariableTranscriptionError(const std::string& what, 
			     const std::string& var);
private: 
  std::string m_variable; 
};

#endif
