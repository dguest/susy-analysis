#ifndef RUN_INFO_H
#define RUN_INFO_H

#include <string> 

struct RunInfo { 
  int run_number; 
  std::string grl; 
  std::string trigger; 
  std::string btag_cal_dir; 
  std::string btag_cal_file; 
}; 

#endif // RUN_INFO_H
