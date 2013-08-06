#include "distill_tools.hh"
#include "StopDistiller.hh"

#include "cutflag.hh"

#include <iostream>
#include "SUSYTools/FakeMetEstimator.h"

#include <boost/format.hpp>

int main(int narg, char* argv[])
{
  std::cout<< "in main" << std::endl;

  std::vector<std::string> input_files; 
  for (int n = 1; n < narg; n++) { 
    input_files.push_back(argv[n]); 
  }

  RunInfo info; 
  // info.grl = "grl.xml"; 
  info.trigger = "EF_xe80_tclcw_tight";
  srand(0); 
  using namespace cutflag; 
  unsigned flags = 0;

  flags |= get_branches; 
  flags |= save_ratios; 
  // flags |= raw_evt_info; 
  // flags |= use_met_reffinal; 
  flags |= verbose; 
  // flags |= debug_cutflow; 
  // flags |= debug_susy; 

  // flags |= is_atlfast; 
  flags |= is_data; 
  // flags |= is_signal; 


  // run the main routine 
  StopDistiller distiller(input_files, info, flags, "output_skim.root"); 
  StopDistiller::Cutflow cut_counters = distiller.run_cutflow(); 
  typedef std::vector<std::pair<std::string, int> > CCOut; 

  float firstcut = cut_counters.at(0).second; 
  for (CCOut::const_iterator cut_itr = cut_counters.begin(); 
       cut_itr != cut_counters.end(); 
       cut_itr++) { 
    std::cout << boost::format("%i events pass %s (%.2f%%)\n") % 
      cut_itr->second % cut_itr->first % 
      ( float(cut_itr->second) * 100 / firstcut) ; 

  }

  std::cout << "done" << std::endl;
  return 0; 

}
