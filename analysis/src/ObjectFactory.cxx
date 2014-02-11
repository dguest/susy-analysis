#include "ObjectFactory.hh"
#include "Jet.hh"
#include "BtagScaler.hh"
#include "BtagBuffer.hh"
#include "BtagConfig.hh"
#include "EventScalefactors.hh"
#include "SkimCounts.hh"

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
  void set_branch(TTree* tree, const std::string& name, void* var); 
}

JetBuffer::JetBuffer(): 
  flavor_truth_label(-1), 
  is_electron_jet(false)
{ 
}
JetBuffer::~JetBuffer() { 
  for (auto itr = btag_scalers.begin(); itr != btag_scalers.end(); itr++) { 
    delete *itr; 
    *itr = 0; 
  }
  for (auto itr = btag_buffers.begin(); itr != btag_buffers.end(); itr++) { 
    delete itr->second; 
    itr->second = 0; 
  }
}

MetBuffer::MetBuffer(TTree* tree, const std::string& prefix) { 
  int errors = 0; 
  errors += std::abs(
    tree->SetBranchAddress((prefix + "met").c_str(), &met)); 
  errors += std::abs(
    tree->SetBranchAddress((prefix + "met_phi").c_str(), &met_phi)); 
  if (errors) throw std::logic_error("unknown met prefix: " + prefix); 
}


ObjectFactory::ObjectFactory(std::string root_file, int n_jets) : 
  m_file(0), 
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
  m_mu_met.emplace(syst::NONE, new MetBuffer(m_tree, "mu_")); 

  set_branch(m_tree, "pass_bits", &m_bits); 
  set_branch(m_tree, "min_jetmet_dphi", &m_dphi); 
  set_branch(m_tree, "n_preselected_jets", &m_n_preselection_jets); 
  set_branch(m_tree, "n_signal_jets", &m_n_signal); 

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

  set_branch(m_tree,"htx", &m_htx); 
  for (int i = 0; i < n_jets; i++) { 
    std::string base_name = (boost::format("jet%i_") % i).str(); 
    m_jet_buffers.push_back(new JetBuffer); 
    set_buffer(*m_jet_buffers.rbegin(), base_name); 
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
  delete m_evt_sf; 
  m_file = 0; 
  m_evt_sf = 0; 
  for (auto itr: m_met) { 
    delete itr.second; 
    itr.second = 0; 
  }
  for (auto itr: m_mu_met) { 
    delete itr.second; 
    itr.second = 0; 
  }
  delete m_skim_counts; 
}

void ObjectFactory
::set_btagging(const std::vector<btag::OperatingPoint>& tag_points){ 
  std::set<btag::OperatingPoint> tags(tag_points.begin(), tag_points.end()); 
  for (size_t jet_n = 0; jet_n < m_jet_buffers.size(); jet_n++) { 
    for (auto tag_iter = tags.begin(); tag_iter != tags.end(); tag_iter++) { 
      set_btag_n(jet_n, *tag_iter); 
    }
  }
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
    if (jet_buffer->pt <= 0) { 
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
double ObjectFactory::dphi()  const  { return m_dphi; }
int ObjectFactory::n_signal_jets()   const  { return m_n_signal; }
int ObjectFactory::n_preselected_jets()   const  { 
  return m_n_preselection_jets; 
}
int ObjectFactory::leading_cjet_pos() const {return m_leading_cjet_pos;}
int ObjectFactory::subleading_cjet_pos() const {return m_subleading_cjet_pos;}
double ObjectFactory::htx() const {return m_htx;}
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

void ObjectFactory::set_btag_n(size_t jet_n, btag::OperatingPoint tag) { 
  std::string branch_name = (boost::format("jet%i") % jet_n).str(); 
  if ( !(m_jet_buffers.size() > jet_n)) { 
    throw std::range_error("asked for out of range jet " + 
			   branch_name); 
  }
  if (m_ioflags & ioflag::no_truth) { 
    throw std::logic_error("tried to set btag buffer with no truth info"); 
  }

  JetBuffer* buffer = m_jet_buffers.at(jet_n); 
  set_btag(buffer, tag, branch_name); 
}

void ObjectFactory::set_btag(JetBuffer* buffer, btag::OperatingPoint tag, 
			     std::string branch_name) { 
  if (tag == btag::NOTAG) return; 
  std::string sf_br_name = branch_name + 
    btag::joiner(tag) + "scale_factor"; 
  if (!buffer->btag_buffers.count(sf_br_name)) { 
    buffer->btag_buffers[sf_br_name] = new BtagBuffer(m_tree, sf_br_name); 
  }
  const BtagBuffer* btag_buffer = buffer->btag_buffers[sf_br_name]; 

  size_t needed_size = tag + 1; 
  size_t scalers_size = buffer->btag_scalers.size(); 
  if (scalers_size < needed_size) { 
    buffer->btag_scalers.resize(needed_size); 
  }
  BtagScaler* scaler = buffer->btag_scalers.at(tag); 
  if (!scaler) { 
    scaler = new BtagScaler(btag_buffer, tag);
  }
  buffer->btag_scalers.at(tag) = scaler; 
}


void ObjectFactory::set_buffer(JetBuffer* b, std::string base_name) 
{
  using namespace std; 
  string pt = base_name + "pt"; 
  string eta = base_name + "eta"; 
  string phi = base_name + "phi"; 
  // not setting E because not saved yet...
  string pb_jfc = base_name + "jfc_b"; 
  string pc_jfc = base_name + "jfc_c"; 
  string pu_jfc = base_name + "jfc_u"; 
  string flavor_truth = base_name + "flavor_truth_label"; 
  
  set_branch(m_tree, pt.c_str(), &b->pt); 
  set_branch(m_tree, eta.c_str(), &b->eta); 
  set_branch(m_tree, phi.c_str(), &b->phi); 

  if (m_tree->GetBranch(pb_jfc.c_str())) { 
    set_branch(m_tree, pb_jfc.c_str(), &b->jfc_b); 
    set_branch(m_tree, pc_jfc.c_str(), &b->jfc_c); 
    set_branch(m_tree, pu_jfc.c_str(), &b->jfc_u); 
  }
  else {
    m_ioflags |= ioflag::no_flavor; 
  }

  if (m_tree->GetBranch(flavor_truth.c_str())) { 
    set_branch(m_tree, flavor_truth.c_str(), &b->flavor_truth_label); 
  }
  else { 
    m_ioflags |= ioflag::no_truth; 
  }

}

bool has_higher_pt(const Jet& v1, const Jet& v2) { 
  return v1.Pt() > v2.Pt(); 
}

namespace { 
  void set_branch(TTree* tree, const std::string& name, void* var) { 
    unsigned found = 0; 
    tree->SetBranchStatus(name.c_str(), true, &found); 
    tree->SetBranchAddress(name.c_str(), var);
    if (found != 1) {
      throw MissingBranch(
	"" + std::to_string(found) + " branches found setting "  + name);
    }
  }
}

MissingBranch::MissingBranch(const std::string& str): 
  std::runtime_error(str) 
{
}
