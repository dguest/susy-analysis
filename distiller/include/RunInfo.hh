#ifndef RUN_INFO_H
#define RUN_INFO_H

#include <string> 
#include "systematic_defs.hh"

struct RunInfo { 
  systematic::Systematic systematic; 
  int run_number; 
  std::string grl; 
  std::string trigger; 
  std::string btag_cal_dir; 
  std::string btag_cal_file; 
  RunInfo(); 
}; 

#endif // RUN_INFO_H
