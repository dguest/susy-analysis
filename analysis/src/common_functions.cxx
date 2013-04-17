#include "common_functions.hh"
#include "PhysicalConstants.hh"
#include <vector>
#include "Jet.hh"
#include "TVector2.h"
#include <cassert>
#include <map>

double get_mttop(const std::vector<Jet>& jets, TVector2 met) 
{
  assert(jets.size() == 3); 
  std::map<double, std::pair<unsigned, unsigned> > mass_pairs; 
  for (unsigned iii = 0; iii < jets.size(); iii++) { 
    for (unsigned jjj = 0; jjj < iii; jjj++) { 
      double mass = (jets.at(iii) + jets.at(jjj)).Mag(); 
      mass_pairs[mass] = std::make_pair(iii,jjj); 
    }
  }
  std::pair<unsigned,unsigned> lowest = mass_pairs.begin()->second; 
  for (unsigned iii = 0; iii < jets.size(); iii++) { 
    if (iii != lowest.first && iii != lowest.second) { 
      return get_mttop(jets.at(iii), met); 
    }
  }
  assert(false); 
}
double get_mttop(const Jet& jet, TVector2 met) { 
  TVector2 jet_pt(jet.Px(), jet.Py()); 
  return sqrt( pow( jet_pt.Mod() + sqrt(met.Mod2() + pow(W_MASS,2) ) , 2) - 
	       (jet_pt + met).Mod2() ); 
}

