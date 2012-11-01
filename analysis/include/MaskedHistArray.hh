#ifndef MASKED_HIST_ARRAY_H
#define MASKED_HIST_ARRAY_H

#include <vector>
#include <map>
#include <utility>
#include <string> 


namespace H5 { 
  class CommonFG; 
}

#include "Histogram.hh"

class MaskedHistArray
{
public: 
  MaskedHistArray(const Histogram& base_hist, std::string tag = ""); 
  void add_mask(unsigned bitmask, std::string name = ""); 
  template<typename T> void fill(T values, const unsigned mask); 
  std::map<std::string,Histogram> get_hists() const; 
  void write_to(H5::CommonFG& out_file, std::string stub = "") const; 
private: 
  bool has_bit(unsigned) const; 
  Histogram m_base_hist; 
  typedef std::vector<std::pair<unsigned, Histogram> > Hists;
  Hists m_hists; 
  typedef std::map<unsigned, std::string> Names;
  Names m_bit_to_name; 
  std::string m_physics_tag; 
}; 

template<typename T>
void MaskedHistArray::fill(T values, const unsigned mask) { 
  for (Hists::iterator itr = m_hists.begin(); itr != m_hists.end(); itr++){ 
    const unsigned hist_mask = itr->first; 
    if ( (hist_mask & mask) == hist_mask) { 
      itr->second.fill(values); 
    }
  }
}

#endif 
