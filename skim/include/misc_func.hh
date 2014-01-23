#ifndef MISC_FUNC_HH
#define MISC_FUNC_HH

#include <string>
#include <stdexcept>

class TTree; 

void setOrThrow(TTree& chain, const std::string& name, void* variable); 

class MissingBranchError: public std::runtime_error
{
public: 
  MissingBranchError(const std::string&); 
}; 


#endif
