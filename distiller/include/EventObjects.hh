#ifndef EVENT_OBJECTS_HH
#define EVENT_OBJECTS_HH

// this is supposed to give us all the jets and leptons we need

#include "PreselectionInfo.hh"
#include "enums_replacement.hh"
#include "systematic_defs.hh"

#include <vector>

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
class EventPreselector;
class TriggerSF;
class BtagCalibration;

struct EventObjects {
  EventObjects(const SusyBuffer&, SUSYObjDef& def, unsigned, const RunInfo&,
    const EventPreselector&);
  EventObjects(EventObjects&) = delete;
  EventObjects& operator=(EventObjects&) = delete;
  ~EventObjects();
  void do_overlap_removal(CutCounter&);
  void make_electron_jet_collection(const BtagCalibration* cal);
  void compute_trigger_sf(SUSYObjDef& def);
  TriggerSF* get_trigger_sf() const;
  const std::vector<SelectedJet*>& signal_jets(JetRep) const;
  const std::vector<SelectedJet*>& leading_jets(JetRep) const;

  std::vector<SelectedJet*> preselected_jets;
  std::vector<Electron*> preselected_electrons;
  std::vector<Muon*> preselected_muons;

  std::vector<SelectedJet*> after_overlap_jets;
  std::vector<Electron*> after_overlap_electrons;
  std::vector<Muon*> after_overlap_muons;

  std::vector<SelectedJet*> veto_jets;

  std::vector<SelectedJet*> good_jets;
  std::vector<Electron*> control_electrons;
  std::vector<Muon*> control_muons;

  std::vector<int> susy_muon_idx;

  PreselectionInfo prec;

private:
  std::vector<SelectedJet*> m_signal_jets;
  std::vector<SelectedJet*> m_signal_jets_eljet;
  std::vector<SelectedJet*> m_leading_jets;
  std::vector<SelectedJet*> m_leading_jets_eljet;

  EventJets* m_all_jets;
  EventElectrons* m_all_electrons;
  EventMuons* m_all_muons;
  TriggerSF* m_trigger_sf;
  systematic::Systematic m_systematic;
  SelectedJet* electron_jet() const;
};

#endif
