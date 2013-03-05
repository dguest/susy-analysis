#ifndef HIST_CONFIG_HH
#define HIST_CONFIG_HH

#include "BtagConfig.hh"
#include <map>

struct HistConfig { 
  btag::EventConfig btag_config; 
  std::map<std::string, float> floats; 
}; 

#endif 
