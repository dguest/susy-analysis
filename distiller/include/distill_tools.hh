#ifndef DISTILL_TOOLS_HH
#define DISTILL_TOOLS_HH

class SUSYObjDef; 
class FakeMetEstimator;
class SelectedJet;
class TVector2; 
class BtagCalibration; 
class TLorentzVector; 
class Muon; 
class Electron; 

// #include "TChain.h"
#include <vector> 
#include <map>
#include <cassert>
#include <cmath>
#include "btag_defs.hh"
#include "constants_distiller.hh"


// --- utility functions

TVector2 get_boson_child_pt(const std::vector<Electron*>&, 
			    const std::vector<Muon*>&); 

float get_min_jetmet_dphi(const std::vector<SelectedJet*>&, 
			  const TVector2& , size_t njet); 

TVector2 sum_muon_pt(const std::vector<Muon*>& muons); 

double get_htx(const std::vector<SelectedJet*>&, const size_t x);

double get_energy_weighted_time(const std::vector<SelectedJet*>&, 
				size_t n_jets); 

// m_ct functions
double get_m_ct(const TLorentzVector& v1, const TLorentzVector& v2); 

double get_mctcorr(const TLorentzVector& v1, const TLorentzVector& v2, 
		   const TVector2& vmet);

double get_mt(const std::vector<Electron*>& , const std::vector<Muon*>& , 
	      const TVector2& ); 

double get_mll(const std::vector<Electron*>& , const std::vector<Muon*>&); 

bool pass_chf_check(const std::vector<SelectedJet*> jets); 

template<typename M, typename A>
A remove_overlaping(const M& mask, A altered, const float delta_r); 

template<typename T>
double scalar_sum_pt(const T& objects); 

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
double scalar_sum_pt(const T& obj) { 
  double sum = 0.0; 
  for (auto itr: obj) { 
    sum += itr->Pt(); 
  }
  return sum; 
}

#endif 
