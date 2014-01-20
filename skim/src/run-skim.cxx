#include "Skimmer.hh"

#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>

// thin wrapper for the Skimmer class
int main(int narg, char* argv[]) { 
  std::vector<std::string> variables; 
  std::string varf = "branches.txt"; 
  std::ifstream var_file(varf.c_str()); 

  std::string line; 
  if (!var_file.is_open()) throw std::runtime_error("can't read " + varf);
  while (!var_file.eof()) { 
    var_file >> line; 
    variables.push_back(line); 
  }
  Skimmer skim(variables); 
  for (int nn = 1; nn < narg; nn++) { 
    skim.addFile(argv[nn]); 
  }
  skim.makeSkim("skim-output.root"); 
  return 0; 
}
