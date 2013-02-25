#include "BitmapCutflow.hh"

int& CutCounter::operator[](std::string key) 
{ 
  bool new_cut = (find(key) == end()); 
  if (new_cut) { 
    m_cuts.push_back(key); 
  }
  return std::map<std::string, int>::operator[](key); 
}

std::vector< std::pair<std::string, int> > CutCounter::get_ordered_cuts() 
  const 
{
  typedef std::vector<std::string>::const_iterator IdxItr; 
  std::vector< std::pair<std::string, int> > ordered_cuts; 
  for (IdxItr itr = m_cuts.begin(); itr != m_cuts.end(); itr++) { 
    ordered_cuts.push_back(*find(*itr));
  }
  return ordered_cuts;
}

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
    if (mask & itr->first) { 
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
