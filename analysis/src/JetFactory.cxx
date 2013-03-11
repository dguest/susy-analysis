#include "JetFactory.hh"
#include "BtagScaler.hh"
#include "distiller/EventBits.hh"

#include <string> 
#include <vector> 
#include <stdexcept> 
#include <boost/format.hpp>

#include "TLorentzVector.h"
#include "TVector2.h"
#include "TFile.h"
#include "TTree.h"

JetBuffer::JetBuffer() 
{ 
}
JetBuffer::~JetBuffer() { 
  for (auto itr = btag_scalers.begin(); itr != btag_scalers.end(); itr++) { 
    delete *itr; 
    *itr = 0; 
  }
}

JetFactory::JetFactory(std::string root_file, int n_jets) : 
  m_hfor_type(-1), 
  m_leading_cjet_pos(-1), 
  m_subleading_cjet_pos(-1), 
  m_ioflags(0)

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
  errors += m_tree->SetBranchAddress("met",&m_met); 
  errors += m_tree->SetBranchAddress("met_phi",&m_met_phi); 
  errors += m_tree->SetBranchAddress("pass_bits",&m_bits); 
  errors += m_tree->SetBranchAddress("min_jetmet_dphi", &m_dphi); 
  errors += m_tree->SetBranchAddress("n_good_jets", &m_n_good); 
  errors += m_tree->SetBranchAddress("n_susy_jets", &m_n_susy); 

  bool has_truth = (m_tree->GetBranch("hfor_type") && 
		    m_tree->GetBranch("leading_cjet_pos") && 
		    m_tree->GetBranch("subleading_cjet_pos") ); 
  if (has_truth) {
    errors += m_tree->SetBranchAddress("hfor_type", &m_hfor_type); 
    errors += m_tree->SetBranchAddress("leading_cjet_pos", 
				     &m_leading_cjet_pos); 
    errors += m_tree->SetBranchAddress("subleading_cjet_pos", 
				       &m_subleading_cjet_pos); 
    errors += m_tree->SetBranchAddress("mc_event_weight", 
				       &m_mc_event_weight); 
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
    set_buffer(base_name); 
  }
} 

JetFactory::~JetFactory() 
{
  for (std::vector<JetBuffer*>::iterator itr = m_jet_buffers.begin(); 
       itr != m_jet_buffers.end(); 
       itr++) { 
    delete *itr; 
    *itr = 0; 
  }
  delete m_file; 
}
void JetFactory::set_btagging(const std::vector<btag::JetTag>& tag_points) { 
  for (size_t jet_n = 0; jet_n < tag_points.size(); jet_n++) { 
    btag::JetTag tag = tag_points.at(jet_n); 
    if (tag == btag::NOTAG) { 
      continue; 
    }
    set_btag(jet_n, tag); 
  }
}

int JetFactory::entries() const { 
  return m_tree->GetEntries(); 
}
void JetFactory::entry(int n) { 
  m_tree->GetEntry(n); 
}

Jet JetFactory::jet(int jet_number) const { 
  // offset in the jet numbersing right now
  JetBuffer* the_buffer = m_jet_buffers.at(jet_number); 
  assert(the_buffer); 
  if (the_buffer->pt <= 0) { 
    throw std::out_of_range("asked for undefined jet"); 
  }
  return Jet(the_buffer, m_ioflags); 
}
std::vector<Jet> JetFactory::jets() const { 
  std::vector<Jet> jets_out; 
  for (std::vector<JetBuffer*>::const_iterator 
	 itr = m_jet_buffers.begin(); itr != m_jet_buffers.end(); itr++) { 
    if ((*itr)->pt <= 0) { 
      return jets_out; 
    }
    jets_out.push_back(Jet(*itr,m_ioflags)); 
    Jet& jet = *jets_out.rbegin(); 
    jet.set_event_met(met()); 
  }
  return jets_out; 
}

TVector2 JetFactory::met()   const  { 
  TVector2 met; 
  met.SetMagPhi(m_met, m_met_phi); 
  return met; 
}
ull_t JetFactory::bits() const  { return m_bits; }
double JetFactory::dphi()  const  { return m_dphi; }
int JetFactory::n_good()   const  { return m_n_good; }
int JetFactory::n_susy()   const  { return m_n_susy; }
int JetFactory::leading_cjet_pos() const {return m_leading_cjet_pos;}
int JetFactory::subleading_cjet_pos() const {return m_subleading_cjet_pos;}
double JetFactory::htx() const {return m_htx;}
double JetFactory::event_weight() const 
{
  if (m_ioflags & ioflag::no_truth) { 
    return 1.0; 
  }
  return m_mc_event_weight; 
}

hfor::JetType JetFactory::hfor_type() const { 
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

void JetFactory::set_btag(size_t jet_n, btag::JetTag tag) { 
  std::string full_branch_name = (boost::format("jet%i") % jet_n).str(); 
  if ( !(m_jet_buffers.size() > jet_n)) { 
    throw std::range_error("asked for out of range jet " + 
			   full_branch_name); 
  }
  if (m_ioflags & ioflag::no_truth) { 
    throw std::logic_error("tried to set btag buffer with no truth info"); 
  }
  JetBuffer* buffer = m_jet_buffers.at(jet_n); 
  size_t needed_size = tag + 1; 
  size_t scalers_size = buffer->btag_scalers.size(); 
  if (scalers_size < needed_size) { 
    buffer->btag_scalers.resize(needed_size); 
  }
  BtagScaler* scaler = buffer->btag_scalers.at(tag); 
  if (!scaler) { 
    scaler = new BtagScaler(m_tree, full_branch_name, tag);
  }
  buffer->btag_scalers.at(tag) = scaler; 
}


void JetFactory::set_buffer(std::string base_name) 
{
  using namespace std; 
  int errors = 0; 
  m_jet_buffers.push_back(new JetBuffer); 
  JetBuffer* b = *m_jet_buffers.rbegin(); 
  string pt = base_name + "pt"; 
  string eta = base_name + "eta"; 
  string phi = base_name + "phi"; 
  // not setting E because not saved yet...
  string pb = base_name + "cnn_b"; 
  string pc = base_name + "cnn_c"; 
  string pu = base_name + "cnn_u"; 
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


Jet::Jet(JetBuffer* basis, unsigned flags): 
  m_pb(basis->cnn_b), 
  m_pc(basis->cnn_c), 
  m_pu(basis->cnn_u), 
  m_truth_label(basis->flavor_truth_label), 
  m_met_dphi(0), 
  m_ioflags(flags), 
  m_buffer(basis)
{
  SetPtEtaPhiM(basis->pt, basis->eta, basis->phi, 0); 
}
void Jet::set_event_met(const TVector2& met) { 
  TLorentzVector met_4vec(met.X(), met.Y(), 0, 1); 
  m_met_dphi = met_4vec.DeltaPhi(*this); 
}
double Jet::met_dphi() const {return m_met_dphi; }
double Jet::pb() const {req_flavor(); return m_pb; } 
double Jet::pc() const {req_flavor(); return m_pc; } 
double Jet::pu() const {req_flavor(); return m_pu; } 
int    Jet::flavor_truth_label() const { 
  if (m_ioflags & ioflag::no_truth) { 
    throw std::runtime_error("no truth info, can't get flavor truth label"); 
  }
  return m_truth_label; 
}

bool Jet::has_flavor() const { 
  bool no_flavor = (m_ioflags & ioflag::no_flavor); 
  return !no_flavor; 
}
bool Jet::pass_tag(btag::JetTag tag) const { 
  const auto required = btag::required_from_tag(tag); 
  return (m_buffer->bits & required) == required; 
}

double Jet::get_scalefactor(btag::JetTag tag, syst::Systematic systematic) 
  const 
{ 
  if (tag == btag::NOTAG) { 
    return 1.0; 
  }
  auto& scalers = m_buffer->btag_scalers; 
  if (int(scalers.size()) <= tag ) { 
    throw std::logic_error("asked for an out of range scalefactor"); 
  }
  if ( !scalers.at(tag)) { 
    throw std::logic_error("asked for an unset jet scalefactor"); 
  }
  return scalers.at(tag)->get_scalefactor
    (m_buffer->bits, flavor_truth_label(),systematic); 
}

void Jet::req_flavor() const 
{
  if (!has_flavor()) { 
    throw std::logic_error("flavor branches don't exist, bad bad bad.."); 
  }
  
}
