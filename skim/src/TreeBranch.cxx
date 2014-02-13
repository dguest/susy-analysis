#include "TreeBranch.hh"

template<>
void FlatOwnedBranch<int>::dump() const { 
  printf("int %i\n", m_value); 
}
template<>
void FlatOwnedBranch<unsigned>::dump() const { 
  printf("uint %i\n", m_value); 
}
template<>
void FlatOwnedBranch<long long>::dump() const { 
  printf("long long %i\n", static_cast<int>(m_value)); 
}
template<>
void FlatOwnedBranch<float>::dump() const { 
  printf("float %f\n", m_value); 
}
template<>
void FlatOwnedBranch<bool>::dump() const { 
  printf("bool "); 
  puts(m_value ? "true" : "false"); 
}

template<>
void ObjBranch<std::vector<float> >::dump() const { 
  printf("float vec, len %i\n", m_value->size()); 
}
