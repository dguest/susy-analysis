#include "Skimmer.hh"

#include <vector>
#include <string>

// thin wrapper for the Skimmer class
int main(int narg, char* argv[]) { 
  std::vector<std::string> variables; 
  variables.push_back("EventNumber"); 
  Skimmer skim(variables); 
  for (int nn = 1; nn < narg; nn++) { 
    skim.addFile(argv[nn]); 
  }
  skim.makeSkim("skim-output.root"); 
}
