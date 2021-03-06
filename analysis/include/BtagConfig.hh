#ifndef BTAG_CONFIG_HH
#define BTAG_CONFIG_HH

#include <string>
#include "btag_enums.hh"

namespace btag {
  unsigned required_from_tag(OperatingPoint);
  unsigned veto_from_tag(OperatingPoint);
  std::string joiner(OperatingPoint tag);
  bool is_inverted(OperatingPoint);

}
#endif
