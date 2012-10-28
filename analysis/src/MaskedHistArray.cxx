#include "MaskedHistArray.hh"
#include "Histogram.hh"
#include "H5Cpp.h"

#include <vector>
#include <map>
#include <utility>
#include <string> 
#include <cassert> 

MaskedHistArray::MaskedHistArray(const Histogram& base_hist): 
  m_base_hist(base_hist) 
{
}

void MaskedHistArray::add_mask(unsigned bitmask, std::string name) 
{ 
  m_hists.push_back(std::make_pair(bitmask, m_base_hist)); 
  assert(m_bit_to_name.find(bitmask) == m_bit_to_name.end()); 
  m_bit_to_name[bitmask] = name; 
}

std::map<std::string, Histogram> MaskedHistArray::get_hists() const 
{
  std::map<std::string, Histogram> return_map; 
  for (Hists::const_iterator hist_itr = m_hists.begin(); 
       hist_itr != m_hists.end(); hist_itr++) { 
    assert(has_bit(hist_itr->first)); 
    std::string name = m_bit_to_name.find(hist_itr->first)->second; 
    return_map.insert(std::make_pair(name,hist_itr->second)); 
  }
  return return_map; 
}

void MaskedHistArray::write_to(H5::CommonFG& out_file, std::string stub) 
  const { 
  for (Hists::const_iterator hist_itr = m_hists.begin(); 
       hist_itr != m_hists.end(); hist_itr++) { 
    std::string name = stub; 
    assert(has_bit(hist_itr->first)); 
    name.append(m_bit_to_name.find(hist_itr->first)->second); 
    hist_itr->second.write_to(out_file, name); 
  }
}

bool MaskedHistArray::has_bit(unsigned bit) const { 
  return (m_bit_to_name.find(bit) != m_bit_to_name.end()); 
}
