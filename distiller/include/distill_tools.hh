#ifndef DISTILL_TOOLS_HH
#define DISTILL_TOOLS_HH

class SUSYObjDef; 
class FakeMetEstimator;
class SelectedJet;
class TFile; 
class TVector2; 
struct RunInfo; 
class BtagCalibration; 
class TLorentzVector; 
class Muon; 
class Electron; 
class Mets; 
class SkimReport; 

// #include "TChain.h"
#include <vector> 
#include <string> 
#include <map>
#include <set>
#include <cassert>
#include <cmath>
#include "btag_defs.hh"
#include "typedefs.hh"
#include "constants_distiller.hh"

// ---- cutflow functions ----
// TODO: move these into a new file, they depend on a bunch of crap
ull_t control_lepton_bits(const std::vector<Electron*>&, 
			  const std::vector<Muon*>&);

ull_t z_control_bits(const std::vector<Electron*>&, 
		     const std::vector<Muon*>&); 

ull_t signal_jet_bits(const std::vector<SelectedJet*>& signal_jets); 

ull_t met_bits(const Mets& met); 

ull_t bad_tile_bits(const Mets& mets, const std::vector<SelectedJet*> jets); 

// --- utility functions

TVector2 get_boson_child_pt(const std::vector<Electron*>&, 
			    const std::vector<Muon*>&); 

float get_min_jetmet_dphi(const std::vector<SelectedJet*>&, 
			  const TVector2& ); 

TVector2 sum_muon_pt(const std::vector<Muon*>& muons); 

double get_htx(const std::vector<SelectedJet*>&, const size_t x);

double get_energy_weighted_time(const std::vector<SelectedJet*>&, 
				size_t n_jets); 

bool pass_chf_check(const std::vector<SelectedJet*> jets); 

template<typename T> 
bool has_os_zmass_pair(const std::vector<T*>&); 

template<typename T>
bool are_os_zmass(const T&, const T&); 

template<typename M, typename A>
A remove_overlaping(const M& mask, A altered, const float delta_r); 

template<typename T>
std::vector<int> get_indices(const T&); 

template<typename T>
std::vector<T*> filter_susy(const std::vector<T*>&); 

bool has_lower_pt(const TLorentzVector*, const TLorentzVector*); 
bool has_higher_pt(const TLorentzVector*, const TLorentzVector*); 

void set_bit(std::vector<SelectedJet*>& jets, unsigned bit); 

void calibrate_jets(std::vector<SelectedJet*> jets, const BtagCalibration*); 


// ---- templates -----

template<typename M, typename A>
A remove_overlaping(const M& mask, A altered, const float delta_r) { 
  for (auto mitr = mask.begin(); mitr != mask.end(); mitr++) { 
    A new_container; 
    for (auto vic = altered.begin(); vic != altered.end(); vic++) { 
      assert((*mitr)->Pt() > 0); 
      double delr = (*mitr)->DeltaR(**vic); 
      if (delr > delta_r) { 
	new_container.push_back(*vic); 
      }
    }
    altered = new_container; 
  }
  return altered; 
} 

template<typename T>
std::vector<int> get_indices(const T& cont) 
{ 
  std::vector<int> indices; 
  for (typename T::const_iterator itr = cont.begin(); 
       itr != cont.end(); itr++ ) { 
    indices.push_back((*itr)->index()); 
  }
  return indices; 
}; 

template<typename T>
std::vector<T*> filter_susy(const std::vector<T*>& in) 
{ 
  std::vector<T*> out; 
  for (typename std::vector<T*>::const_iterator itr = in.begin(); 
	 itr!= in.end(); itr++) { 
    if ( (*itr)->pass_susy() ) { 
      out.push_back(*itr); 
    }
  }
  return out; 
}

template<typename T> 
bool has_os_zmass_pair(const std::vector<T*>& lep) { 
  size_t n_lep = lep.size(); 
  for (size_t iii = 0; iii < n_lep; iii++) { 
    for (size_t jjj = iii + 1; jjj < n_lep; jjj++) { 
      const T& lep1 = *lep.at(jjj); 
      const T& lep2 = *lep.at(iii); 
      if (are_os_zmass(lep1, lep2)) return true; 
    }
  }
  return false; 
}

template<typename T> 
bool are_os_zmass(const T& lep1, const T& lep2) { 
  float ch_prod = lep1.charge() * lep2.charge(); 
  float mass = (lep1 + lep2).Mag(); 
  bool os = (ch_prod > -1.5 && ch_prod < -0.5); 
  bool zmass = (std::fabs(mass - Z_MASS) < Z_MASS_WINDOW); 
  return os && zmass; 
  
}

#endif 
