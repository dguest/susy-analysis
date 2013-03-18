#ifndef OBJECT_SELECTION_HH
#define OBJECT_SELECTION_HH

#include <vector> 
class Electron; 
class Muon; 

namespace object { 
  typedef std::vector<Electron*> Electrons; 
  typedef std::vector<Muon*> Muons; 
  Electrons veto_electrons(const Electrons&); 
  Electrons control_electrons(const Electrons&); 
  Muons veto_muons(const Muons&); 
  Muons control_muons(const Muons&); 
}

#endif 
