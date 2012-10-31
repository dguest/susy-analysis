#include "MaskedHistArray.hh"
#include "Histogram.hh"
#include "H5Cpp.h"

#include <vector>
#include <map>
#include <utility>
#include <string> 
#include <cassert> 

MaskedHistArray::MaskedHistArray(const Histogram& base_hist, 
				 std::string tag): 
  m_base_hist(base_hist), 
  m_physics_tag(tag)
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
    if (m_physics_tag.size()) { 
      using namespace H5; 
      DataSet the_ds = out_file.openDataSet(name); 
      StrType vls_type(0, H5T_VARIABLE);
      DataSpace att_space(H5S_SCALAR);
      Attribute tag = the_ds.createAttribute("tag", vls_type, att_space); 
      tag.write(vls_type, &m_physics_tag); 
    }
  }
}

bool MaskedHistArray::has_bit(unsigned bit) const { 
  return (m_bit_to_name.find(bit) != m_bit_to_name.end()); 
}
