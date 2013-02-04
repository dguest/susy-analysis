#ifndef CHECK_SIZE_HH
#define CHECK_SIZE_HH

#include <string> 
#include <stdexcept>

#define CHECK_SIZE(BRANCH, SIZE)\
  CHECK::throw_size("branch " #BRANCH " has wrong number"		\
	     " of entries, expected %i, found %i", SIZE, BRANCH->size())


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
  void throw_size(std::string formattable_err, int expected, int actual); 
}


#endif
