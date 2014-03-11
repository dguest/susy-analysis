#ifndef STACKEREXCEPTIONS
#define STACKEREXCEPTIONS

#include <stdexcept>

class MissingBranch: public std::runtime_error {
public: 
  MissingBranch(const std::string&); 
}; 


#endif
