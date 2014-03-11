#include "common_functions.hh"
#include "constants_physical.hh"
#include "StackerExceptions.hh"
#include <vector>
#include <string>
#include "Jet.hh"
#include "TVector2.h"
#include "TTree.h"
#include "TLeaf.h"
#include <cassert>
#include <map>
#include <stdexcept>

// ___________________________________________________________________
// branch setting stuff

void internal_set(TTree* tree, const std::string& name, 
		  const std::string& type_name, void* var) { 
  unsigned found = 0; 
  tree->SetBranchStatus(name.c_str(), true, &found); 
  if (found != 1) {
    throw MissingBranch(
      "" + std::to_string(found) + " branches found setting "  + name);
  }
  std::string type = tree->GetLeaf(name.c_str())->GetTypeName(); 
  if (type != type_name) { 
    throw std::logic_error(
      "branch types don't match for '" + name + "': tree has " + 
      type_name + " need a " + type); 
  }
  tree->SetBranchAddress(name.c_str(), var);
}

// misc crap to check type names in ROOT
std::string get_name(unsigned long long) { return "ULong64_t"; }
std::string get_name(double) { return "Double_t"; }
std::string get_name(float) { return "Float_t"; }
std::string get_name(bool) { return "Bool_t"; }
std::string get_name(int) { return "Int_t"; }

// ___________________________________________________________________
// mttop stuff

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

