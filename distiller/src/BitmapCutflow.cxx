#include "BitmapCutflow.hh"


BitmapCutflow::BitmapCutflow(std::string first_name): 
  m_first_name(first_name)
{ 
}

void BitmapCutflow::add(std::string name, ull_t mask) { 
  m_mask_name.push_back(std::make_pair(mask, name)); 
}

int BitmapCutflow::fill(const ull_t mask) { 
  int n_passed = 0; 
  m_counter[m_first_name]++; 
  for (MaskName::const_iterator itr = m_mask_name.begin(); 
       itr != m_mask_name.end(); itr++) { 
    if ( (mask & itr->first) == itr->first) { 
      m_counter[itr->second]++; 
      n_passed++; 
    }
    else { 
      break; 
    }
  }
  return n_passed; 
}

std::vector< std::pair<std::string, int> > BitmapCutflow::get() const { 
  return m_counter.get_ordered_cuts(); 
} 
