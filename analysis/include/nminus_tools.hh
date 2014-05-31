#ifndef SELECTION_FACTORY_HH
#define SELECTION_FACTORY_HH

class ISelection;
class RegionConfig;

#include <map>

namespace nminus {
  ISelection* selection_factory(const RegionConfig& config);
}

#endif
