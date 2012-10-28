#include "HistBuilder.hh"
#include <iostream> 
#include <string> 

int main(int narg, char* argv[]) { 
  if (narg < 2) { 
    std::cout << "no input" << std::endl;
    return 0; 
  }
  std::string file = argv[1]; 
  HistBuilder builder(file); 
  builder.add_cut_mask("all",0); 
  builder.add_cut_mask("1",1); 
  builder.add_cut_mask("2",2); 
  builder.add_cut_mask("3",3); 

  builder.build(); 
  builder.save("bullshit.h5"); 
}
