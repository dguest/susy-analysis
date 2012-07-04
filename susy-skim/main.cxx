#include "main.hh"
#include "susy.h"
#include <iostream>
#include <string> 

int main (int narg, char* argv[])
{

  susy _susy_buffer; 
  TTree* tree = _susy_buffer.fChain; 
  int n_entries = tree->GetEntriesFast(); 
  std::cout << n_entries << " in chain" << std::endl; 

  for (int evt_n = 0; evt_n < 10; evt_n++) { 
    tree->GetEntry(evt_n); 
    
    std::cout << _susy_buffer.el_n << " electrons in event " << evt_n
	      << std::endl; 
  }

  return 0;
}

