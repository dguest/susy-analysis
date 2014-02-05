#include "distill_tools.hh"
#include "constants_cutflow.hh"
#include "Jets.hh"
#include "Leptons.hh"
#include "Met.hh"
#include "constants.hh"
#include "RunInfo.hh"

#include "cutflag.hh"
#include "EventBits.hh"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <string> 
#include <streambuf>
#include <cassert>
#include <cstdlib> // getenv, others
#include <cstdio>
#include <map>
#include "TVector2.h"
#include <math.h>
#include <stdexcept> 

//_________________________________________________________________
// cutflow functions
 
ull_t signal_jet_bits(const std::vector<SelectedJet*>& jets) { 
  ull_t pass_bits = 0; 
  if (jets.size() > 0) { 
    float leading_jet_pt = jets.at(0)->Pt(); 
    if (leading_jet_pt > FILTER_LEADING_JET_PT) { 
      pass_bits |= pass::leading_jet; 
    }
    if (leading_jet_pt > CUTFLOW_LEADING_JET_PT) { 
      pass_bits |= pass::cutflow_leading; 
    }
  }
  if (jets.size() >= N_SR_JETS) pass_bits |= pass::n_jet; 

  const unsigned medium = jetbit::jfc_medium_anti_u | jetbit::jfc_anti_b; 
  if (jets.size() >= 3) { 
    if ( (jets.at(1)->bits() & jets.at(2)->bits() & medium) == medium) { 
      pass_bits |= pass::cutflow_tag_2; 
    }
  }
  const unsigned loose = jetbit::jfc_loose_anti_u | jetbit::jfc_anti_b; 
  if (jets.size() >= 4) { 
    if ( (jets.at(1)->bits() & jets.at(2)->bits() & loose) == loose) { 
      if ( (jets.at(3)->bits() & medium) == medium) {
	pass_bits |= pass::cutflow_tag_1; 
      }
    }
  }
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

//____________________________________________________________
// utility functions 

TVector2 get_boson_child_pt(const std::vector<Electron*>& el, 
			    const std::vector<Muon*>& mu) { 
  const size_t n_muons = mu.size(); 
  for (size_t mu_n = 0; mu_n < n_muons; mu_n++) { 
    for (size_t mu_n2 = mu_n + 1; mu_n2 < n_muons; mu_n2++) { 
      const auto& mu1 = *mu.at(mu_n); 
      const auto& mu2 = *mu.at(mu_n2); 
      if (are_os_zmass(mu1, mu2)) { 
	return (mu1 + mu2).Vect().XYvector(); 
      }
    }
  }
  if (mu.size() == 1) { 
    return mu.at(0)->Vect().XYvector(); 
  }
  if (el.size() == 1) { 
    return el.at(0)->Vect().XYvector(); 
  }
  return TVector2(); 
}

float get_min_jetmet_dphi(const std::vector<SelectedJet*>& jets, 
			  const TVector2& met){ 
  typedef std::vector<SelectedJet*> JVec; 
  typedef JVec::const_iterator JItr; 
  TLorentzVector mector; 
  mector.SetPtEtaPhiE(1,0,met.Phi(),1); 
  float min_dphi = M_PI; 
  for(JItr itr = jets.begin(); itr != jets.end(); itr++){
    float deltaphi = fabs(mector.DeltaPhi(**itr)); 
    min_dphi = std::min(deltaphi, min_dphi); 
  }
  return min_dphi; 
}

TVector2 sum_muon_pt(const std::vector<Muon*>& muons) { 
  TLorentzVector sum; 
  for (auto mu: muons) { 
    sum += *mu; 
  }
  return sum.Vect().XYvector(); 
}

double get_htx(const std::vector<SelectedJet*>& jets, const size_t x){ 
  double htx = 0; 
  const unsigned required_bits = jetbit::pass_susy_def; 
  size_t n_jets = jets.size(); 
  if (n_jets <= x) return 0.0; 
  for (auto itr = jets.cbegin() + x; itr != jets.cend(); itr++) { 
    unsigned jet_bits = (*itr)->bits(); 
    bool has_required = ( (jet_bits & required_bits) == required_bits); 
    if (has_required) { 
      htx += (*itr)->Pt(); 
    }
  }
  return htx; 
}
double get_energy_weighted_time(const std::vector<SelectedJet*>& jets,
				size_t max_jet) { 
  const size_t n_jets = std::min(max_jet, jets.size()); 
  double denom = 0; 
  double num = 0; 
  
  for (auto itr = jets.begin(); itr != jets.begin() + n_jets; itr++) { 
    const auto& jet = **itr; 
    denom += jet.E(); 
    num += jet.E() * jet.timing(); 
  }
  return num/denom; 
}

bool has_lower_pt(const TLorentzVector* v1, const TLorentzVector* v2) { 
  return v1->Pt() < v2->Pt(); 
}
bool has_higher_pt(const TLorentzVector* v1, const TLorentzVector* v2) { 
  return v1->Pt() > v2->Pt(); 
}

void set_bit(std::vector<SelectedJet*>& jets, unsigned bit) { 
  for (std::vector<SelectedJet*>::iterator itr = jets.begin(); 
       itr != jets.end(); itr++) { 
    (*itr)->set_bit(bit); 
  }
}

void calibrate_jets(std::vector<SelectedJet*> jets, 
		    const BtagCalibration* cal) { 
  for (auto jitr: jets) {
    jitr->set_flavor_tag(cal); 
  }
}

