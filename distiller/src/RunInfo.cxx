#include "RunInfo.hh"

RunInfo::RunInfo(): 
  systematic(systematic::NONE), 
  cutflow_type(CutflowType::NONE), 
  boson_pt_max_mev(-1), 
  truth_met_max_mev(-1)
{
}
