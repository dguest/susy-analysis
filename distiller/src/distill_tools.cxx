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
#include "mctlib.hh"
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
			  const TVector2& met, size_t n_jets){ 
  typedef std::vector<SelectedJet*> JVec; 
  typedef JVec::const_iterator JItr; 
  TLorentzVector mector; 
  mector.SetPtEtaPhiE(1,0,met.Phi(),1); 
  float min_dphi = M_PI; 
  size_t lastj = std::min(n_jets, jets.size()); 
  for(JItr itr = jets.begin(); itr != jets.begin() + lastj; itr++){
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
TVector2 sum_el_pt(const std::vector<Electron*>& els) { 
  TLorentzVector sum; 
  for (auto el: els) { 
    sum += *el; 
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

double get_m_ct(const TLorentzVector& v1, const TLorentzVector& v2) { 
  double et1 = v1.Pt(); 
  double et2 = v2.Pt(); 
  TVector2 diff_pt = v1.Vect().XYvector() - v2.Vect().XYvector(); 
  double mct2 = std::pow(et1 + et2, 2) - std::pow(diff_pt.Mod(), 2); 
  return std::sqrt(mct2); 
}

double get_mctcorr(const TLorentzVector& tv1, const TLorentzVector& tv2, 
		   const TVector2& vmet)
{
  mctlib mct_object;
  
  double v1[4] = {tv1.E(), tv1.Px(), tv1.Py(), tv1.Pz()};
  double v2[4] = {tv2.E(), tv2.Px(), tv2.Py(), tv2.Pz()};
  double vds[4] = {0.0, 0.0, 0.0, 0.0};
  double ptm[2] = {vmet.X(), vmet.Y()};
  return mct_object.mctcorr(v1, v2, vds, ptm, 8e6, 0.0);
}

double get_mt(const std::vector<Electron*>& el, 
	      const std::vector<Muon*>& mu, const TVector2& met) { 
  const double nothing = -1; 
  size_t nel = el.size(); 
  size_t nmu = mu.size(); 
  // require exactly one lepton 
  if (nel + nmu != 1) return nothing; 
  TLorentzVector* lep = 0; 
  if (nel > 0) { 
    lep = el.at(0); 
  } else if (nmu > 0) { 
    lep = mu.at(0); 
  } else { 
    throw std::logic_error("lepton count in get_mt"); 
  }
  TVector2 lep2vec = lep->Vect().XYvector(); 
  return std::sqrt(2*lep2vec.Mod()*met.Mod() - 2*lep2vec*met);
}

double get_mll(const std::vector<Electron*>& el, 
	       const std::vector<Muon*>& mu) { 
  const double nothing = -1; 
  size_t nel = el.size(); 
  size_t nmu = mu.size(); 
  size_t total_lep = nel + nmu; 
  if (total_lep != 2) return nothing; 
  if (nel == 1 && nmu == 1) { 
    return (*el.at(0) + *mu.at(0)).M(); 
  } else if (nel == 2) { 
    return (*el.at(0) + *el.at(1)).M(); 
  } else if (nmu == 2) { 
    return (*mu.at(0) + *mu.at(1)).M(); 
  } else { 
    throw std::logic_error(std::to_string(nel) + " electrons, " + 
			   std::to_string(nmu) + " muons in get_mll"); 
  }
  
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

