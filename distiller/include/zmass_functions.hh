#ifndef ZMASS_FUNCTIONS_HH
#define ZMASS_FUNCTIONS_HH

#include "constants_z.hh"

template<typename T> 
bool has_os_zmass_pair(const std::vector<T*>&); 
template<typename T>
bool are_os_zmass(const T&, const T&); 

// implementation 
template<typename T> 
bool has_os_zmass_pair(const std::vector<T*>& lep) { 
  size_t n_lep = lep.size(); 
  for (size_t iii = 0; iii < n_lep; iii++) { 
    for (size_t jjj = iii + 1; jjj < n_lep; jjj++) { 
      const T& lep1 = *lep.at(jjj); 
      const T& lep2 = *lep.at(iii); 
      if (are_os_zmass(lep1, lep2)) return true; 
    }
  }
  return false; 
}

template<typename T> 
bool are_os_zmass(const T& lep1, const T& lep2) { 
  float ch_prod = lep1.charge() * lep2.charge(); 
  float mass = (lep1 + lep2).Mag(); 
  bool os = (ch_prod > -1.5 && ch_prod < -0.5); 
  bool zmass = (std::fabs(mass - Z_MASS) < Z_MASS_WINDOW); 
  return os && zmass; 
  
}

#endif 
