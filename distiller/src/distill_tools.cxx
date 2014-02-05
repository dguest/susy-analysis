#include "distill_tools.hh"
#include "constants_distiller.hh"
#include "zmass_functions.hh"
#include "Jets.hh"
#include "Leptons.hh"
#include "Met.hh"
#include "constants.hh"
#include "RunInfo.hh"

#include "cutflag.hh"

#include <algorithm>
#include <string> 
// #include <streambuf>
#include <cassert>
#include <cstdlib> // getenv, others
#include <cstdio>
#include <map>
#include "TVector2.h"
#include <math.h>
#include <stdexcept> 


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

bool pass_chf_check(const std::vector<SelectedJet*> jets) { 
  size_t njet = std::min(jets.size(), CHF_CHECK_NJET); 
  for (auto itr = jets.cbegin(); itr != jets.cbegin() + njet; itr++) { 
    const auto& jet = **itr; 
    if (!jet.pass_chf()) return false; 
  }
  return true; 
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

