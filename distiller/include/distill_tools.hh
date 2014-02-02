#ifndef DISTILL_TOOLS_HH
#define DISTILL_TOOLS_HH

class SusyBuffer; 
class SUSYObjDef; 
class FakeMetEstimator;
class SelectedJet;
class TFile; 
class TVector2; 
struct RunInfo; 
namespace outtree { 
  class OutTree; 
  class Jet; 
  class ScaleFactor; 
}; 
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

// ---- copy functions ----
void copy_jet_info(const SelectedJet* , outtree::Jet&); 

void copy_scale_factor(const SelectedJet*, outtree::ScaleFactor&, 
		       btag::OperatingPoint); 

template<typename T, typename B>
void copy_id_vec_to_box(const T& vec, B& box); 

void copy_leading_jet_info(const std::vector<SelectedJet*>& signal_jets, 
			   outtree::OutTree& out_tree); 

void copy_cjet_truth(outtree::OutTree& out_tree, 
		     const std::vector<SelectedJet*>& jets); 

void copy_event_truth(outtree::OutTree& out_tree, const SusyBuffer& buffer, 
		      unsigned branches); 

void copy_met(outtree::OutTree& out, const Mets& mets); 

// ---- cutflow functions ----
ull_t control_lepton_bits(const std::vector<Electron*>&, 
			  const std::vector<Muon*>&);

ull_t z_control_bits(const std::vector<Electron*>&, 
		     const std::vector<Muon*>&); 

ull_t signal_jet_bits(const std::vector<SelectedJet*>& signal_jets); 

ull_t met_bits(const Mets& met); 



TVector2 get_boson_child_pt(const std::vector<Electron*>&, 
			    const std::vector<Muon*>&); 

float get_min_jetmet_dphi(const std::vector<SelectedJet*>&, 
			  const TVector2& ); 

TVector2 sum_muon_pt(const std::vector<Muon*>& muons); 

double get_htx(const std::vector<SelectedJet*>&, const size_t x);

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

void add_skim_report(const SkimReport& report, outtree::OutTree&); 


// ---- templates -----

template<typename T, typename B>
void copy_id_vec_to_box(const T& vec, B& box) { 
  float nominal = 1.0; 
  float up = 1.0; 
  float down = 1.0; 
  for (auto itr: vec) { 
    nominal *= itr->id_sf(); 
    up *= itr->id_sf() + itr->id_sf_err(); 
    down *= itr->id_sf() - itr->id_sf_err(); 
  }
  box.nominal = nominal; 
  box.up = up; 
  box.down = down; 
}


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
