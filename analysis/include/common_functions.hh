#ifndef COMMON_FUNCTIONS_HH
#define COMMON_FUNCTIONS_HH

#include <vector>
#include <string>

class Jet; 
class TVector2; 
class TTree; 

template<typename T>
void set_branch(TTree* tree, const std::string& name, T* var); 
// gets called by above
void internal_set(TTree* tree, const std::string& name, 
		  const std::string& type_name, void* var); 

double get_mttop(const std::vector<Jet>& jets, TVector2 met); 
double get_mttop(const Jet& jet, TVector2 met); 

// misc crap to check type names in ROOT
std::string get_name(unsigned long long); 
std::string get_name(double); 
std::string get_name(float); 
std::string get_name(bool); 
std::string get_name(int); 

template<typename T>
void set_branch(TTree* tree, const std::string& name, T* var){ 
  std::string type_name = get_name(*var); 
  internal_set(tree, name, type_name, var); 
} 


#endif
