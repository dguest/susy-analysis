#ifndef CUTFLOW_H
#define CUTFLOW_H

class SusyBuffer; 
class SUSYObjDef; 
class FakeMetEstimator;
class SelectedJet;
class TFile; 
class TVector2; 
class RunInfo; 
class OutTree; 

#include "TLorentzVector.h"
#include "TChain.h"
#include <vector> 
#include <string> 
#include <map>
#include <set>
#include <cassert>



std::vector<std::pair<std::string, int> >
run_cutflow(std::vector<std::string> files, 
	    RunInfo info, unsigned flags = 0, 
	    std::string out_ntuple_name = ""); 


void copy_leading_jet_info(const std::vector<SelectedJet*>& signal_jets, 
			   OutTree& out_tree); 

unsigned jet_cleaning_bit(const std::vector<SelectedJet*>& preselection_jets);

void fill_cjet_truth(OutTree& out_tree, 
		     const std::vector<SelectedJet*>& jets); 
void fill_event_truth(OutTree& out_tree, const SusyBuffer& buffer, 
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
unsigned get_ctag_bits(const std::vector<SelectedJet*>& jets); 

TVector2 get_met(const SusyBuffer& buffer, 
		 SUSYObjDef& def, 
		 const RunInfo&, const std::vector<int>& muon_idx );


bool has_lower_pt(const TLorentzVector*, const TLorentzVector*); 

void set_bit(std::vector<SelectedJet*>& jets, unsigned bit); 

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


#endif //CUTFLOW_H
