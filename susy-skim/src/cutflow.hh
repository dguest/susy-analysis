#ifndef CUTFLOW_H
#define CUTFLOW_H

class SusyBuffer; 
class SUSYObjDef; 
class FakeMetEstimator;
class SelectedJet;
class SelectedJets; 
class TFile; 
class SusyElectrons; 
class Electron; 
class OutTree; 


#include "TLorentzVector.h"
#include "TVector2.h"
#include "TChain.h"
#include <vector> 
#include <string> 
#include <map>

struct RunInfo { 
  int run_number; 
}; 

//--- these are now (annoyingly) defined in FudgeMCTool.h ---
static const double MeV = 1.0; 
static const double GeV = 1000.0*MeV; 

namespace cutflag { 
  const unsigned verbose         = 1u << 0; 
  const unsigned is_data         = 1u << 1; 
  const unsigned is_signal       = 1u << 2;  
  const unsigned use_low_pt_jets = 1u << 3; 
  const unsigned debug_susy      = 1u << 4; 
  const unsigned raw_evt_info    = 1u << 5; 
}

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
			      unsigned& pass_bits, OutTree& out_tree, 
			      const TVector2& met); 


float get_min_jetmet_dphi(const std::vector<SelectedJet*>&, 
			  const TVector2& ); 

float get_sum_jetmet_dphi(const std::vector<SelectedJet*>&, 
			  const TVector2& ); 


template<typename M, typename A>
void remove_overlaping(const M& mask, A& altered, const float delta_r); 



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

bool pass_mainz_ctag(const SelectedJet& jet); 

std::vector<int> fill_muons(const SusyBuffer& buffer, SUSYObjDef& def, 
			    unsigned flags, const RunInfo& info); 


TVector2 get_met(const SusyBuffer& buffer, 
		 SUSYObjDef& def, 
		 const RunInfo&, const std::vector<int>& muon_idx );

// bool has_lower_pt(const TLorentzVector&, const TLorentzVector&); 
bool has_lower_pt(const TLorentzVector*, const TLorentzVector*); 

class SelectedJet: public TLorentzVector { 
public: 
  SelectedJet(const SelectedJets* container, int jet_index); 
  double combNN_btag() const; 
  int jet_index() const;
  double jfitcomb_cu(const SusyBuffer& buffer, int jet_index) const;
  double jfitcomb_cb(const SusyBuffer& buffer, int jet_index) const;
  double jvf() const; 
  void set_bit(unsigned); 
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

class SelectedJets: public std::vector<SelectedJet*> 
{ 
public: 
  SelectedJets(); 
  SelectedJets(const SusyBuffer& buffer, SUSYObjDef& def, 
	       const unsigned flags, const RunInfo& info);
  ~SelectedJets(); 
private: 
  const SusyBuffer* m_buffer; 
  friend class SelectedJet; 
}; 

class Electron: public TLorentzVector 
{ 
public: 
  Electron(const SusyElectrons* container, int index); 
  bool pass_susy() const; 
private: 
  bool m_pass_susy; 
}; 

class SusyElectrons: public std::vector<Electron*> 
{ 
public: 
  SusyElectrons(); 
  SusyElectrons(const SusyBuffer& buffer, SUSYObjDef& def, 
		const unsigned flags, const RunInfo& info); 
  ~SusyElectrons(); 
private: 
  const SusyBuffer* m_buffer; 
  SUSYObjDef* m_def; 
  const unsigned m_flags; 
  const RunInfo* m_info; 
  friend class Electron; 
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

class SmartChain: public TChain { 
public: 
  SmartChain(std::string tree_name); 
  
  template<typename T, typename Z>
  void SetBranchAddress(T name, Z branch, 
  			bool turnon = false); 

  // void SetBranchAddress(std::string name, void* branch, 
  // 			bool turnon = false); 

private: 
  void SetBranchAddressPrivate(std::string name, void* branch, 
			       bool turnon = false); 
}; 

template<typename T, typename Z>
void SmartChain::SetBranchAddress(T name, Z branch, 
				  bool turnon) { 
  *branch = 0; 
  SetBranchAddressPrivate(name, branch, turnon); 
}


class OutTree
{
public: 
  OutTree(std::string file, std::string tree = "evt_tree", 
	  const unsigned flags = 0); 
  ~OutTree(); 
  void fill(); 
  void clear_buffer(); 
  
  unsigned pass_bits; 
  double met; 
  double min_jetmet_dphi; 
  double sum_jetmet_dphi; 
  
  class Jet 
  {
  public: 
    double pt; 
    double eta; 
    double phi; 
    int flavor_truth_label; 
    double cnn_b; 
    double cnn_c; 
    double cnn_u; 
  private: 
    friend class OutTree; 
    void set_branches(TTree*, std::string prefix); 
    void clear(); 
  }; 

  Jet leading_jet_uncensored; 

  Jet leading_jet; 
  Jet subleading_jet; 
  Jet isr_jet; 

private:
  void init(const unsigned flags = 0); 
  TFile* m_file; 
  TTree* m_tree; 
}; 
int fill_cutflow_from_bits(CutCounter&, const unsigned); 

void copy_jet_info(const SelectedJet& , OutTree::Jet&); 

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
      double delr = (*itr)->DeltaR(*altered.at(idx)); 
      if (delr > delta_r) { 
	new_container.push_back(altered.at(idx)); 
      }
    }
    altered = new_container; 
  }
} 

// --- obsolete? ---

class TrigSimulator { 
public: 
  TrigSimulator(float fraction_preswap = 2281.26 / 4689.68); 
  bool get_decision(const SusyBuffer&); 
private: 
  float m_frac_preswap; 
}; 

bool IsSmartLArHoleVeto(TVector2 met,
			FakeMetEstimator& fakeMetEst,
			const SusyBuffer& buffer, 
			SUSYObjDef& def, 
			std::vector<SelectedJet> baseline_jets );


#endif //CUTFLOW_H
