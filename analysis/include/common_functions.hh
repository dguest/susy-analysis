#ifndef COMMON_FUNCTIONS_HH
#define COMMON_FUNCTIONS_HH

#include <vector>

class Jet; 
class TVector2; 
class JetFactory; 

double get_mttop(const std::vector<Jet>& jets, TVector2 met); 
double get_mttop(const Jet& jet, TVector2 met); 

void setup_taggers(JetFactory* factory, unsigned histbuilder_flags); 

#endif
