#ifndef RUN_INFO_H
#define RUN_INFO_H

#include <string> 
#include "systematic_defs.hh"
#include "CutflowConfig.hh"

struct RunInfo { 
  systematic::Systematic systematic; 
  CutflowType cutflow_type; 
  std::string grl; 
  std::string trigger; 
  std::string btag_cal_dir; 
  std::string btag_cal_file; 
  double boson_pt_max_mev; 
  RunInfo(); 
}; 

#endif // RUN_INFO_H
