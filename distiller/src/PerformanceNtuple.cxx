#include "PerformanceNtuple.hh"
#include "RunBits.hh"
#include "SmartChain.hh"
#include "SusyBuffer.h"
#include <string> 
#include <fstream>
#include <iostream>
#include <boost/format.hpp>
#include <boost/scoped_ptr.hpp>

int make_perf_ntuple(std::string input, unsigned flags, 
		     std::string out_file) 
{
  std::ofstream nullstream("/dev/null"); 
  std::streambuf* out_buffer = nullstream.rdbuf(); 
  if (flags & cutflag::verbose) { 
    out_buffer = std::cout.rdbuf(); 
  }
  std::ostream out_stream(out_buffer); 

  boost::scoped_ptr<SmartChain> input_chain(new SmartChain("susy")); 

  if (input.size() == 0) { 
    throw std::runtime_error("I need files to run!"); 
  }

  int ret_code = input_chain->Add(input.c_str(),-1); 
  if (ret_code != 1) { 
    std::string err = (boost::format("bad file: %s") % 
		       input).str(); 
    throw std::runtime_error(err); 
  }

  unsigned branches = cutflag::mv3 | cutflag::jetfitter_charm; 

  BranchNames branch_names; 
  branch_names.trigger = "EF_xe80_tclcw_loose"; 

  SusyBuffer buffer(input_chain.get(), branches, branch_names); 
  int n_entries = input_chain->GetEntries(); 


  return 0; 
}
