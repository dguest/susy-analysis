#include "distill_tools.hh"
#include "SusyBuffer.h"
#include "OutTree.hh"
#include "Jets.hh"
#include "Leptons.hh"
#include "constants.hh"
#include "RunInfo.hh"
#include "BitmapCutflow.hh"
#include "SmartChain.hh"
#include "CollectionTreeReport.hh"
#include "EventPreselector.hh"

#include "RunBits.hh"
#include "EventBits.hh"
#include "EventConstants.hh"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <string> 
#include <streambuf>
#include <cassert>
#include <cstdlib> // getenv, others
#include <cstdio>
#include <map>
#include "TChain.h"
#include "TVector2.h"
#include "TError.h"
#include <math.h>
#include <stdexcept> 
#include "SUSYTools/SUSYObjDef.h"
#include "SUSYTools/FakeMetEstimator.h"

#include <boost/format.hpp>
#include <boost/scoped_ptr.hpp>

int main(int narg, char* argv[])
{
  cout<< "in main" << endl;

  std::vector<std::string> input_files; 
  for (int n = 1; n < narg; n++) { 
    input_files.push_back(argv[n]); 
  }

  RunInfo info; 
  info.run_number = 180614; 
  // info.grl = "grl.xml"; 
  srand(0); 
  using namespace cutflag; 
  unsigned flags = verbose | is_signal | debug_susy; 

  flags |= get_branches; 
  flags |= save_ratios; 
  // flags |= raw_evt_info; 
  // flags |= use_met_reffinal; 
  flags |= verbose; 
  flags |= is_atlfast; 
  flags |= debug_cutflow; 

  // flags |= is_data; 

  // run the main routine 
  typedef std::vector<std::pair<std::string, int> > CCOut; 
  CCOut cut_counters = run_cutflow(input_files, 
				   info, 
				   flags, 
				   "output_skim.root"); 

  float firstcut = cut_counters.at(0).second; 
  for (CCOut::const_iterator cut_itr = cut_counters.begin(); 
       cut_itr != cut_counters.end(); 
       cut_itr++) { 
    std::cout << boost::format("%i events pass %s (%.2f%%)\n") % 
      cut_itr->second % cut_itr->first % 
      ( float(cut_itr->second) * 100 / firstcut) ; 

  }

  return 0; 

}
