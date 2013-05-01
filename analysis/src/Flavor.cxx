#include "Flavor.hh"

bool operator<(Flavor l, Flavor r) { 
  return static_cast<int>(l) < static_cast<int>(r); 
}
