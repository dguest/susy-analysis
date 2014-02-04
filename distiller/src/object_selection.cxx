#include "object_selection.hh"
#include "constants_distiller.hh"
#include "constants_jet.hh"
#include "Jets.hh"
#include "Leptons.hh"
#include "constants_lepton.hh"
#include <cassert> 
#include <vector> 

// TODO: move leptons into leptons, jets into jets? 

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
      bool is_low_pt = jet.Pt() < jet::PRESELECTION_PT; 
      bool is_good_eta = std::abs(jet.Eta()) < jet::PRESELECTION_ETA; 
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

  Jets bad_jets(const Jets& jets) { 
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
      const float abs_eta = std::abs(jet.Eta()); 
      bool signal_pt = jet.Pt() > jet::SIGNAL_PT_CUT; 
      bool ok_eta = abs_eta < jet::SIGNAL_ETA_CUT; 

      bool no_tracks = jet.jvf() < -0.5; 
      bool ok_jvf_frac = jet.jvf() > jet::JVF_CUT; 
      bool ignore_jvf = (jet.Pt() > jet::PT_IGNORE_JVF || 
			 abs_eta > jet::ETA_IGNORE_JFV); 

      bool ok_jvf = (ok_jvf_frac || ignore_jvf || no_tracks); 
      if (signal_pt && ok_eta && ok_jvf ) { 
	out.push_back(jet_itr); 
      }
      // bool tag_eta = fabs(jet.Eta()) < jet::TAGGING_ETA_LIM; 
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
