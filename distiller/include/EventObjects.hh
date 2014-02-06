#ifndef EVENT_OBJECTS_HH
#define EVENT_OBJECTS_HH

// this is supposed to give us all the jets and leptons we need
#include<vector>

class SusyBuffer; 
class SUSYObjDef; 
class RunInfo;
class SelectedJet; 
class Muon; 
class Electron;  
class CutCounter; 
class EventJets; 
class EventMuons; 
class EventElectrons;

struct EventObjects { 
  EventObjects(const SusyBuffer&, SUSYObjDef& def, unsigned, const RunInfo&);
  EventObjects(EventObjects&) = delete; 
  EventObjects& operator=(EventObjects&) = delete; 
  ~EventObjects(); 
  void do_overlap_removal(CutCounter&); 
  EventJets* all_jets; 
  EventElectrons* all_electrons; 
  EventMuons* all_muons; 
  
  std::vector<SelectedJet*> preselected_jets; 
  std::vector<Electron*> preselected_electrons; 
  std::vector<Muon*> preselected_muons; 

  std::vector<SelectedJet*> after_overlap_jets; 
  std::vector<Electron*> after_overlap_electrons; 
  std::vector<Muon*> after_overlap_muons; 

  std::vector<SelectedJet*> veto_jets; 
  std::vector<Electron*> veto_electrons; 
  std::vector<Muon*> veto_muons; 
  
  std::vector<SelectedJet*> good_jets; 
  std::vector<SelectedJet*> signal_jets; 
  std::vector<Electron*> control_electrons; 
  std::vector<Muon*> control_muons; 
  
  std::vector<int> susy_muon_idx; 
}; 

#endif 
