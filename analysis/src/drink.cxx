#include "HistBuilder.hh"
#include "HistBuilderFlags.hh"
#include <iostream> 
#include <string> 
#include "HistConfig.hh"

int main(int narg, char* argv[]) { 
  if (narg < 2) { 
    std::cout << "no input" << std::endl;
    return 0; 
  }
  unsigned flags = buildflag::verbose | buildflag::fill_truth; 
  std::string file = argv[1]; 
  HistConfig hist_config; 
  HistBuilder* builder = new HistBuilder(file, hist_config, flags); 
  builder->add_cut_mask("all",0); 

  builder->build(); 
  builder->save("bullshit.h5"); 
  delete builder; 
}
