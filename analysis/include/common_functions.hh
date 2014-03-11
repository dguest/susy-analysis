#ifndef COMMON_FUNCTIONS_HH
#define COMMON_FUNCTIONS_HH

#include <vector>
#include <string>
#include <stdexcept>

class Jet; 
class TVector2; 
class TTree; 

// --- branch checking functions ---

template<typename T>
void set_branch(TTree* tree, const std::string& name, T* var); 
// gets called by above
void internal_set(TTree* tree, const std::string& name, 
		  const std::string& type_name, void* var); 

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

// --- mt top functions (don't know if they will ever be used) ---

double get_mttop(const std::vector<Jet>& jets, TVector2 met); 
double get_mttop(const Jet& jet, TVector2 met); 


#endif
