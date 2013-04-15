#ifndef OBJECT_SELECTION_HH
#define OBJECT_SELECTION_HH

#include <vector> 
class Electron; 
class Muon; 
class SelectedJet; 

namespace object { 
  typedef std::vector<Electron*> Electrons; 
  typedef std::vector<Muon*> Muons; 
  typedef std::vector<SelectedJet*> Jets; 
  Electrons veto_electrons(const Electrons&); 
  Electrons control_electrons(const Electrons&); 
  Muons veto_muons(const Muons&); 
  Muons control_muons(const Muons&); 
  Jets preselection_jets(const Jets&); 
  Jets veto_jets(const Jets&); 
  Jets signal_jets(const Jets&); 
}

#endif 
