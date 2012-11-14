#include "JetFactory.hh"

#include <string> 
#include <vector> 
#include <stdexcept> 
#include <boost/format.hpp>

#include "TLorentzVector.h"
#include "TVector2.h"
#include "TFile.h"
#include "TTree.h"

JetFactory::JetFactory(std::string root_file, int n_jets) : 
  m_flags(0)
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
  errors += m_tree->SetBranchAddress("hfor_type", &m_hfor_type); 

  errors += m_tree->SetBranchAddress("leading_cjet_pos", 
				     &m_leading_cjet_pos); 
  errors += m_tree->SetBranchAddress("subleading_cjet_pos", 
				     &m_subleading_cjet_pos); 
  if (errors) { 
    throw std::runtime_error
      ((boost::format("%i branch setting errors") % errors).str()); 
  }
  for (int i = 1; i <= n_jets; i++) { 
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
    throw std::range_error("asked for undefined jet"); 
  }
  return Jet(the_buffer, m_flags); 
}
std::vector<Jet> JetFactory::jets() const { 
  std::vector<Jet> jets_out; 
  for (std::vector<JetBuffer*>::const_iterator 
	 itr = m_jet_buffers.begin(); itr != m_jet_buffers.end(); itr++) { 
    if ((*itr)->pt <= 0) { 
      return jets_out; 
    }
    jets_out.push_back(Jet(*itr,m_flags)); 
  }
  return jets_out; 
}

TVector2 JetFactory::met()   const  { 
  TVector2 met; 
  met.SetMagPhi(m_met, m_met_phi); 
  return met; 
}
unsigned JetFactory::bits() const  { return m_bits; }
double JetFactory::dphi()  const  { return m_dphi; }
int JetFactory::n_good()   const  { return m_n_good; }
int JetFactory::n_susy()   const  { return m_n_susy; }
int JetFactory::leading_cjet_pos() const {return m_leading_cjet_pos;}
int JetFactory::subleading_cjet_pos() const {return m_subleading_cjet_pos;}


hfor::JetType JetFactory::hfor_type() const { 
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


void JetFactory::set_buffer(std::string base_name) 
{
  using namespace std; 
  int errors = 0; 
  JetBuffer* b = new JetBuffer; 
  m_jet_buffers.push_back(b); 
  string pt = base_name + "pt"; 
  string eta = base_name + "eta"; 
  string phi = base_name + "phi"; 
  // not setting E because not saved yet...
  string pb = base_name + "cnn_b"; 
  string pc = base_name + "cnn_c"; 
  string pu = base_name + "cnn_u"; 
  string flavor_truth = base_name + "flavor_truth_label"; 

  errors += m_tree->SetBranchAddress(pt.c_str(), &b->pt); 
  errors += m_tree->SetBranchAddress(eta.c_str(), &b->eta); 
  errors += m_tree->SetBranchAddress(phi.c_str(), &b->phi); 

  if (m_tree->GetBranch(pb.c_str())) { 
    errors += m_tree->SetBranchAddress(pb.c_str(), &b->cnn_b); 
    errors += m_tree->SetBranchAddress(pc.c_str(), &b->cnn_c); 
    errors += m_tree->SetBranchAddress(pu.c_str(), &b->cnn_u); 
  }
  else {
    m_flags |= ioflag::no_flavor; 
  }

  if (m_tree->GetBranch(flavor_truth.c_str())) { 
    errors += m_tree->SetBranchAddress(flavor_truth.c_str(), 
				       &b->flavor_truth_label); 
  }
  else { 
    m_flags |= ioflag::no_truth; 
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
  m_flags(flags)
{
  SetPtEtaPhiM(basis->pt, basis->eta, basis->phi, 0); 
}
double Jet::pb() const {req_flavor(); return m_pb; } 
double Jet::pc() const {req_flavor(); return m_pc; } 
double Jet::pu() const {req_flavor(); return m_pu; } 
int    Jet::flavor_truth_label() const { 
  if (m_flags & ioflag::no_truth) { 
    throw std::runtime_error("no truth info found"); 
  }
  return m_truth_label; 
}

bool Jet::has_flavor() const { 
  return !(m_flags & ioflag::no_flavor); 
}

void Jet::req_flavor() const 
{
  if (!has_flavor()) { 
    throw std::runtime_error("flavor branches don't exist, bad bad bad.."); 
  }
  
}

