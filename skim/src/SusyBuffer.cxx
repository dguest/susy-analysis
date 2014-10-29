#include "SusyBuffer.hh"
#include "misc_func.hh"		// setOrThrow
#include "BranchBuffer.hh"

#include <vector>
#include <string>
#include <utility>

#include "TChain.h"

namespace {
  // warning: works by setting the status of all trigger branches to 0
  bool allTriggersMising(TChain&);
}

SusyBuffer::SusyBuffer(TChain& chain,
		       const std::vector<std::string>& variables):
  m_branch_buffer(new BranchBuffer),
  m_has_mc(true),
  m_has_xe80_tclcw_loose(true)
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

  m_branch_buffer->addInputs(chain, variables);
}

SusyBuffer::~SusyBuffer() {
  delete m_branch_buffer;
  delete mc_pt;
  delete mc_eta;
  delete mc_phi;
  delete mc_m;
  delete mc_status;
  delete mc_pdgId;
}

std::set<std::string> SusyBuffer::getExposedInputs() const {
  return m_branch_buffer->getExposedInputs();
}
std::set<std::string> SusyBuffer::getMissingBranches() const {
  return m_branch_buffer->getMissingBranches();
}


void SusyBuffer::setPassThrough(TTree& tr) const {
  m_branch_buffer->setPassThrough(tr);
}


bool SusyBuffer::hasMc() const {
  return m_has_mc;
}

void SusyBuffer::dump() const {
  m_branch_buffer->dump();
}

// ========== private stuff ==========

// generic branch setting functions


// specific branch setting functions
void SusyBuffer::setMcBranches(TChain& chain) {
  try {
    m_branch_buffer->set(chain, "mcevt_weight", &mcevt_weight, Save::NEVER);
  } catch (MissingBranchError& e) {
    m_has_mc = false;
  }
  // these branches shouldn't normally never be saved (they are huge)
  const auto should_save_mc = Save::NEVER;
  if (m_has_mc) {
    m_branch_buffer->set(chain, "mc_n",      &mc_n     , Save::IF_LISTED);
    m_branch_buffer->set(chain, "mc_pt",     &mc_pt    , should_save_mc);
    m_branch_buffer->set(chain, "mc_eta",    &mc_eta   , should_save_mc);
    m_branch_buffer->set(chain, "mc_phi",    &mc_phi   , should_save_mc);
    m_branch_buffer->set(chain, "mc_m",      &mc_m     , should_save_mc);
    m_branch_buffer->set(chain, "mc_status", &mc_status, should_save_mc);
    m_branch_buffer->set(chain, "mc_pdgId",  &mc_pdgId , should_save_mc);
  } else {
    mc_pt = 0;
    mc_eta = 0;
    mc_phi = 0;
    mc_m = 0;
    mc_status = 0;
    mc_pdgId = 0;
  }
}

#define SET_TRIG(name, save)			\
  m_branch_buffer->set(chain, #name, &tr.name, save)

void SusyBuffer::setTriggerBranches(TChain& chain) {
  Triggers& tr = triggers;
  try {
    SET_TRIG(EF_xe80_tclcw_loose, Save::ALWAYS);
  } catch (MissingBranchError& err) {
    m_has_xe80_tclcw_loose = false;
    tr.EF_xe80_tclcw_loose = false;
  }
  SET_TRIG(EF_xe80T_tclcw_loose, Save::ALWAYS);
  SET_TRIG(EF_xe80_tclcw_tight, Save::ALWAYS);
  SET_TRIG(EF_xe80T_tclcw, Save::ALWAYS);
  SET_TRIG(EF_mu18_tight_mu8_EFFS, Save::ALWAYS);
  SET_TRIG(EF_mu24i_tight, Save::ALWAYS);
  SET_TRIG(EF_mu36_tight, Save::ALWAYS);
  SET_TRIG(EF_e24vhi_medium1, Save::ALWAYS);
  SET_TRIG(EF_2e12Tvh_loose1, Save::ALWAYS);
  SET_TRIG(EF_e60_medium1, Save::ALWAYS);
}

void SusyBuffer::setMetBranches(TChain& chain) {
  const std::string mbs = "MET_Egamma10NoTau_";
  m_branch_buffer->set(chain, mbs + "RefFinal_etx", &met.etx);
  m_branch_buffer->set(chain, mbs + "RefFinal_ety", &met.ety);
  m_branch_buffer->set(chain, mbs + "Muon_Total_Staco_etx", &met.muon_etx);
  m_branch_buffer->set(chain, mbs + "Muon_Total_Staco_ety", &met.muon_ety);
}

namespace {
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

