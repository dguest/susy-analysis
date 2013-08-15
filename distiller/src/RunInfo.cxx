#include "RunInfo.hh"

RunInfo::RunInfo(): 
  systematic(systematic::NONE), 
  cutflow_type(CutflowType::NOMINAL), 
  boson_pt_max_mev(-1)
{
}
