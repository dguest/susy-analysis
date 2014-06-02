#ifndef EXCEPTIONS_HH
#define EXCEPTIONS_HH

#include <stdexcept>

class TolerableError: public std::runtime_error
{
public:
  TolerableError(std::string msg);
};

class EventReadError : public TolerableError
{
public:
  EventReadError(std::string msg) throw();
  // event_read_error(event_read_error&) throw();
  // event_read_error& operator=(const event_read_error&) throw();
  // virtual ~event_read_error() throw();
  // virtual const char* what() throw();
};

class NoJetMatchError: public TolerableError
{
public:
  NoJetMatchError();
};

#endif
