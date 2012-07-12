#ifndef MAIN_H
#define MAIN_H

class susy; 
class SUSYObjDef; 
class TVector2; 
class EmulFakeMet;

#include "TLorentzVector.h"

struct RunInfo { 
  bool is_data; 
  int run_number; 
}; 

bool IsSmartLArHoleVeto(TVector2 met,
			EmulFakeMet& fakeMetEst,
			const susy& buffer, 
			SUSYObjDef& def, 
			const RunInfo& info ); 

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
private: 
  double m_combNN_btag_wt; 
  int m_jet_index;
}; 

#endif //MAIN_H
