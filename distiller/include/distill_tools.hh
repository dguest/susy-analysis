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

// #include "TChain.h"
#include <vector> 
#include <string> 
#include <map>
#include <set>
#include <cassert>
#include <cmath>
#include "btag_defs.hh"
#include "typedefs.hh"
#include "DistillerConstants.hh"



void copy_jet_info(const SelectedJet* , outtree::Jet&); 
void copy_scale_factor(const SelectedJet*, outtree::ScaleFactor&, 
		       btag::Tagger); 

void copy_leading_jet_info(const std::vector<SelectedJet*>& signal_jets, 
			   outtree::OutTree& out_tree); 

ull_t jet_cleaning_bit(const std::vector<SelectedJet*>& preselection_jets);

ull_t control_lepton_bits(const std::vector<Electron*>&, 
			  const std::vector<Muon*>&);
ull_t signal_jet_bits(const std::vector<SelectedJet*>& signal_jets); 
ull_t met_bits(const TVector2& met); 

template<typename T> 
bool has_os_zmass_pair(const std::vector<T*>&); 

void fill_cjet_truth(outtree::OutTree& out_tree, 
		     const std::vector<SelectedJet*>& jets); 

void fill_event_truth(outtree::OutTree& out_tree, const SusyBuffer& buffer, 
		      unsigned branches); 

float get_min_jetmet_dphi(const std::vector<SelectedJet*>&, 
			  const TVector2& ); 

double get_htx(const std::vector<SelectedJet*>&);

template<typename M, typename A>
A remove_overlaping(const M& mask, const A& altered, const float delta_r); 

template<typename T>
std::vector<int> get_indices(const T&); 

template<typename T>
std::vector<T*> filter_susy(const std::vector<T*>&); 

bool pass_mainz_ctag(const SelectedJet* jet); 

TVector2 get_met(const SusyBuffer& buffer, 
		 SUSYObjDef& def, 
		 const RunInfo&, const std::vector<int>& muon_idx );
TVector2 get_mumet(const TVector2& met, const std::vector<Muon*> muons); 

bool has_lower_pt(const TLorentzVector*, const TLorentzVector*); 

void set_bit(std::vector<SelectedJet*>& jets, unsigned bit); 

void calibrate_jets(std::vector<SelectedJet*> jets, const BtagCalibration*); 

// --- io things ----


// ---- templates -----

template<typename M, typename A>
A remove_overlaping(const M& mask, const A& altered, const float delta_r) { 
  A final = altered; 
  for (auto mitr = mask.begin(); mitr != mask.end(); mitr++) { 
    A new_container; 
    for (auto vic = final.begin(); vic != final.end(); vic++) { 
      assert((*mitr)->Pt() > 0); 
      double delr = (*mitr)->DeltaR(**vic); 
      if (delr > delta_r) { 
	new_container.push_back(*vic); 
      }
    }
    final = new_container; 
  }
  return final; 
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
      float ch_prod = lep.at(jjj)->charge() * lep.at(iii)->charge(); 
      float mass = (*lep.at(jjj) + *lep.at(iii)).Mag(); 
      bool os = (ch_prod > -1.5 && ch_prod < -0.5); 
      bool zmass = (std::fabs(mass - Z_MASS) < Z_MASS_WINDOW); 
      if (os && zmass) return true; 
    }
  }
  return false; 
}

#endif 
