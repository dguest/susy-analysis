#include "Jet.hh"
#include "ObjectFactory.hh"	// JetBuffer
#include "BtagScaler.hh"
#include "BtagConfig.hh"

#include <stdexcept> 
#include <cassert> 

#include "TLorentzVector.h"
#include "TVector2.h"
#include "Flavor.hh"

namespace { 
  const int RANK_ERROR_CODE = -10; 
}

Jet::Jet(const JetBuffer* basis, unsigned flags): 
  m_rank(RANK_ERROR_CODE), 
  m_jfc_pb(basis->jfc_b), 
  m_jfc_pc(basis->jfc_c), 
  m_jfc_pu(basis->jfc_u), 
  m_ioflags(flags), 
  m_buffer(basis), 
  m_el_jet(false)
{
  SetPtEtaPhiM(basis->pt, basis->eta, basis->phi, 0); 
  switch (basis->flavor_truth_label) { 
  case 0: m_truth_label = Flavor::LIGHT; break; 
  case 4: m_truth_label = Flavor::CHARM; break; 
  case 5: m_truth_label = Flavor::BOTTOM; break; 
  case 15: m_truth_label = Flavor::TAU; break; 
  default: m_truth_label = Flavor::NONE; break; 
  }
}
void Jet::set_event_met(const TVector2& met) { 
  m_met_dphi = met.DeltaPhi(Vect().XYvector()); 
}

void Jet::set_rank(int rank) { 
  if (rank == RANK_ERROR_CODE) { 
    throw std::logic_error("-1 rank is an error code"); 
  }
  m_rank = rank; 
}

int Jet::get_rank() const { 
  if (m_rank == RANK_ERROR_CODE) { 
    throw std::logic_error("ask for rank of unranked jet"); 
  }
  return m_rank; 
}


double Jet::met_dphi() const {
  return m_met_dphi; 
}
double Jet::flavor_weight(Flavor flav, btag::Tagger tagger) const { 
  req_flavor(); 
  assert(tagger == btag::JFC); 
  switch (flav) { 
  case Flavor::BOTTOM: return m_jfc_pb; 
  case Flavor::CHARM:  return m_jfc_pc; 
  case Flavor::LIGHT:  return m_jfc_pu; 
  default: throw std::invalid_argument(
    "non-implemented flavor in " __FILE__); 
  }
}
Flavor Jet::flavor_truth_label() const { 
  if (m_ioflags & ioflag::no_truth) { 
    throw std::runtime_error("no truth info, can't get flavor truth label"); 
  }
  return m_truth_label; 
}

bool Jet::has_flavor() const { 
  bool no_flavor = (m_ioflags & ioflag::no_flavor); 
  return !no_flavor; 
}

bool Jet::pass_tag(btag::OperatingPoint tag) const { 
  auto anti_u = log(m_jfc_pc / m_jfc_pu); 
  auto anti_b = log(m_jfc_pc / m_jfc_pb); 
  bool pass_u = anti_u > btag::JFC_MEDIUM_ANTI_U_CUT; 
  bool pass_b = anti_b > btag::JFC_MEDIUM_ANTI_B_CUT; 
  switch (tag) { 
  case btag::JFC_MEDIUM: return pass_u && pass_b; 
  case btag::JFC_LOOSE: return pass_b; 
  default: throw std::logic_error("unknown tag requested in " __FILE__); 
  }
}

double Jet::get_scalefactor(syst::Systematic systematic) const 
{ 
  assert(false); 
  return -1; 
}

bool Jet::is_electron_jet() const { 
  return m_buffer->is_electron_jet; 
}

void Jet::req_flavor() const 
{
  if (!has_flavor()) { 
    throw std::logic_error("flavor branches don't exist, bad bad bad.."); 
  }
  
}

