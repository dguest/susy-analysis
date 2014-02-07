#include "bitset_functions.hh"
#include "constants_distiller.hh"
#include "Jets.hh"
#include "Leptons.hh"
#include "Met.hh"
#include "EventBits.hh"
#include "EventObjects.hh"
#include "ObjectComposites.hh"

//_________________________________________________________________
// cutflow functions

namespace { 
  bool pass_chf_check(const std::vector<SelectedJet*> jets); 
}

namespace bits { 

  // --- event object bits ----
  // main function 
  ull_t event_object_bits(const EventObjects& obj) { 
    ull_t pass_bits = signal_jet_bits(obj.signal_jets); 
    pass_bits |= pass::cosmic_muon | pass::bad_muon; 
    for (auto mu: obj.preselected_muons) { 
      if (mu->bad() ) pass_bits &=~ pass::bad_muon; 
    }
    for (auto mu: obj.after_overlap_muons) { 
      if (mu->cosmic() ) pass_bits &=~ pass::cosmic_muon; 
    }

    if (pass_chf_check(obj.signal_jets)) pass_bits |= pass::jet_chf; 
    pass_bits |= control_lepton_bits(obj.control_electrons, 
				     obj.control_muons); 
    if (obj.veto_electrons.size() == 0) pass_bits |= pass::electron_veto; 
    if (obj.veto_muons.size() == 0) pass_bits |= pass::muon_veto; 
    if (obj.veto_jets.size() == 0) pass_bits |= pass::jet_clean; 

    return pass_bits; 

  }

  ull_t signal_jet_bits(const std::vector<SelectedJet*>& jets) { 
    ull_t pass_bits = 0; 
    if (jets.size() >= 1) { 
      float leading_jet_pt = jets.at(0)->Pt(); 
      if (leading_jet_pt > FILTER_LEADING_JET_PT) { 
	pass_bits |= pass::leading_jet; 
      }
      if (leading_jet_pt > CUTFLOW_JET1_PT) { 
	pass_bits |= pass::cutflow_leading; 
      }
    }
    if (jets.size() >= N_SR_JETS) pass_bits |= pass::n_jet; 
    if (jets.size() >= 2 && jets.at(1)->Pt() > CUTFLOW_JET2_PT) { 
      pass_bits |= pass::cutflow_jet2; 
    }
    if (jets.size() < 3 || jets.at(2)->Pt() < CUTFLOW_JET3_PT_VETO) { 
      pass_bits |= pass::cutflow_jet3; 
    }

    int n_tags = 0; 
    const unsigned medium = jetbit::jfc_medium_anti_u | jetbit::jfc_anti_b; 
    const int n_jets = std::min(jets.size(), N_SR_JETS); 
    for (auto itr = jets.begin(); itr != jets.begin() + n_jets; itr++) { 
      const auto jet = **itr; 
      bool med_tagged = ((jet.bits() & medium) == medium); 
      if (med_tagged && jet.in_tag_eta()) n_tags++; 
    }
    if (n_tags >= 1) pass_bits |= pass::tagged; 
    if (n_tags >= 2) pass_bits |= pass::double_tagged; 
    return pass_bits; 
  } 

  ull_t control_lepton_bits(const std::vector<Electron*>& el, 
			    const std::vector<Muon*>& mu) { 
    ull_t pass_bits = 0; 
    size_t n_el = el.size(); 
    size_t n_mu = mu.size(); 
    if (n_el == 1) pass_bits |= pass::control_electron; 
    if (n_mu == 1) pass_bits |= pass::control_muon; 
    return pass_bits; 
  }

  // --- ObjectComposites bits ---
  ull_t object_composit_bits(const ObjectComposites& par) { 
    ull_t pass_bits = 0; 
    if (par.energy_weighted_time < ENERGY_WEIGHTED_TIME_MAX) {
      pass_bits |= pass::energy_wt_time; 
    }
    if (par.min_jetmet_dphi > DPHI_JET_MET_MIN) { 
      pass_bits |= pass::dphi_jetmet_min; 
    }
    if (par.met_eff > MET_EFF_MIN) pass_bits |= pass::met_eff; 

    if (par.mass_ct > SR_MCT_MIN) pass_bits |= pass::sr_mct; 
    if (par.mass_ct > CROF_MCT_MIN) pass_bits |= pass::crof_mct; 
    if (par.mass_cc > M_CC_MIN) pass_bits |= pass::m_cc; 

    return pass_bits; 
  }

  // --- other bits ---
  ull_t met_bits(const Mets& mets){ 
    ull_t pass_bits = 0; 
    if (mets.nominal.Mod() > FILTER_MET) { 
      pass_bits |= pass::met; 
    }
    if (mets.nominal.Mod() > CUTFLOW_MET) { 
      pass_bits |= pass::cutflow_met; 
    }
    if (mets.muon.Mod() > FILTER_MET) pass_bits |= pass::mu_met; 

    return pass_bits; 
  }

  ull_t bad_tile_bits(const Mets& mets, 
		      const std::vector<SelectedJet*> jets) { 
    ull_t bits = pass::bad_tile_stmet | pass::bad_tile_mumet; 
  
    for (auto jet: jets) { 
      if (jet->bad_tile(mets.nominal)) bits &=~ pass::bad_tile_stmet; 
      if (jet->bad_tile(mets.muon)) bits &=~ pass::bad_tile_stmet; 
    }
    return bits; 
  }
}

namespace { 
  bool pass_chf_check(const std::vector<SelectedJet*> jets) { 
    size_t njet = std::min(jets.size(), CHF_CHECK_NJET); 
    for (auto itr = jets.cbegin(); itr != jets.cbegin() + njet; itr++) { 
      const auto& jet = **itr; 
      if (!jet.pass_chf()) return false; 
    }
    return true; 
  }

}
