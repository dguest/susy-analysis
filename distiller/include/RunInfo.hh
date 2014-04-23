#ifndef RUN_INFO_H
#define RUN_INFO_H

#include <string>
#include "systematic_defs.hh"
#include "CutflowConfig.hh"

struct RunInfo {
  systematic::Systematic systematic;
  CutflowType cutflow_type;
  std::string grl;
  std::string btag_cal_dir;
  std::string btag_cal_file;
  double boson_pt_max_mev;
  double truth_met_max_mev;
  std::string pu_config;
  std::string pu_lumicalc;
  std::string out_ntuple;
  std::string mumet_out_ntuple;
  std::string leptmet_out_ntuple;
  RunInfo();
};

#endif // RUN_INFO_H
