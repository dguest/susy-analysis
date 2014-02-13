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
  // warning: works by setting the status of all trigger branches to 0
  bool allTriggersMising(TChain&); 
}

SusyBuffer::SusyBuffer(TChain& chain, 
		       const std::vector<std::string>& variables): 
  m_requested_passthrough(variables.begin(), variables.end()), 
  m_has_mc(true), 
  m_has_xe80_tclcw_tight(true)
{ 
  if (chain.GetEntries() == 0) { 
    throw TolerableDataError("chain is empty, nothing to skim"); 
  }
  if (allTriggersMising(chain)) { 
    throw TolerableDataError("all triggers missing from chain"); 
  }
  // Set local buffers for the inputs. These are counted as "exposed". 
  // Depending on whether branches are requested in 'variables', these
  // may also be added to the list of outputs. 
  setMcBranches(chain); 
  setTriggerBranches(chain); 

  // loop over requrested variables
  for (const auto itr: variables) { 
    // if they haven't already been processed in the above setting routines
    // we need to add them to the outputs here. 
    if (!m_exposed_inputs.count(itr) && !m_out_branches.count(itr) ) { 
      m_out_branches.insert(
	std::make_pair(itr,getBranchBuffer(chain, itr))); 
      if (!m_out_branches.find(itr)->second) { 
	m_out_branches.erase(itr); 
	m_missing_inputs.insert(itr); 
      }
    }
  }
}

SusyBuffer::~SusyBuffer() { 
  for (auto itr: m_out_branches) { 
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
  for (const auto itr: m_out_branches) { 
    itr.second->addToTree(target); 
  }
}

bool SusyBuffer::hasMc() const { 
  return m_has_mc; 
}

void SusyBuffer::dump() const { 
  puts("dumping..."); 
  for (auto pair: m_out_branches) { 
    printf("branch %s ", pair.first.c_str()); 
    pair.second->dump(); 
  }
}

// ========== private stuff ==========

// generic branch setting functions
template<typename T> 
void SusyBuffer::set(TChain& ch, const std::string& name, T* ptr, Save save)
{ 
  *ptr = 0; 
  setInternal(ch, name, ptr); 
  if (save == Save::NEVER) return; 
  bool listed = (
    save == Save::IF_LISTED && m_requested_passthrough.count(name)); 
  if (save == Save::ALWAYS || listed ) { 
    std::string br_class = ch.FindBranch(name.c_str())->GetClassName();
    if (br_class.size() > 0) { 
      m_out_branches.insert(
	std::make_pair(name, new ObjBranch<T>(ptr, name, br_class))); 
    } else { 
      m_out_branches.insert(
	std::make_pair(name, new FlatBranch<T>(ptr, name))); 
    }
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
    set(chain, "mcevt_weight", &mcevt_weight, Save::NEVER); 
  } catch (MissingBranchError& e) { 
    m_has_mc = false; 
  }
  // these branches shouldn't normally never be saved (they are huge)
  const auto should_save_mc = Save::NEVER; 
  if (m_has_mc) { 
    set(chain, "mc_n",      &mc_n     , Save::IF_LISTED); 
    set(chain, "mc_pt",     &mc_pt    , should_save_mc); 
    set(chain, "mc_eta",    &mc_eta   , should_save_mc); 
    set(chain, "mc_phi",    &mc_phi   , should_save_mc); 
    set(chain, "mc_m",      &mc_m     , should_save_mc); 
    set(chain, "mc_status", &mc_status, should_save_mc); 
    set(chain, "mc_pdgId",  &mc_pdgId , should_save_mc); 
  }
}

void SusyBuffer::setTriggerBranches(TChain& chain) { 
  Triggers& tr = triggers; 
  try { 
    set(chain, "EF_xe80_tclcw_loose", &tr.EF_xe80_tclcw_tight, Save::ALWAYS); 
  } catch (MissingBranchError& err) { 
    m_has_xe80_tclcw_tight = false; 
    tr.EF_xe80_tclcw_tight = false; 
  }
  set(chain, "EF_xe80T_tclcw_loose", &tr.EF_xe80T_tclcw_loose, Save::ALWAYS); 
  set(chain, "EF_xe80_tclcw_tight",  &tr.EF_xe80_tclcw_tight, Save::ALWAYS); 
  set(chain, "EF_xe80T_tclcw",         &tr.EF_xe80T_tclcw, Save::ALWAYS); 

  set(chain, "EF_e24vhi_medium1", &tr.EF_e24vhi_medium1, Save::ALWAYS); 
  set(chain, "EF_e60_medium1"   , &tr.EF_e60_medium1, Save::ALWAYS);    
  set(chain, "EF_2e12Tvh_loose1", &tr.EF_2e12Tvh_loose1, Save::ALWAYS); 

  set(chain, "EF_mu18_tight_mu8_EFFS", 
      &tr.EF_mu18_tight_mu8_EFFS, Save::ALWAYS); 
  set(chain, "EF_mu24i_tight",         &tr.EF_mu24i_tight, Save::ALWAYS); 
  set(chain, "EF_mu36_tight",          &tr.EF_mu36_tight, Save::ALWAYS); 
}

void SusyBuffer::setMetBranches(TChain& chain) { 
  set(chain, "MET_Egamma10NoTau_RefFinal_etx", &met.etx); 
  set(chain, "MET_Egamma10NoTau_RefFinal_ety", &met.ety); 
  set(chain, "MET_Egamma10NoTau_Muon_Total_Staco_etx", &met.muon_etx); 
  set(chain, "MET_Egamma10NoTau_Muon_Total_Staco_ety", &met.muon_ety); 
}

// macro to use in getBranchBuffer below
#define TRY_BRANCH_TYPE(TYPE) \
  do { if (branch_type == #TYPE) {			\
      return new ObjBranch<TYPE> (chain, name);	\
    }							\
  } while (0)

// macro to use in getBranchBuffer below
#define TRY_FLAT_TYPE(TYPE) \
  do { if (branch_type == #TYPE) {			\
      return new FlatOwnedBranch<TYPE> (chain, name);	\
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
    TRY_FLAT_TYPE(Float_t); 
    TRY_FLAT_TYPE(UInt_t); 
    TRY_FLAT_TYPE(Bool_t); 
    TRY_FLAT_TYPE(Int_t); 

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

  bool allTriggersMising(TChain& chain) { 
    unsigned int total_found = 0; 
    for (auto itr: {"EF_xe80*", "EF_mu*", "EF_el*"} ) { 

      // this line doesn't make any sense, but somehow it makes 
      // the SetBranchStatus call below do what it's supposed to. 
      // No idea why this works. 
      chain.GetBranch(itr); 

      unsigned int found = 0; 
      chain.SetBranchStatus(itr, 0, &found); 
      if (found) return false; 
      total_found += found; 
    }
    return true; 
  }
}


VariableTranscriptionError
::VariableTranscriptionError(const std::string& what, 
			     const std::string& var): 
  std::logic_error(what + " (variable: " + var + " )"), 
  m_variable(var) 
{
  
}

