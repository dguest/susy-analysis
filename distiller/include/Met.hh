#ifndef MET_HH
#define MET_HH

#include <vector>

#include "TVector2.h"

class SusyBuffer; 
class SUSYObjDef; 
class RunInfo; 
class Muon; 

struct Mets
{
  Mets(const SusyBuffer&, SUSYObjDef&, 
       const std::vector<int>& mudex, 
       const TVector2& sum_muons); 
  TVector2 nominal; 
  TVector2 muon; 
  TVector2 nominal_up; 
  TVector2 muon_up; 
  TVector2 nominal_down; 
  TVector2 muon_down; 
  TVector2 nominal_res; 
  TVector2 muon_res; 
}; 


#endif 
