#ifndef MET_HH
#define MET_HH

#include <vector>

#include "TVector2.h"
#include "systematic_defs.hh"

class SusyBuffer; 
class SUSYObjDef; 
class RunInfo; 
class Muon; 

struct Mets
{
  Mets(const SusyBuffer&, SUSYObjDef&, 
       const std::vector<int>& mudex, 
       const TVector2& sum_muons, 
       const TVector2& sum_electrons,
       systematic::Systematic syst); 
  TVector2 nominal; 
  TVector2 muon;
  TVector2 lepton;
}; 


#endif 
