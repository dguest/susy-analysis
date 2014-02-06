#include "EventObjects.hh"
#include "Jets.hh"
#include "Leptons.hh"
#include "CutCounter.hh"
#include "RunInfo.hh"
#include "constants_distiller.hh"
#include "object_selection.hh"


EventObjects::EventObjects(
  const SusyBuffer& buf, SUSYObjDef& def, 
  unsigned flags, const RunInfo& info): 
  all_jets(0), 
  all_electrons(0), 
  all_muons(0)
{ 
  all_jets = new EventJets(buf, def, flags, info); 
  all_electrons = new EventElectrons(buf, def, flags, info); 
  all_muons = new EventMuons(buf, def, flags, info); 
  
}

void EventObjects::do_overlap_removal(CutCounter& ob_counts) { 
  std::sort(all_jets->begin(),all_jets->end(),object::has_higher_pt); 

  preselected_jets = object::preselection_jets(*all_jets); 
  preselected_electrons = object::filter_susy(*all_electrons); 
  preselected_muons = object::filter_susy(*all_muons); 

  ob_counts["preselected_jets"] += preselected_jets.size(); 
  ob_counts["preselected_el"] += preselected_electrons.size(); 
  ob_counts["preselected_mu"] += preselected_muons.size(); 

  // need to get susy muon indices before overlap
  susy_muon_idx = object::get_indices(preselected_muons); 

  // --- overlap removal ---
  after_overlap_jets = object::remove_overlaping(
    preselected_electrons, preselected_jets, REMOVE_JET_CONE); 
  after_overlap_electrons = object::remove_overlaping(
    after_overlap_jets, preselected_electrons, REMOVE_EL_CONE); 
  after_overlap_muons = object::remove_overlaping(
    after_overlap_jets, preselected_muons, REMOVE_MU_CONE); 

  ob_counts["after_overlap_jets"] += after_overlap_jets.size(); 
  ob_counts["after_overlap_el"] += after_overlap_electrons.size(); 
  ob_counts["after_overlap_mu"] += after_overlap_muons.size(); 

  veto_jets = object::bad_jets(after_overlap_jets); 
  veto_electrons = object::veto_electrons(after_overlap_electrons); 
  veto_muons = object::veto_muons(after_overlap_muons); 

  ob_counts["veto_jets"] += veto_jets.size(); 
  ob_counts["veto_el"] += veto_electrons.size(); 
  ob_counts["veto_mu"] += veto_muons.size(); 

  good_jets = object::remove_bad_jets(after_overlap_jets); 
  ob_counts["good_jets"] += good_jets.size(); 
  signal_jets = object::signal_jets(good_jets); 
  control_electrons = object::control_electrons(veto_electrons); 
  control_muons = object::control_muons(veto_muons); 

  ob_counts["signal_jets"] += signal_jets.size(); 
  ob_counts["control_el"] += control_electrons.size(); 
  ob_counts["control_mu"] += control_muons.size(); 

  const int n_leading = std::min(signal_jets.size(), N_SR_JETS); 
  leading_jets.assign(signal_jets.begin(), signal_jets.begin() + n_leading); 
}

EventObjects::~EventObjects() { 
  delete all_jets; 
  delete all_electrons; 
  delete all_muons; 
  all_jets = 0; 
  all_electrons = 0; 
  all_muons = 0; 
}
