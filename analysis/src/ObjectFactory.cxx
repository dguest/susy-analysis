#include "ObjectFactory.hh"
#include "Jet.hh"
#include "BtagScaler.hh"
#include "BtagBuffer.hh"
#include "BtagConfig.hh"
#include "EventScalefactors.hh"
// #include "met_systematics.hh"

#include <string> 
#include <vector> 
#include <set> 
#include <stdexcept> 
#include <algorithm>
#include <boost/format.hpp>

#include "TVector2.h"
#include "TFile.h"
#include "TTree.h"

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
  m_electron_jet_buffer(0), 
  m_hfor_type(-1), 
  m_leading_cjet_pos(-1), 
  m_subleading_cjet_pos(-1), 
  m_mc_event_weight(0.0/0.0), 
  m_pileup_weight(0.0/0.0), 
  m_boson_pt_weight(0.0/0.0), 
  m_ioflags(0), 
  m_evt_sf(0)

{
  m_file = new TFile(root_file.c_str()); 
  if (!m_file) { 
    throw std::runtime_error(root_file + " could not be found"); 
  }
  m_tree = dynamic_cast<TTree*>(m_file->Get("evt_tree")); 
  if (!m_tree) { 
    throw std::runtime_error("evt_tree could not be found"); 
  }
  int errors = 0; 
  m_met.emplace(syst::NONE, new MetBuffer(m_tree, "")); 
  m_mu_met.emplace(syst::NONE, new MetBuffer(m_tree, "mu_")); 

  errors += m_tree->SetBranchAddress("pass_bits",&m_bits); 
  errors += m_tree->SetBranchAddress("min_jetmet_dphi", &m_dphi); 
  errors += m_tree->SetBranchAddress("n_good_jets", &m_n_good); 
  errors += m_tree->SetBranchAddress("n_susy_jets", &m_n_susy); 

  bool has_truth = (m_tree->GetBranch("hfor_type") && 
		    m_tree->GetBranch("leading_cjet_pos") && 
		    m_tree->GetBranch("subleading_cjet_pos") ); 
  if (has_truth) {
    m_met.emplace(syst::METUP, new MetBuffer(m_tree, "stup_")); 
    m_met.emplace(syst::METDOWN, new MetBuffer(m_tree, "stdown_")); 
    m_met.emplace(syst::METRES, new MetBuffer(m_tree, "stres_")); 
    m_mu_met.emplace(syst::METUP, new MetBuffer(m_tree, "stup_mu_")); 
    m_mu_met.emplace(syst::METDOWN, new MetBuffer(m_tree, "stdown_mu_")); 
    m_mu_met.emplace(syst::METRES, new MetBuffer(m_tree, "stres_mu_")); 
    errors += m_tree->SetBranchAddress("hfor_type", &m_hfor_type); 
    errors += m_tree->SetBranchAddress("leading_cjet_pos", 
				     &m_leading_cjet_pos); 
    errors += m_tree->SetBranchAddress("subleading_cjet_pos", 
				       &m_subleading_cjet_pos); 
    errors += m_tree->SetBranchAddress("mc_event_weight", 
				       &m_mc_event_weight); 
    errors += m_tree->SetBranchAddress("pileup_weight", &m_pileup_weight); 
    m_evt_sf = new EventScalefactors(m_tree); 
  }
  else { 
    m_ioflags |= ioflag::no_truth; 
  }

  errors += m_tree->SetBranchAddress("htx", &m_htx); 
  if (errors) { 
    throw std::runtime_error
      ((boost::format("%i branch setting errors") % errors).str()); 
  }
  for (int i = 0; i < n_jets; i++) { 
    std::string base_name = (boost::format("jet%i_") % i).str(); 
    m_jet_buffers.push_back(new JetBuffer); 
    set_buffer(*m_jet_buffers.rbegin(), base_name); 
  }
  if (m_tree->GetBranch("boson_pt_weight")) { 
    m_tree->SetBranchAddress("boson_pt_weight", &m_boson_pt_weight); 
    m_ioflags |= ioflag::has_boson_pt_weight; 
  }
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
  delete m_electron_jet_buffer; 
}

void ObjectFactory::use_electron_jet(bool use) { 
  if (use && !m_electron_jet_buffer) { 
    m_electron_jet_buffer = new JetBuffer; 
    m_electron_jet_buffer->is_electron_jet = true; 
    set_buffer(m_electron_jet_buffer, "electron_jet_"); 
  }
  else if (!use) { 
    delete m_electron_jet_buffer; 
    m_electron_jet_buffer = 0; 
  }
}

void ObjectFactory
::set_btagging(const std::vector<btag::OperatingPoint>& tag_points){ 
  std::set<btag::OperatingPoint> tags(tag_points.begin(), tag_points.end()); 
  for (size_t jet_n = 0; jet_n < m_jet_buffers.size(); jet_n++) { 
    for (auto tag_iter = tags.begin(); tag_iter != tags.end(); tag_iter++) { 
      set_btag_n(jet_n, *tag_iter); 
    }
  }
  if (m_electron_jet_buffer) { 
    for (auto tag_iter = tags.begin(); tag_iter != tags.end(); tag_iter++) { 
      set_btag(m_electron_jet_buffer, *tag_iter, "electron_jet"); 
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
  bool is_data = (m_ioflags & ioflag::no_truth); 
  std::vector<syst::Systematic> met_systs = {syst::NONE}; 
  if (!is_data) { 
    met_systs.insert(
      met_systs.end(),{syst::METUP, syst::METDOWN, syst::METRES});
  }

  std::vector<Jet> jets_out; 
  for (auto itr = m_jet_buffers.begin(); itr != m_jet_buffers.end(); itr++) { 
    if ((*itr)->pt <= 0) { 
      return jets_out; 
    }
    jets_out.push_back(Jet(*itr,m_ioflags)); 
    Jet& jet = *jets_out.rbegin(); 
    for (auto sys: met_systs) { 
      jet.set_event_met(met(sys), sys); 
      jet.set_mu_met(mu_met(sys), sys); 
    }
  }
  if (m_electron_jet_buffer) { 
    if (m_electron_jet_buffer->pt > 0) { 
      jets_out.push_back(Jet(m_electron_jet_buffer, m_ioflags)); 
      for (auto sys: met_systs) { 
	jets_out.rbegin()->set_event_met(met(sys), sys); 
	jets_out.rbegin()->set_mu_met(met(sys), sys); 
      }
      std::sort(jets_out.begin(), jets_out.end(), has_higher_pt); 
    }
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
TVector2 ObjectFactory::mu_met(syst::Systematic sy) const  { 
  if (!m_mu_met.count(sy)) sy = syst::NONE; 
  const MetBuffer& buf = *m_mu_met.at(sy); 
  TVector2 met; 
  met.SetMagPhi(buf.met, buf.met_phi); 
  return met; 
}
ull_t ObjectFactory::bits() const  { return m_bits; }
double ObjectFactory::dphi()  const  { return m_dphi; }
int ObjectFactory::n_good()   const  { return m_n_good; }
int ObjectFactory::n_susy()   const  { return m_n_susy; }
int ObjectFactory::leading_cjet_pos() const {return m_leading_cjet_pos;}
int ObjectFactory::subleading_cjet_pos() const {return m_subleading_cjet_pos;}
double ObjectFactory::htx() const {return m_htx;}
double ObjectFactory::event_weight() const 
{
  if (m_ioflags & ioflag::no_truth) { 
    return 1.0; 
  }
  float base = m_mc_event_weight * m_pileup_weight; 
  if (m_ioflags & ioflag::has_boson_pt_weight) base *= m_boson_pt_weight; 
  return base; 
}

EventScalefactors* ObjectFactory::event_scalefactors() const { 
  return m_evt_sf; 
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
  int errors = 0; 
  string pt = base_name + "pt"; 
  string eta = base_name + "eta"; 
  string phi = base_name + "phi"; 
  // not setting E because not saved yet...
  string pb = base_name + "cnn_b"; 
  string pc = base_name + "cnn_c"; 
  string pu = base_name + "cnn_u"; 
  string pb_jfc = base_name + "jfc_b"; 
  string pc_jfc = base_name + "jfc_c"; 
  string pu_jfc = base_name + "jfc_u"; 
  string flavor_truth = base_name + "flavor_truth_label"; 
  string bits = base_name + "bits"; 
  
  errors += abs(m_tree->SetBranchAddress(pt.c_str(), &b->pt)); 
  errors += abs(m_tree->SetBranchAddress(eta.c_str(), &b->eta)); 
  errors += abs(m_tree->SetBranchAddress(phi.c_str(), &b->phi)); 
  errors += abs(m_tree->SetBranchAddress(bits.c_str(), &b->bits)); 

  if (m_tree->GetBranch(pb.c_str())) { 
    errors += abs(m_tree->SetBranchAddress(pb.c_str(), &b->cnn_b)); 
    errors += abs(m_tree->SetBranchAddress(pc.c_str(), &b->cnn_c)); 
    errors += abs(m_tree->SetBranchAddress(pu.c_str(), &b->cnn_u)); 
    errors += abs(m_tree->SetBranchAddress(pb_jfc.c_str(), &b->jfc_b)); 
    errors += abs(m_tree->SetBranchAddress(pc_jfc.c_str(), &b->jfc_c)); 
    errors += abs(m_tree->SetBranchAddress(pu_jfc.c_str(), &b->jfc_u)); 
  }
  else {
    m_ioflags |= ioflag::no_flavor; 
  }

  if (m_tree->GetBranch(flavor_truth.c_str())) { 
    errors += abs(m_tree->SetBranchAddress(flavor_truth.c_str(), 
					   &b->flavor_truth_label)); 
  }
  else { 
    m_ioflags |= ioflag::no_truth; 
  }

  if (errors) { 
    throw std::runtime_error
      ((boost::format("%i branch setting errors formatting %s") % 
	errors % base_name).str()); 
  }

}

bool has_higher_pt(const Jet& v1, const Jet& v2) { 
  return v1.Pt() > v2.Pt(); 
}

