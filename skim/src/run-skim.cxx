#include "Skimmer.hh"

#include <vector>
#include <string>
#include <fstream>

// thin wrapper for the Skimmer class
int main(int narg, char* argv[]) { 
  std::vector<std::string> variables; 
  std::ifstream var_file("branches.txt"); 

  std::string line; 
  while (!var_file.eof()) { 
    var_file >> line; 
    variables.push_back(line); 
  }
  Skimmer skim(variables); 
  for (int nn = 1; nn < narg; nn++) { 
    skim.addFile(argv[nn]); 
  }
  skim.makeSkim("skim-output.root"); 
}
