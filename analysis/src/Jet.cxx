#include "Jet.hh"
#include "ObjectFactory.hh"	// JetBuffer
#include "BtagBuffer.hh"
#include "BtagConfig.hh"

#include <stdexcept> 
#include <cassert> 

#include "TLorentzVector.h"
#include "TVector2.h"
#include "Flavor.hh"
#include "JetBuffer.hh"

namespace { 
  const int RANK_ERROR_CODE = -10; 
  double build_scalefactor(const BtagBuffer& buffer, 
			   bool pass, Flavor flavor, syst::Systematic); 
}

Jet::Jet(const JetBuffer* basis, bool has_truth): 
  m_rank(RANK_ERROR_CODE), 
  m_jfc_pb(basis->m_jfc_b), 
  m_jfc_pc(basis->m_jfc_c), 
  m_jfc_pu(basis->m_jfc_u), 
  m_ioflags(!has_truth & ioflag::no_truth), 
  m_buffer(basis), 
  m_el_jet(false)
{
  SetPtEtaPhiM(basis->m_pt, basis->m_eta, basis->m_phi, 0); 
  switch (basis->m_flavor_truth_label) { 
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
  if (m_ioflags & ioflag::no_truth) { 
    throw std::logic_error("no truth info given in " __FILE__); 
  }
  // ACHTUNG: this result will be wrong if we ask for non-JFC_MEDIUM!
  bool pass = pass_tag(btag::JFC_MEDIUM); 
  return build_scalefactor(*m_buffer->m_medium_sf, pass, 
			   m_truth_label, systematic); 

}

bool Jet::is_electron_jet() const { 
  assert(false); 
  return false; 
}

void Jet::req_flavor() const 
{
  if (!has_flavor()) { 
    throw std::logic_error("flavor branches don't exist, bad bad bad.."); 
  }
  
}

namespace { 
  TagSF get_var(Flavor, syst::Systematic); 
  double build_scalefactor(const BtagBuffer& buffer, 
			   bool pass, Flavor flavor, 
			   syst::Systematic syst) {
    using namespace syst; 

    // we could handle the tag inversion somewhere else, but for now 
    // it seems easier to invert here, at risk of (worst case) doubling 
    // the number of taggers we have to define. 
    // if (m_inverted) err *= -1.0; 	

    TagSF var = get_var(flavor, syst); 
    // invert non-passing tags
    if (!pass && var != TagSF::NOMINAL) { 
      if (var == TagSF::UP) var = TagSF::DOWN; 
      else var = TagSF::UP; 
    }

    return buffer.sf(var); 

  }
  TagSF get_var(Flavor flavor, syst::Systematic syst) { 
    using namespace syst; 
    switch (syst) { 
    case BUP: 
      return flavor == Flavor::BOTTOM ? TagSF::UP: TagSF::NOMINAL; 
    case BDOWN: 
      return flavor == Flavor::BOTTOM ? TagSF::DOWN: TagSF::NOMINAL; 
    case CUP: 
      return flavor == Flavor::CHARM ? TagSF::UP: TagSF::NOMINAL; 
    case CDOWN: 
      return flavor == Flavor::CHARM ? TagSF::DOWN: TagSF::NOMINAL; 
    case UUP: 
      return flavor == Flavor::LIGHT ? TagSF::UP: TagSF::NOMINAL; 
    case UDOWN: 
      return flavor == Flavor::LIGHT ? TagSF::DOWN: TagSF::NOMINAL; 
    case TUP: 
      return flavor == Flavor::TAU ? TagSF::UP: TagSF::NOMINAL; 
    case TDOWN: 
      return flavor == Flavor::TAU ? TagSF::DOWN: TagSF::NOMINAL; 
    default: 
      return TagSF::NOMINAL; 
    }

  }

}
