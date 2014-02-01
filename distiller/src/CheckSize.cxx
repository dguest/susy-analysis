#include "CheckSize.hh"
#include <stdexcept>

EventReadError::EventReadError(std::string msg) throw(): 
  std::runtime_error(msg)
{
} 

void CHECK::throw_size(const std::string& br_name, int expected, int actual){
  if (expected != actual){ 
    std::string err = "branch " + br_name + " has wrong number of entries,"
      " expected " + std::to_string(expected) + ", found " + 
      std::to_string(actual);
    throw EventReadError(err); 
  }
}

