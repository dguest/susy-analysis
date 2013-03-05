#ifndef HIST_CONFIG_HH
#define HIST_CONFIG_HH

#include "BtagConfig.hh"
#include "systematic_defs.hh"
#include <map>
#include <string> 

struct HistConfig { 
  btag::EventConfig btag_config; 
  syst::Systematic systematic; 
  // the floats map is a leftover hack, should remove
  std::map<std::string, float> floats; 
  HistConfig(); 
}; 

#endif 
