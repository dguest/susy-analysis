#include "object_selection.hh"
#include "DistillerConstants.hh"
#include "JetConstants.hh"
#include "Jets.hh"
#include "Leptons.hh"
#include <cassert> 
#include <vector> 

namespace object { 
  Electrons veto_electrons(const Electrons& preselected_electrons) { 
    Electrons out; 
    for (auto el: preselected_electrons) {
      if (el->rel_isolation() < VETO_ELECTRON_REL_ISOLATION) { 
	out.push_back(el); 
      }
    }
    return out; 
  }
  Electrons control_electrons(const Electrons& electrons) { 
    Electrons out; 
    for (auto el: electrons) { 
      if (el->Pt() > CONTROL_ELECTRON_PT) { 
	out.push_back(el); 
      }
    }
    return out; 
  }

  Muons veto_muons(const Muons& preselected_muons){ 
    Muons out; 
    for (auto mu: preselected_muons) {
      if ( mu->isolation() < VETO_MUON_ISOLATION) { 
	out.push_back(mu); 
      }
    }
    return out; 
  }
  Muons control_muons(const Muons& muons) { 
    Muons out; 
    for (auto mu: muons) { 
      if ( mu->Pt() > CONTROL_MUON_PT) { 
	out.push_back(mu); 
      }
    }
    return out; 
  }

  Jets preselection_jets(const Jets& jets){ 
    Jets out; 
    for (auto jet_itr: jets){
      auto& jet = *jet_itr; 
      bool is_low_pt = jet.Pt() < JET_PT_CUT; 
      bool is_good_eta = std::fabs(jet.Eta()) < PRESELECTION_JET_ETA; 
      if (!is_low_pt && is_good_eta) { 
	out.push_back(jet_itr); 
      }
    }
    return out; 
  }

  Jets remove_bad_jets(const Jets& jets) {
    Jets out; 
    for (auto itr: jets){
      auto& jet = *itr; 
      if (jet.bits() & jetbit::pass_susy_def) { 
	out.push_back(itr); 
      }
    }
    return out; 
  }

  Jets veto_jets(const Jets& jets) { 
    Jets out; 
    for (auto jet_itr: jets){
      auto& jet = *jet_itr; 
      bool pass_susy = (jet.bits() & jetbit::pass_susy_def); 
      if (!pass_susy) { 
	out.push_back(jet_itr); 
      }
    }
    return out; 
  }
  Jets signal_jets(const Jets& jets) { 
    Jets out; 
    for (auto jet_itr: jets){
      const SelectedJet& jet = *jet_itr; 
      bool signal_pt = jet.Pt() > SIGNAL_JET_PT_CUT; 
      bool tag_eta = fabs(jet.Eta()) < JET_TAGGING_ETA_LIM; 
      bool jvf = (jet.jvf() > JET_JVF_CUT) || jet.Pt() > JET_PT_IGNORE_JVF; 
      if (signal_pt && tag_eta && jvf ) { 
	out.push_back(jet_itr); 
      }
    }
    return out; 
  }

  SelectedJet* get_leptojet(const Jets& jets, const TLorentzVector& lepton){
    SelectedJet* nearest_jet = 0; 
    float min_delta_r = 10; 
    for (auto j_itr: jets) {
      float delta_r = lepton.DeltaR(*j_itr); 
      if (delta_r < min_delta_r) { 
	nearest_jet = j_itr; 
	min_delta_r = delta_r; 
      }
    }
    assert(nearest_jet); 
    return nearest_jet; 
  }
  
}
