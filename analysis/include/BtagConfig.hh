#ifndef BTAG_CONFIG_HH
#define BTAG_CONFIG_HH

#include <string> 
#include "btag_enums.hh"

namespace btag { 
  unsigned required_from_tag(OperatingPoint); 
  unsigned veto_from_tag(OperatingPoint); 
  std::string joiner(OperatingPoint tag); 
  bool is_inverted(OperatingPoint); 
  
  // log(pc / pu) selection minimum
  const double JFC_MEDIUM_ANTI_U_CUT =  0.95; 

  // log(pc / pb) selection minimum
  const double JFC_MEDIUM_ANTI_B_CUT = -0.90;
} 
#endif 
