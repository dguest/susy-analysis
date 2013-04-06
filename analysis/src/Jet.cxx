#include "Jet.hh"
#include "JetFactory.hh"	// JetBuffer
#include "BtagScaler.hh"

#include <stdexcept> 

#include "TLorentzVector.h"
#include "TVector2.h"

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
  const auto veto = btag::veto_from_tag(tag); 
  const auto jet_bits = m_buffer->bits; 
  bool has_required = ((jet_bits & required) == required); 
  bool has_veto = (jet_bits & veto); 
  return has_required & (!has_veto); 
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
