#include "Flavor.hh"
#include <cassert>

bool operator<(Flavor l, Flavor r) {
  return static_cast<int>(l) < static_cast<int>(r);
}

const char* as_char(Flavor flav) {
  switch (flav) {
  case Flavor::CHARM: return "charm";
  case Flavor::BOTTOM: return "bottom";
  case Flavor::LIGHT: return "light";
  case Flavor::TAU: return "tau";
  case Flavor::NONE: return "none";
  default: assert(false);
  }
}
