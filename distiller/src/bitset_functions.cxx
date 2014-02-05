#include "bitset_functions.hh"
#include "constants_distiller.hh"
#include "Jets.hh"
#include "Leptons.hh"
#include "Met.hh"
#include "EventBits.hh"
#include "zmass_functions.hh"

//_________________________________________________________________
// cutflow functions


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
    if ((jet.bits() & medium) == medium) n_tags++; 
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

ull_t z_control_bits(const std::vector<Electron*>& el, 
		     const std::vector<Muon*>& mu) { 
  ull_t pass_bits = 0; 
  // both of these true if _any_ leptons are in the z mass window
  // (windown defined as a constant)
  if (has_os_zmass_pair(el)) pass_bits |= pass::os_zmass_el_pair; 
  if (has_os_zmass_pair(mu)) pass_bits |= pass::os_zmass_mu_pair; 
  return pass_bits; 
}

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

ull_t bad_tile_bits(const Mets& mets, const std::vector<SelectedJet*> jets) { 
  ull_t bits = pass::bad_tile_stmet | pass::bad_tile_mumet; 
  
  for (auto jet: jets) { 
    if (jet->bad_tile(mets.nominal)) bits &=~ pass::bad_tile_stmet; 
    if (jet->bad_tile(mets.muon)) bits &=~ pass::bad_tile_stmet; 
  }
  return bits; 
}

