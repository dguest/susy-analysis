#include "Jet.hh"
#include "ObjectFactory.hh"	// JetBuffer
#include "BtagScaler.hh"
#include "BtagConfig.hh"

#include <stdexcept> 

#include "TLorentzVector.h"
#include "TVector2.h"
#include "Flavor.hh"

namespace { 
  const int RANK_ERROR_CODE = -10; 
}

Jet::Jet(const JetBuffer* basis, unsigned flags): 
  m_rank(RANK_ERROR_CODE), 
  m_pb(basis->cnn_b), 
  m_pc(basis->cnn_c), 
  m_pu(basis->cnn_u), 
  m_jfc_pb(basis->jfc_b), 
  m_jfc_pc(basis->jfc_c), 
  m_jfc_pu(basis->jfc_u), 
  m_ioflags(flags), 
  m_buffer(basis), 
  m_el_jet(false), 
  m_tag(btag::NOTAG)
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
void Jet::set_event_met(const TVector2& met, syst::Systematic sy) { 
  TLorentzVector met_4vec(met.X(), met.Y(), 0, 1); 
  m_met_dphi[sy] = met_4vec.DeltaPhi(*this); 
}
void Jet::set_mu_met(const TVector2& met, syst::Systematic sy) { 
  TLorentzVector met_4vec(met.X(), met.Y(), 0, 1); 
  m_mu_met_dphi[sy] = met_4vec.DeltaPhi(*this); 
}

void Jet::set_tag(btag::OperatingPoint op) { 
  m_tag = op; 
}; 

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

btag::OperatingPoint Jet::get_tag() const {
  return m_tag; 
}; 


double Jet::met_dphi(syst::Systematic sy) const {
  return m_met_dphi.at(sy); 
}
double Jet::mu_met_dphi(syst::Systematic sy) const {
  return m_mu_met_dphi.at(sy); 
}
double Jet::flavor_weight(Flavor flav, btag::Tagger tagger) const { 
  req_flavor(); 
  if (tagger == btag::CNN) { 
    switch (flav) { 
    case Flavor::BOTTOM: return m_pb; 
    case Flavor::CHARM:  return m_pc; 
    case Flavor::LIGHT:  return m_pu; 
    default: { 
      throw std::invalid_argument("non-implemented flavor in " __FILE__); 
    }
    }
  }
  else if (tagger == btag::JFC) { 
    switch (flav) { 
    case Flavor::BOTTOM: return m_jfc_pb; 
    case Flavor::CHARM:  return m_jfc_pc; 
    case Flavor::LIGHT:  return m_jfc_pu; 
    default: { 
      throw std::invalid_argument("non-implemented flavor in " __FILE__); 
    }
    }
  }
  throw std::invalid_argument("non-implemented tagger in " __FILE__); 
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
  const auto required = btag::required_from_tag(tag); 
  const auto veto = btag::veto_from_tag(tag); 
  const auto jet_bits = m_buffer->bits; 
  bool has_required = ((jet_bits & required) == required); 
  bool has_veto = (jet_bits & veto); 
  return has_required & (!has_veto); 
}

double Jet::get_scalefactor(btag::OperatingPoint tag, syst::Systematic systematic) 
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

bool Jet::is_electron_jet() const { 
  return m_buffer->is_electron_jet; 
}

void Jet::req_flavor() const 
{
  if (!has_flavor()) { 
    throw std::logic_error("flavor branches don't exist, bad bad bad.."); 
  }
  
}

