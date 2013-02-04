#include "CheckSize.hh"
#include <boost/format.hpp>
#include <stdexcept>

EventReadError::EventReadError(std::string msg) throw(): 
  std::runtime_error(msg)
{
} 

void CHECK::throw_size(std::string formattable, int expected, int actual){
  if (expected != actual){ 
    std::string err = (boost::format(formattable) % expected % 
		       actual).str();	
    throw EventReadError(err); 
  }
}

