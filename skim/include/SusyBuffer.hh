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
  std::set<std::string> getExposedInputs() const; 
  std::set<std::string> getMissingBranches() const; 
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

  // skimming triggers
  bool xe80_tclcw_tight; 	// from stop -> charm search (not sure why..)
  bool xe80T_tclcw_loose; 	// missing the first few bunch trains
  bool xe80_tclcw_loose; 	// missing in the beginning of year
  bool mu18_tight_mu8_EFFS; 	// for Z cr?
  bool mu24i_tight; 		// isolated muon
  bool mu36_tight;          	// generic muon

  // met (we check to make sure there's also met in skimmed events which 
  // fire the met trigger)
  float met_etx; 
  float met_ety; 
  float met_muon_etx; 
  float met_muon_ety; 

private: 
  // generic branch setters
  template<typename T> 
  void set(TChain& chain, const std::string& name, T ptr, bool save = false); 
  void setInternal(TChain& chain, const std::string& name, void* dest); 

  // specific branch setters (call the above functions)
  void setMcBranches(TChain& chain); 
  void setTriggerBranches(TChain& chain); 
  void setMetBranches(TChain& chain); 

  // assorted private data
  std::map<std::string, ITreeBranch*> m_tree_branches; 
  const std::set<std::string> m_requested_passthrough; 
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
  ~VariableTranscriptionError() throw () {} 
private: 
  std::string m_variable; 
};

#endif
