#include "HistBuilder.hh"
#include "HistBuilderFlags.hh"
#include <iostream> 
#include <string> 

int main(int narg, char* argv[]) { 
  if (narg < 2) { 
    std::cout << "no input" << std::endl;
    return 0; 
  }
  unsigned flags = buildflag::verbose & buildflag::fill_truth; 
  std::string file = argv[1]; 
  HistBuilder* builder = new HistBuilder(file, flags); 
  printf("constructed\n"); 
  builder->add_cut_mask("all",0); 
  printf("added cut\n"); 

  builder->build(); 
  printf("filled\n"); 
  builder->save("bullshit.h5"); 
  printf("saved\n"); 
  delete builder; 
}
