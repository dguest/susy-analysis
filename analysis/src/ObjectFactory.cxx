#include "ObjectFactory.hh"
#include "Jet.hh"
#include "JetBuffer.hh"
#include "BtagScaler.hh"
#include "BtagBuffer.hh"
#include "BtagConfig.hh"
#include "EventScalefactors.hh"
#include "EventRecoParameters.hh"
#include "SkimCounts.hh"
#include "common_functions.hh"

#include <string> 
#include <vector> 
#include <set> 
#include <stdexcept> 
#include <algorithm>
#include <boost/format.hpp>

#include "TVector2.h"
#include "TFile.h"
#include "TTree.h"

namespace { 
  void set_reco_parameters(TTree*, EventRecoParameters&); 
}

MetBuffer::MetBuffer(TTree* tree, const std::string& prefix) { 
  int errors = 0; 
  errors += std::abs(
    tree->SetBranchAddress((prefix + "met").c_str(), &met)); 
  errors += std::abs(
    tree->SetBranchAddress((prefix + "met_phi").c_str(), &met_phi)); 
  if (errors) throw std::logic_error("unknown met prefix: " + prefix); 
}


ObjectFactory::ObjectFactory(const std::string& root_file, int n_jets) : 
  m_tree(0), 
  m_file(0), 
  m_event_reco_parameters(0), 
  m_hfor_type(-1), 
  m_leading_cjet_pos(-1), 
  m_subleading_cjet_pos(-1), 
  m_mc_event_weight(0.0/0.0), 
  m_pileup_weight(0.0/0.0), 
  m_boson_pt_weight(0.0/0.0), 
  m_boson_pt(0.0/0.0), 
  m_ioflags(0), 
  m_evt_sf(0), 
  m_skim_counts(0)

{
  m_file = new TFile(root_file.c_str()); 
  if (!m_file) { 
    throw std::runtime_error(root_file + " could not be found"); 
  }
  m_skim_counts = new SkimCounts(*m_file); 
  m_tree = dynamic_cast<TTree*>(m_file->Get("evt_tree")); 
  if (!m_tree) { 
    throw std::runtime_error("evt_tree could not be found"); 
  }
  m_met.emplace(syst::NONE, new MetBuffer(m_tree, "")); 

  m_event_reco_parameters = new EventRecoParameters; 
  set_reco_parameters(m_tree, *m_event_reco_parameters); 
  set_branch(m_tree, "pass_bits", &m_bits); 
  // set_branch(m_tree, "min_jetmet_dphi", &m_dphi); 
  // set_branch(m_tree, "n_preselected_jets", &m_n_preselection_jets); 
  // set_branch(m_tree, "n_signal_jets", &m_n_signal); 

  try { 
    set_branch(m_tree,"hfor_type", &m_hfor_type); 
    set_branch(m_tree,"truth_leading_cjet_pos", &m_leading_cjet_pos); 
    set_branch(m_tree,"truth_subleading_cjet_pos", &m_subleading_cjet_pos); 
    set_branch(m_tree,"mc_event_weight", &m_mc_event_weight); 
    set_branch(m_tree,"pileup_weight", &m_pileup_weight); 
    m_evt_sf = new EventScalefactors(m_tree); 
  } catch (MissingBranch& err) { 
    m_ioflags |= ioflag::no_truth; 
  }

  // set_branch(m_tree,"htx", &m_htx); 
  for (int i = 0; i < n_jets; i++) { 
    std::string base_name = (boost::format("jet%i_") % i).str(); 
    m_jet_buffers.push_back(new JetBuffer); 
    m_jet_buffers.back()->set_buffer(m_tree, base_name); 
  }
  if (m_tree->GetBranch("boson_pt_weight")) { 
    m_tree->SetBranchAddress("boson_pt_weight", &m_boson_pt_weight); 
    m_tree->SetBranchAddress("boson_pt", &m_boson_pt); 
    m_ioflags |= ioflag::has_boson_pt_weight; 
  }
  set_branch(m_tree, "boson_child_pt", &m_boson_child_pt); 
  set_branch(m_tree, "boson_child_phi", &m_boson_child_phi); 
} 

ObjectFactory::~ObjectFactory() 
{
  for (auto itr: m_jet_buffers) {
    delete itr; 
    itr = 0; 
  }
  delete m_tree; 
  if (m_file) { 
    m_file->Close("R"); 
  }
  delete m_file; 
  delete m_event_reco_parameters; 
  delete m_evt_sf; 
  m_file = 0; 
  m_evt_sf = 0; 
  for (auto itr: m_met) { 
    delete itr.second; 
    itr.second = 0; 
  }
  delete m_skim_counts; 
}

int ObjectFactory::entries() const { 
  return m_tree->GetEntries(); 
}
void ObjectFactory::entry(int n) { 
  m_tree->GetEntry(n); 
}

std::vector<Jet> ObjectFactory::jets() const { 

  std::vector<Jet> jets_out; 
  int jrank = 0; 
  for (auto jet_buffer: m_jet_buffers) { 
    if (!jet_buffer->has_jet()) { 
      return jets_out; 
    }
    jets_out.push_back(Jet(jet_buffer,m_ioflags)); 
    Jet& jet = *jets_out.rbegin(); 
    jet.set_rank(jrank); 
    jet.set_event_met(met()); 
    jrank++; 
  }

  return jets_out; 
}

TVector2 ObjectFactory::met(syst::Systematic sy) const  { 
  if (!m_met.count(sy)) sy = syst::NONE; 
  const MetBuffer& buf = *m_met.at(sy); 
  TVector2 met; 
  met.SetMagPhi(buf.met, buf.met_phi); 
  return met; 
}
ull_t ObjectFactory::bits() const  { return m_bits; }
EventRecoParameters ObjectFactory::event_reco_parameters() const { 
  return *m_event_reco_parameters; 
}
int ObjectFactory::leading_cjet_pos() const {return m_leading_cjet_pos;}
int ObjectFactory::subleading_cjet_pos() const {return m_subleading_cjet_pos;}
double ObjectFactory::event_weight() const 
{
  if (m_ioflags & ioflag::no_truth) { 
    return 1.0; 
  }
  float base = m_mc_event_weight * m_pileup_weight; 
  return base; 
}
float ObjectFactory::marks_boson_pt_weight() const {
  if (!(m_ioflags & ioflag::has_boson_pt_weight) ) { 
    return 1.0; 
  }
  return m_boson_pt_weight; 
}
float ObjectFactory::boson_pt() const { 
  if (!(m_ioflags & ioflag::has_boson_pt_weight) ) { 
    return -1.0; 
  }
  return m_boson_pt; 
}
TVector2 ObjectFactory::boson_child() const { 
  if (m_boson_child_pt < 0.0) { 
    return TVector2(); 
  }
  TVector2 vec; 
  vec.SetMagPhi(m_boson_child_pt, m_boson_child_phi); 
  return vec; 
}

EventScalefactors* ObjectFactory::event_scalefactors() const { 
  return m_evt_sf; 
}

const SkimCounts* ObjectFactory::skim_counts() const { 
  return m_skim_counts; 
}

hfor::JetType ObjectFactory::hfor_type() const { 
  if (m_ioflags & ioflag::no_truth) { 
    throw std::runtime_error("no truth info found, can't get hfor type"); 
  }
  using namespace hfor; 
  switch (m_hfor_type) { 
  case -1: return NONE; 
  case 0: return BB; 
  case 1: return CC; 
  case 2: return C; 
  case 3: return LIGHT; 
  case 4: return KILL; 
  default: 
    throw std::runtime_error("given undefined hfor type"); 
  }
}

bool has_higher_pt(const Jet& v1, const Jet& v2) { 
  return v1.Pt() > v2.Pt(); 
}


namespace { 
  void set_reco_parameters(TTree* tree, EventRecoParameters& re) { 

#define SET_PARAMETER(tree, branch)		\
  set_branch(tree, #branch, &re.branch)

    SET_PARAMETER(tree, pass_met_trigger); 
    SET_PARAMETER(tree, pass_mu_trigger); 
    SET_PARAMETER(tree, pass_el_trigger); 
    SET_PARAMETER(tree, pass_event_quality); 

    SET_PARAMETER(tree, pass_osdf); 
    SET_PARAMETER(tree, pass_ossf); 

    SET_PARAMETER(tree, n_preselected_jets); 
    SET_PARAMETER(tree, n_signal_jets); 
    SET_PARAMETER(tree, n_veto_electrons); 
    SET_PARAMETER(tree, n_veto_muons); 
    SET_PARAMETER(tree, n_control_electrons); 
    SET_PARAMETER(tree, n_control_muons); 

    SET_PARAMETER(tree, min_jetmet_dphi); 
    SET_PARAMETER(tree, met_eff); 
    SET_PARAMETER(tree, mct); 
    SET_PARAMETER(tree, mcc); 
    SET_PARAMETER(tree, mt); 
    SET_PARAMETER(tree, mll); 
    SET_PARAMETER(tree, htx); 

    SET_PARAMETER(tree, max_lepton_pt);

#undef SET_PARAMETER

  }
}  

