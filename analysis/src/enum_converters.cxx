#include "enum_converters.hh"
#include <stdexcept> 

// annoying conversion needed to keep the JetTagRescaler modular

btag::JetTag btag_from_jettag(jettag::TaggingPoint p) { 
  using namespace btag; 
  switch (p){ 
  case jettag::LOOSE: return LOOSE; 
  case jettag::MEDIUM: return MEDIUM; 
  case jettag::TIGHT: return TIGHT; 
  case jettag::ANTILOOSE: return ANTILOOSE; 
  default: throw std::logic_error("unknown tag op in "__FILE__); 
  }
}

jettag::TaggingPoint jettag_from_btag(btag::JetTag p){ 
  using namespace jettag; 
  switch (p){ 
  case btag::LOOSE: return LOOSE; 
  case btag::MEDIUM: return MEDIUM; 
  case btag::TIGHT: return TIGHT; 
  case btag::ANTILOOSE: return ANTILOOSE; 
  default: throw std::logic_error("unknown tag op in "__FILE__); 
  }
}
