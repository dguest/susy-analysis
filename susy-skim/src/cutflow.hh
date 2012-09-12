#ifndef CUTFLOW_H
#define CUTFLOW_H

class susy; 
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
  bool is_data; 
  int run_number; 
  bool is_signal; 
}; 

static const double MeV = 1.0; 
static const double GeV = 1000.0*MeV; 

class SmartChain: public TChain { 
public: 
  SmartChain(std::string tree_name); 
  void SetBranchAddress(std::string name, void** branch); 
}; 


namespace cutflag { 
  const unsigned verbose = 1u << 0; 
}


std::map<std::string, int> 
run_cutflow(std::vector<std::string> files, 
	    RunInfo info, const unsigned flags = 0); 



bool IsSmartLArHoleVeto(TVector2 met,
			FakeMetEstimator& fakeMetEst,
			const susy& buffer, 
			SUSYObjDef& def, 
			std::vector<SelectedJet> baseline_jets );


bool check_if_jet(int jet_n, 
		  const susy& buffer, 
		  SUSYObjDef& def, 
		  const RunInfo&); 

bool check_if_electron(int iEl,
		       const susy& buffer, 
		       SUSYObjDef& def, 
		       const RunInfo&);

bool check_if_muon(int iMu,
		   const susy& buffer, 
		   SUSYObjDef& def, 
		   const RunInfo&);

class TrigSimulator { 
public: 
  TrigSimulator(float fraction_preswap = 2281.26 / 4689.68); 
  bool get_decision(const susy&); 
private: 
  float m_frac_preswap; 
}; 

/*TVector2 get_MET(const susy& buffer, 
  SUSYObjDef& def, 
  const RunInfo&); */
class SelectedJet: public TLorentzVector { 
public: 
  SelectedJet(const susy& buffer, int jet_index); 
  double combNN_btag() const; 
  int jet_index() const;
  double jfitcomb_cu(const susy& buffer, int jet_index) const;
  double jfitcomb_cb(const susy& buffer, int jet_index) const;
private: 
  double m_combNN_btag_wt; 
  int m_jet_index;
  // double cu;
  // double cb; 
  // double m_cu;
  // double m_cb;
}; 


#endif //CUTFLOW_H
