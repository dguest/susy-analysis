#include "BtagScaler.hh"
#include <stdexcept> 
#include "BtagBuffer.hh"
#include <boost/format.hpp>
#include "typedefs.hh"
#include "BtagConfig.hh"
#include "distiller/JetBits.hh"

BtagScaler::BtagScaler(const BtagBuffer* buffer, btag::OperatingPoint tag): 
  m_buffer(buffer)
{
  m_required = btag::required_from_tag(tag); 
  m_veto = btag::veto_from_tag(tag); 
  m_inverted = btag::is_inverted(tag); 
}

double BtagScaler::get_scalefactor(unsigned jet_mask, Flavor flavor, 
				   syst::Systematic syst) const 
{
  using namespace syst; 
  bool pass = (jet_mask & m_required) && !(jet_mask | m_veto); 
  double base = m_buffer->scale_factor;
  double err = m_buffer->scale_factor_err; 

  // we could handle the tag inversion somewhere else, but for now 
  // it seems easier to invert here, at risk of (worst case) doubling 
  // the number of taggers we have to define. 
  if (m_inverted) err *= -1.0; 	

  double err_up = pass ? err : -err;
  double err_down = -err_up; 
  switch (syst) { 
  case NONE: 
    return base; 
  case BUP: 
    return flavor == Flavor::BOTTOM ? base + err_up: base; 
  case BDOWN: 
    return flavor == Flavor::BOTTOM ? base + err_down: base; 
  case CUP: 
    return flavor == Flavor::CHARM ? base + err_up: base; 
  case CDOWN: 
    return flavor == Flavor::CHARM ? base + err_down: base; 
  case UUP: 
    return flavor == Flavor::LIGHT ? base + err_up: base; 
  case UDOWN: 
    return flavor == Flavor::LIGHT ? base + err_down: base; 
  case TUP: 
    return flavor == Flavor::TAU ? base + err_up: base; 
  case TDOWN: 
    return flavor == Flavor::TAU ? base + err_down: base; 
  default: 
    throw std::logic_error("got unknown systematic in " __FILE__); 
  }
}

