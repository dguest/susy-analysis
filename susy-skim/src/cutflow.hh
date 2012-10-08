#ifndef CUTFLOW_H
#define CUTFLOW_H

class SusyBuffer; 
class SUSYObjDef; 
class FakeMetEstimator;
class SelectedJet;
class EventJets; 
class TFile; 
class EventElectrons; 
class Electron; 
class EventMuons; 
class Muon; 
class TVector2; 

#include "OutTree.hh" // copy_jet_info

#include "TLorentzVector.h"
#include "TChain.h"
#include <vector> 
#include <string> 
#include <map>
#include <set>
#include <cassert>

struct RunInfo { 
  int run_number; 
}; 

static const double MeV = 1.0; 
static const double GeV = 1000.0*MeV; 

namespace jetbit { 
  const unsigned good            = 1u << 0; 
  const unsigned low_pt          = 1u << 1; 
  const unsigned el_overlap      = 1u << 2; 
}

std::vector<std::pair<std::string, int> >
run_cutflow(std::vector<std::string> files, 
	    RunInfo info, const unsigned flags = 0, 
	    std::string out_ntuple_name = ""); 

void do_multijet_calculations(const std::vector<SelectedJet*>& leading_jets, 
			      unsigned& pass_bits, 
			      OutTree& out_tree, 
			      const TVector2& met); 


float get_min_jetmet_dphi(const std::vector<SelectedJet*>&, 
			  const TVector2& ); 

float get_sum_jetmet_dphi(const std::vector<SelectedJet*>&, 
			  const TVector2& ); 


template<typename M, typename A>
void remove_overlaping(const M& mask, A& altered, const float delta_r); 

template<typename T>
std::vector<int> get_indices(const T&); 

template<typename T>
std::vector<T*> filter_susy(const std::vector<T*>&); 

bool check_if_jet(int jet_n, 
		  const SusyBuffer& buffer, 
		  SUSYObjDef& def, 
		  const unsigned flags, 
		  const RunInfo& info); 

bool check_if_electron(int iEl,
		       const SusyBuffer& buffer, 
		       SUSYObjDef& def, 
		       const unsigned flags, 
		       const RunInfo& info);

bool check_if_muon(int iMu,
		   const SusyBuffer& buffer, 
		   SUSYObjDef& def, 
		   const unsigned flags);

bool pass_mainz_ctag(const SelectedJet* jet); 


TVector2 get_met(const SusyBuffer& buffer, 
		 SUSYObjDef& def, 
		 const RunInfo&, const std::vector<int>& muon_idx );


bool has_lower_pt(const TLorentzVector*, const TLorentzVector*); 

class SelectedJet: public TLorentzVector { 
public: 
  SelectedJet(const EventJets* container, int jet_index); 
  double combNN_btag() const; 
  int index() const;
  double jfitcomb_cu(const SusyBuffer& buffer, int jet_index) const;
  double jfitcomb_cb(const SusyBuffer& buffer, int jet_index) const;
  double jvf() const; 
  void set_bit(unsigned); 
  void unset_bit(unsigned); 
  unsigned bits() const; 
  double pb() const; 
  double pu() const; 
  double pc() const; 
  int flavor_truth_label() const; 
private: 
  double m_cnn_b; 
  double m_cnn_c; 
  double m_cnn_u; 
  int m_jet_index;
  float m_jvf; //jet_jvtxf
  unsigned m_bits; 
  int m_flavor_truth_label; 
}; 

class EventJets: public std::vector<SelectedJet*> 
{ 
public: 
  EventJets(const SusyBuffer& buffer, SUSYObjDef& def, 
	    unsigned flags, const RunInfo& info);
  ~EventJets(); 
private: 
  EventJets() {}; 
  EventJets(const EventJets&) {}; 
  const SusyBuffer* m_buffer; 
  friend class SelectedJet; 
}; 

class Electron: public TLorentzVector 
{ 
public: 
  Electron(const EventElectrons* container, int index); 
  bool pass_susy() const; 
  int index() const; 
private: 
  bool m_pass_susy; 
  int m_index; 
}; 

class EventElectrons: public std::vector<Electron*> 
{ 
public: 
  EventElectrons(const SusyBuffer& buffer, SUSYObjDef& def, 
		 unsigned flags, const RunInfo& info); 
  ~EventElectrons(); 
private: 
  EventElectrons() {}; 
  EventElectrons(const EventElectrons&) {}; 
  const SusyBuffer* m_buffer; 
  SUSYObjDef* m_def; 
  unsigned m_flags; 
  const RunInfo* m_info; 
  friend class Electron; 
}; 

class Muon: public TLorentzVector
{
public: 
  Muon(const EventMuons* container, int index); 
  bool pass_susy() const; 
  int index() const; 
private: 
  bool m_pass_susy; 
  int m_index; 
}; 

class EventMuons: public std::vector<Muon*>
{
public: 
  EventMuons(const SusyBuffer& buffer, SUSYObjDef& def, 
	     unsigned flags, const RunInfo& info); 
  ~EventMuons(); 
private: 
  EventMuons() {}; 
  EventMuons(const EventMuons&) {}; 
  const SusyBuffer* m_buffer; 
  SUSYObjDef* m_def; 
  unsigned m_flags; 
  const RunInfo* m_info; 
  friend class Muon; 
}; 
// --- io things ----

class CutCounter: public std::map<std::string, int> 
{
public: 
  int& operator[](std::string key); 
  std::vector< std::pair<std::string, int> > get_ordered_cuts() const; 
private: 
  std::vector<std::string> m_cuts; 
}; 

class BitmapCutflow 
{ 
public: 
  BitmapCutflow(std::string first_name = "n_events"); 
  void add(std::string name, unsigned mask); 
  int fill(const unsigned mask); 
  std::vector< std::pair<std::string, int> > get() const; 
private: 
  typedef std::vector<std::pair<unsigned,std::string> > MaskName; 
  std::string m_first_name; 
  CutCounter m_counter; 
  MaskName m_mask_name; 
};

class SmartChain: public TChain { 
public: 
  SmartChain(std::string tree_name); 
  
  template<typename T, typename Z>
  void SetBranchAddress(T name, Z branch, 
  			bool turnon = false); 

  std::vector<std::string> get_all_branch_names() const; 
private: 
  void SetBranchAddressPrivate(std::string name, void* branch, 
			       bool turnon = false); 
  std::vector<std::string> m_set_branches; 
  std::set<std::string> m_set_branch_set; 
}; 



void copy_jet_info(const SelectedJet* , OutTree::Jet&); 

// ---- templates -----

template<typename M, typename A>
void remove_overlaping(const M& mask, A& altered, const float delta_r) { 
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
    }
    altered = new_container; 
  }
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

template<typename T, typename Z>
void SmartChain::SetBranchAddress(T name, Z branch, 
				  bool turnon) { 
  *branch = 0; 
  SetBranchAddressPrivate(name, branch, turnon); 
}

#endif //CUTFLOW_H
