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
  builder.build(); 
}
