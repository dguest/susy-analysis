#include "SusyBuffer.hh"
#include "misc_func.hh"		// setOrThrow
#include "TreeBranch.hh"

#include <vector> 
#include <string> 
#include <utility>

#include "TChain.h"
#include "TLeaf.h"

namespace {
  ITreeBranch* getBranchBuffer(TChain&, const std::string& name); 
}

SusyBuffer::SusyBuffer(TChain& chain, 
		       const std::vector<std::string>& variables): 
  m_requested_passthrough(variables.begin(), variables.end()), 
  m_has_mc(true), 
  m_has_xe80_tclcw_tight(true)
{ 
  setMcBranches(chain); 
  setTriggerBranches(chain); 

  for (const auto itr: variables) { 
    if (!m_exposed_inputs.count(itr) ) { 
      m_tree_branches.insert(
	std::make_pair(itr,getBranchBuffer(chain, itr))); 
      if (!m_tree_branches.find(itr)->second) { 
	m_tree_branches.erase(itr); 
	m_missing_inputs.insert(itr); 
      }
    }
  }
}

SusyBuffer::~SusyBuffer() { 
  for (auto itr: m_tree_branches) { 
    delete itr.second; 
  }
}

std::set<std::string> SusyBuffer::getExposedInputs() const { 
  return m_exposed_inputs; 
}

std::set<std::string> SusyBuffer::getMissingBranches() const { 
  return m_missing_inputs; 
}

void SusyBuffer::setPassThrough(TTree& target) const { 
  for (const auto itr: m_tree_branches) { 
    itr.second->addToTree(target); 
  }
}

bool SusyBuffer::hasMc() const { 
  return m_has_mc; 
}

// ========== private stuff ==========

// generic branch setting functions
template<typename T> 
void SusyBuffer::set(TChain& ch, const std::string& name, T ptr, bool save)
{ 
  *ptr = 0; 
  setInternal(ch, name, ptr); 
  if (save || m_requested_passthrough.count(name)) { 
    std::string br_class = ch.FindBranch(name.c_str())->GetClassName();
    m_tree_branches.insert(
      std::make_pair(name, new TreeBranch<T>(ptr, name, br_class))); 
  }
}

void SusyBuffer::setInternal(TChain& chain, const std::string& name, 
			     void* val) { 
  setOrThrow(chain, name, val); 
  m_exposed_inputs.insert(name); 
}

// specific branch setting functions
void SusyBuffer::setMcBranches(TChain& chain) { 
  try {
    set(chain, "mcevt_weight", &mcevt_weight); 
  } catch (MissingBranchError& e) { 
    m_has_mc = false; 
  }
  if (m_has_mc) { 
    set(chain, "mc_n", &mc_n); 
    set(chain, "mc_pt", &mc_pt); 
    set(chain, "mc_eta", &mc_eta); 
    set(chain, "mc_phi", &mc_phi); 
    set(chain, "mc_m", &mc_m); 
    set(chain, "mc_status", &mc_status); 
    set(chain, "mc_pdgId", &mc_pdgId); 
  }
}

void SusyBuffer::setTriggerBranches(TChain& chain) { 
  try { 
    set(chain, "EF_xe80_tclcw_loose", &xe80_tclcw_tight, true); 
  } catch (MissingBranchError& err) { 
    m_has_xe80_tclcw_tight = false; 
    xe80_tclcw_tight = false; 
  }
  set(chain, "EF_xe80T_tclcw_loose", &xe80T_tclcw_loose, true); 
  set(chain, "EF_xe80_tclcw_tight", &xe80_tclcw_tight, true); 
  set(chain, "EF_mu18_tight_mu8_EFFS", &mu18_tight_mu8_EFFS, true); 
  set(chain, "EF_mu24i_tight", &mu24i_tight, true); 
  set(chain, "EF_mu36_tight", &mu36_tight, true); 
}

void SusyBuffer::setMetBranches(TChain& chain) { 
  set(chain, "MET_Egamma10NoTau_RefFinal_etx", &met_etx); 
  set(chain, "MET_Egamma10NoTau_RefFinal_ety", &met_ety); 
  set(chain, "MET_Egamma10NoTau_Muon_Total_Staco_etx", &met_muon_etx); 
  set(chain, "MET_Egamma10NoTau_Muon_Total_Staco_ety", &met_muon_ety); 
}

// macro to use in getBranchBuffer below
#define TRY_BRANCH_TYPE(TYPE) \
  do { if (branch_type == #TYPE) {			\
      return new TreeBranch<TYPE*> (chain, name);	\
    }							\
  } while (0)


namespace {
  ITreeBranch* getBranchBuffer(TChain& chain, const std::string& name) { 
    TLeaf* leaf = chain.GetLeaf(name.c_str()); 
    if (!leaf) { 
      return 0; 
    }
    std::string branch_type = leaf->GetTypeName(); 

    using namespace std; 
    TRY_BRANCH_TYPE(Float_t); 
    TRY_BRANCH_TYPE(UInt_t); 
    TRY_BRANCH_TYPE(Bool_t); 
    TRY_BRANCH_TYPE(Int_t); 

    TRY_BRANCH_TYPE(vector<int>); 
    TRY_BRANCH_TYPE(vector<unsigned int>); 
    TRY_BRANCH_TYPE(vector<float>); 
    TRY_BRANCH_TYPE(vector<double>); 
    TRY_BRANCH_TYPE(vector<vector<int> >); 
    TRY_BRANCH_TYPE(vector<vector<unsigned int> >); 
    TRY_BRANCH_TYPE(vector<vector<float> >); 
    TRY_BRANCH_TYPE(vector<vector<double> >); 
    
    throw std::domain_error("branch type " + branch_type + 
			    " is not defined in " __FILE__); 
    return 0; 
  }


}


VariableTranscriptionError
::VariableTranscriptionError(const std::string& what, 
			     const std::string& var): 
  std::logic_error(what + " (variable: " + var + " )"), 
  m_variable(var) 
{
  
}

