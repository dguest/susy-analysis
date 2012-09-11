#ifndef CUTFLOW_H
#define CUTFLOW_H

class susy; 
class SUSYObjDef; 
class TVector2; 
class FakeMetEstimator;
class BaselineJet;

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


std::map<std::string, int> run_cutflow(std::vector<std::string> files, 
				       RunInfo info); 



bool IsSmartLArHoleVeto(TVector2 met,
			FakeMetEstimator& fakeMetEst,
			const susy& buffer, 
			SUSYObjDef& def, 
			std::vector<BaselineJet> baseline_jets );


bool check_lar_hole_veto(int jet_n, 
			 const susy& buffer, 
			 SUSYObjDef& def, 
			 const RunInfo&); 

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

/*TVector2 get_MET(const susy& buffer, 
		  SUSYObjDef& def, 
		  const RunInfo&); */
class BaselineJet:public TLorentzVector { 
public: 
  BaselineJet(const susy& buffer, int jet_index); 
  double combNN_btag(); 
  int jet_index();
  double jfitcomb_cu(const susy& buffer, int jet_index);
  double jfitcomb_cb(const susy& buffer, int jet_index);
private: 
  double m_combNN_btag_wt; 
  int m_jet_index;
  double cu;
  double cb; 
  double m_cu;
  double m_cb;
}; 


#endif //CUTFLOW_H
