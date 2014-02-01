#ifndef CHECK_SIZE_HH
#define CHECK_SIZE_HH

#include <string> 
#include <stdexcept>

#define CHECK_SIZE(BRANCH, SIZE)\
  CHECK::throw_size(#BRANCH, SIZE, BRANCH->size())


class EventReadError : public std::runtime_error
{
public: 
  EventReadError(std::string msg) throw(); 
  // event_read_error(event_read_error&) throw(); 
  // event_read_error& operator=(const event_read_error&) throw(); 
  // virtual ~event_read_error() throw(); 
  // virtual const char* what() throw(); 
}; 

namespace CHECK { 
  void throw_size(const std::string& br_name, int expected, int actual); 
}


#endif
