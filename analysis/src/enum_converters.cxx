#include "enum_converters.hh"
#include <stdexcept> 

// annoying conversion needed to keep the JetTagRescaler modular

btag::OperatingPoint btag_from_jettag(jettag::TaggingPoint p) { 
  using namespace btag; 
  switch (p){ 
  case jettag::LOOSE: return LOOSE; 
  case jettag::MEDIUM: return MEDIUM; 
  case jettag::TIGHT: return TIGHT; 
  case jettag::ANTILOOSE: return ANTILOOSE; 
  case jettag::NOTAG: return NOTAG; 
  default: throw std::logic_error("unknown tag op in "__FILE__); 
  }
}

jettag::TaggingPoint jettag_from_btag(btag::OperatingPoint p){ 
  using namespace jettag; 
  switch (p){ 
  case btag::LOOSE: return LOOSE; 
  case btag::MEDIUM: return MEDIUM; 
  case btag::TIGHT: return TIGHT; 
  case btag::ANTILOOSE: return ANTILOOSE; 
  case btag::NOTAG: return NOTAG; 
  default: throw std::logic_error("unknown tag op in "__FILE__); 
  }
}
