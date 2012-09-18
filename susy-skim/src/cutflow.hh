#ifndef CUTFLOW_H
#define CUTFLOW_H

class SusyBuffer; 
class SUSYObjDef; 
class TVector2; 
class FakeMetEstimator;
class SelectedJet;

#include "TLorentzVector.h"
#include "TVector2.h"
#include "TChain.h"
#include <vector> 
#include <string> 
#include <map>

struct RunInfo { 
  int run_number; 
}; 

static const double MeV = 1.0; 
static const double GeV = 1000.0*MeV; 

class SmartChain: public TChain { 
public: 
  SmartChain(std::string tree_name); 
  void SetBranchAddress(std::string name, void* branch); 
}; 


namespace cutflag { 
  const unsigned verbose         = 1u << 0; 
  const unsigned is_data         = 1u << 1; 
  const unsigned is_signal       = 1u << 2;  
  const unsigned use_low_pt_jets = 1u << 3; 
}


std::vector<std::pair<std::string, int> >
run_cutflow(std::vector<std::string> files, 
	    RunInfo info, const unsigned flags = 0); 



bool IsSmartLArHoleVeto(TVector2 met,
			FakeMetEstimator& fakeMetEst,
			const SusyBuffer& buffer, 
			SUSYObjDef& def, 
			std::vector<SelectedJet> baseline_jets );


bool check_if_jet(int jet_n, 
		  const SusyBuffer& buffer, 
		  SUSYObjDef& def, 
		  const unsigned flags); 

bool check_if_electron(int iEl,
		       const SusyBuffer& buffer, 
		       SUSYObjDef& def, 
		       const unsigned flags, 
		       const RunInfo& info);

bool check_if_muon(int iMu,
		   const SusyBuffer& buffer, 
		   SUSYObjDef& def, 
		   const unsigned flags);

class TrigSimulator { 
public: 
  TrigSimulator(float fraction_preswap = 2281.26 / 4689.68); 
  bool get_decision(const SusyBuffer&); 
private: 
  float m_frac_preswap; 
}; 

/*TVector2 get_MET(const SusyBuffer& buffer, 
  SUSYObjDef& def, 
  const RunInfo&); */
class SelectedJet: public TLorentzVector { 
public: 
  SelectedJet(const SusyBuffer& buffer, int jet_index); 
  double combNN_btag() const; 
  int jet_index() const;
  double jfitcomb_cu(const SusyBuffer& buffer, int jet_index) const;
  double jfitcomb_cb(const SusyBuffer& buffer, int jet_index) const;
private: 
  double m_combNN_btag_wt; 
  int m_jet_index;
  // double cu;
  // double cb; 
  // double m_cu;
  // double m_cb;
}; 

class CutCounter: public std::map<std::string, int> 
{
public: 
  int& operator[](std::string key); 
  std::vector< std::pair<std::string, int> > get_ordered_cuts() const; 
private: 
  std::vector<std::string> m_cuts; 
}; 

#endif //CUTFLOW_H
