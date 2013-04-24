#include "object_selection.hh"
#include "DistillerConstants.hh"
#include "JetConstants.hh"
#include "Jets.hh"
#include "Leptons.hh"
#include <vector> 

namespace object { 
  Electrons veto_electrons(const Electrons& preselected_electrons) { 
    Electrons out; 
    for (auto el = preselected_electrons.begin(); 
	 el != preselected_electrons.end(); el++) { 
      if ((*el)->rel_isolation() < VETO_ELECTRON_REL_ISOLATION) { 
	out.push_back(*el); 
      }
    }
    return out; 
  }
  Electrons control_electrons(const Electrons& electrons) { 
    Electrons out; 
    for (auto el = electrons.begin(); el != electrons.end(); el++) { 
      if ((*el)->Pt() > CONTROL_ELECTRON_PT) { 
	out.push_back(*el); 
      }
    }
    return out; 
  }

  Muons veto_muons(const Muons& preselected_muons){ 
    Muons out; 
    for (auto mu = preselected_muons.begin(); mu != preselected_muons.end(); 
	 mu++) { 
      if ( (*mu)->isolation() < VETO_MUON_ISOLATION) { 
	out.push_back(*mu); 
      }
    }
    return out; 
  }
  Muons control_muons(const Muons& muons) { 
    Muons out; 
    for (auto mu = muons.begin(); mu != muons.end(); mu++) { 
      if ( (*mu)->Pt() > CONTROL_MUON_PT) { 
	out.push_back(*mu); 
      }
    }
    return out; 
  }

  Jets preselection_jets(const Jets& jets){ 
    Jets out; 
    for (auto jet_itr = jets.cbegin(); jet_itr != jets.cend(); jet_itr++) {
      auto& jet = **jet_itr; 
      bool is_low_pt = jet.Pt() < JET_PT_CUT; 
      bool is_good_eta = std::fabs(jet.Eta()) < PRESELECTION_JET_ETA; 
      if (!is_low_pt && is_good_eta) { 
	out.push_back(*jet_itr); 
      }
    }
    return out; 
  }

  Jets remove_bad_jets(const Jets& jets) {
    Jets out; 
    for (auto itr = jets.cbegin(); itr != jets.cend(); itr++) { 
      auto& jet = **itr; 
      if (jet.bits() & jetbit::pass_susy_def) { 
	out.push_back(*itr); 
      }
    }
    return out; 
  }

  Jets veto_jets(const Jets& jets) { 
    Jets out; 
    for (auto jet_itr = jets.cbegin(); jet_itr != jets.cend(); jet_itr++) {
      auto& jet = **jet_itr; 
      bool pass_susy = (jet.bits() & jetbit::pass_susy_def); 
      if (!pass_susy) { 
	out.push_back(*jet_itr); 
      }
    }
    return out; 
  }
  Jets signal_jets(const Jets& jets) { 
    Jets out; 
    for (auto jet_itr = jets.cbegin(); jet_itr != jets.cend(); jet_itr++) { 
      const SelectedJet& jet = **jet_itr; 
      bool signal_pt = jet.Pt() > SIGNAL_JET_PT_CUT; 
      bool tag_eta = fabs(jet.Eta()) < JET_TAGGING_ETA_LIM; 
      bool jvf = (jet.jvf() > JET_JVF_CUT) || jet.Pt() > JET_PT_IGNORE_JVF; 
      bool first_jet = (jet_itr == jets.cbegin()); 
      if (signal_pt && (tag_eta || first_jet) && jvf ) { 
	out.push_back(*jet_itr); 
      }
    }
    return out; 
  }
  
}
