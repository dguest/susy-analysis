#include "main.hh"
#include "susy.h"
#include <iostream>
#include <string> 

#include "SUSYTools/SUSYObjDef.h"

int main (int narg, char* argv[])
{

  SUSYObjDef def; 

  susy buffer; 
  TTree* tree = buffer.fChain; 
  int n_entries = tree->GetEntriesFast(); 
  std::cout << n_entries << " in chain" << std::endl; 

  for (int evt_n = 0; evt_n < 10; evt_n++) { 
    tree->GetEntry(evt_n); 
    
    std::cout << buffer.el_n << " electrons in event " << evt_n
	      << std::endl; 
    std::cout << "in crack? " << (def.IsInCrack(0.4) ? "yes":"no" )
	      << std::endl;
  }

  return 0;
}

