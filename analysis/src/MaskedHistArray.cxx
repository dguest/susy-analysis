#include "MaskedHistArray.hh"
#include "Histogram.hh"
#include "H5Cpp.h"

#include <vector>
#include <map>
#include <utility>
#include <string> 
#include <cassert> 
#include <stdexcept> 

MaskedHist::MaskedHist(const Histogram& base_hist, unsigned mask, 
		       std::string name, unsigned anti): 
  Histogram(base_hist), 
  m_mask(mask), 
  m_name(name), 
  m_anti(anti)
{
  if (m_anti & m_mask) { 
    throw std::runtime_error(__FILE__ ": mask and antimask bits overlap"); 
  }
}
void MaskedHist::write_to(H5::CommonFG& out_file, std::string stub, 
			  std::string tag) const { 
  std::string name = stub; 
  name.append(m_name); 
  Histogram::write_to(out_file, name); 
  
  if (tag.size()) { 
    using namespace H5; 
    DataSet the_ds = out_file.openDataSet(name); 
    StrType vls_type(0, H5T_VARIABLE);
    DataSpace att_space(H5S_SCALAR);
    Attribute tag = the_ds.createAttribute("tag", vls_type, att_space); 
    tag.write(vls_type, &tag); 
  }
}

MaskedHistArray::MaskedHistArray(const Histogram& base_hist, 
				 std::string tag): 
  m_base_hist(base_hist), 
  m_physics_tag(tag)
{
}


void MaskedHistArray::add_mask(unsigned bitmask, std::string name, 
			       unsigned anti) 
{ 
  m_hists.push_back(MaskedHist(m_base_hist, bitmask, name, anti)); 
  assert(m_names.count(name) == 0); 
  m_names.insert(name); 
}

// std::map<std::string, Histogram> MaskedHistArray::get_hists() const 
// {
//   std::map<std::string, Histogram> return_map; 
//   for (Hists::const_iterator hist_itr = m_hists.begin(); 
//        hist_itr != m_hists.end(); hist_itr++) { 
//     assert(has_bit(hist_itr->first)); 
//     std::string name = m_bit_to_name.find(hist_itr->first)->second; 
//     return_map.insert(std::make_pair(name,hist_itr->second)); 
//   }
//   return return_map; 
// }

void MaskedHistArray::write_to(H5::CommonFG& out_file, std::string stub) 
  const { 
  for (Hists::const_iterator hist_itr = m_hists.begin(); 
       hist_itr != m_hists.end(); hist_itr++) { 
    hist_itr->write_to(out_file, stub, m_physics_tag); 
  }
}

