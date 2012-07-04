#ifndef MAIN_H
#define MAIN_H

class susy; 
class SUSYObjDef; 

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

#endif //MAIN_H
