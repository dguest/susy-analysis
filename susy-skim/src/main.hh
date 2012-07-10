#ifndef MAIN_H
#define MAIN_H

class susy; 
class SUSYObjDef; 

#include "TLorentzVector.h"

struct RunInfo { 
  bool is_data; 
  int run_number; 
}; 
bool check_lar_hole_veto(int jet_n, 
			 const susy& buffer, 
			 SUSYObjDef& def, 
			 const RunInfo&); 

bool check_if_jet(int jet_n, 
		  const susy& buffer, 
		  SUSYObjDef& def, 
		  const RunInfo&); 

TVector2 get_MET(const susy& buffer, 
		  SUSYObjDef& def, 
		  const RunInfo&);

class BaselineJet:public TLorentzVector { 
public: 
  BaselineJet(const susy& buffer, int jet_index); 
  double combNN_btag(); 
private: 
  double m_combNN_btag_wt; 
}; 

#endif //MAIN_H
