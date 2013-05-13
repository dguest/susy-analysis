#include "JetTagRescaler.hh"

#include <fstream> 
#include <iostream> 
#include <vector> 

int main (int nargs, char* argv[]) { 
  if (nargs == 1) return 1; 
  // std::ifstream in_file(argv[1]); 
  JetTagRescaler rescaler(argv[1]); 
  rescaler.set_dummy(1); 
  std::vector<int> flavors = {0, 4, 5, 15, 20}; 
  std::vector<float> pts = {30.0, 35.0, 40.0, 45.0, 150.0, 200.0, 400.0};
  for (auto flav = flavors.begin(); flav != flavors.end(); flav++) { 
    for (auto ptitr = pts.begin(); ptitr != pts.end(); ptitr++) { 
      double pt_mev = (*ptitr) * 1e3; 
      
      std::cout << "flavor: "<< *flav << " pt: " << *ptitr << " scale: "
		<< rescaler.get_sf(pt_mev, *flav, jettag::MEDIUM) 
		<< std::endl; 
    }
  }
}
