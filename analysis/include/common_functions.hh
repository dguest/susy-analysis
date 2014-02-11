#ifndef COMMON_FUNCTIONS_HH
#define COMMON_FUNCTIONS_HH

#include <vector>
#include <string>

class Jet; 
class TVector2; 
class TTree; 

void set_branch(TTree* tree, const std::string& name, void* var); 

double get_mttop(const std::vector<Jet>& jets, TVector2 met); 
double get_mttop(const Jet& jet, TVector2 met); 


#endif
