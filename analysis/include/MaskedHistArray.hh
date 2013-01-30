#ifndef MASKED_HIST_ARRAY_H
#define MASKED_HIST_ARRAY_H

#include <vector>
#include <set>
#include <utility>
#include <string> 


namespace H5 { 
  class CommonFG; 
}

#include "Histogram.hh"

class MaskedHist: public Histogram
{
public: 
  MaskedHist(const Histogram& base_hist, unsigned mask, std::string name, 
	     unsigned anti = 0); 
  template<typename T> 
  void fill_mask(T values, unsigned mask, double w = 1.0); 
  void write_to(H5::CommonFG& out_file, std::string stub = "", 
		std::string tag = "") const;
private: 
  unsigned m_mask; 
  std::string m_name; 
  unsigned m_anti; 
}; 

class MaskedHistArray
{
public: 
  MaskedHistArray(const Histogram& base_hist, std::string tag = ""); 
  void add_mask(unsigned bitmask, std::string name = "", unsigned anti = 0); 
  template<typename T> void fill(T values, unsigned mask, double w = 1.0); 
  // std::map<std::string,Histogram> get_hists() const; 
  void write_to(H5::CommonFG& out_file, std::string stub = "") const; 
private: 
  Histogram m_base_hist; 
  typedef std::vector<MaskedHist> Hists;
  Hists m_hists; 
  std::set<std::string> m_names; 
  std::string m_physics_tag; 
}; 

template<typename T>
void MaskedHist::fill_mask(T values, unsigned mask, double weight) { 
  bool pass_mask = ( (m_mask & mask) == m_mask); 
  bool pass_anti = ( (m_anti & mask) == 0); 
  if (pass_mask && pass_anti) { 
    fill(values, weight); 
  }
}

template<typename T>
void MaskedHistArray::fill(T values,  unsigned mask, double weight) { 
  for (Hists::iterator itr = m_hists.begin(); itr != m_hists.end(); itr++){ 
    itr->fill_mask(values, mask, weight); 
  }
}

#endif 
