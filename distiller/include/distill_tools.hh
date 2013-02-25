#ifndef DISTILL_TOOLS_HH
#define DISTILL_TOOLS_HH

class SusyBuffer; 
class SUSYObjDef; 
class FakeMetEstimator;
class SelectedJet;
class TFile; 
class TVector2; 
class RunInfo; 
namespace outtree { 
  class OutTree; 
  class Jet; 
  class ScaleFactor; 
}; 
class BtagCalibration; 

#include "TLorentzVector.h"
// #include "TChain.h"
#include <vector> 
#include <string> 
#include <map>
#include <set>
#include <cassert>
#include "btag_defs.hh"
#include "typedefs.hh"
// fix this somehow
// #include "OutTree.hh"



void copy_jet_info(const SelectedJet* , outtree::Jet&); 
void copy_scale_factor(const SelectedJet*, outtree::ScaleFactor&, 
		       btag::Tagger); 

void copy_leading_jet_info(const std::vector<SelectedJet*>& signal_jets, 
			   outtree::OutTree& out_tree); 

ull_t jet_cleaning_bit(const std::vector<SelectedJet*>& preselection_jets);

void fill_cjet_truth(outtree::OutTree& out_tree, 
		     const std::vector<SelectedJet*>& jets); 

void fill_event_truth(outtree::OutTree& out_tree, const SusyBuffer& buffer, 
		      unsigned branches); 

float get_min_jetmet_dphi(const std::vector<SelectedJet*>&, 
			  const TVector2& ); 

double get_htx(const std::vector<SelectedJet*>&);

template<typename M, typename A>
std::vector<double> remove_overlaping(const M& mask, 
				      A& altered, 
				      const float delta_r); 

template<typename T>
std::vector<int> get_indices(const T&); 

template<typename T>
std::vector<T*> filter_susy(const std::vector<T*>&); 

bool pass_mainz_ctag(const SelectedJet* jet); 
bool pass_anti_b_ctag(const SelectedJet* jet); 
bool pass_anti_u_ctag(const SelectedJet* jet, float cut); 
ull_t get_ctag_bits(const std::vector<SelectedJet*>& jets); 

TVector2 get_met(const SusyBuffer& buffer, 
		 SUSYObjDef& def, 
		 const RunInfo&, const std::vector<int>& muon_idx );


bool has_lower_pt(const TLorentzVector*, const TLorentzVector*); 

void set_bit(std::vector<SelectedJet*>& jets, unsigned bit); 

void calibrate_jets(std::vector<SelectedJet*> jets, const BtagCalibration*); 

// --- io things ----


// ---- templates -----

template<typename M, typename A>
std::vector<double> remove_overlaping(const M& mask, 
				      A& altered, 
				      const float delta_r) { 
  std::vector<double> delta_r_vals; 
  for (typename M::const_iterator 
	 itr = mask.begin(); 
       itr != mask.end(); 
       itr++) { 
    const unsigned n_jets = altered.size(); 
    A new_container; 
    for (unsigned idx = 0; idx < n_jets; idx++) { 
      assert((*itr)->Pt() > 0); 
      double delr = (*itr)->DeltaR(*altered.at(idx)); 
      if (delr > delta_r) { 
	new_container.push_back(altered.at(idx)); 
      }
      else { 
	delta_r_vals.push_back(delr); 
      }
    }
    altered = new_container; 
  }
  return delta_r_vals; 
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


#endif 
