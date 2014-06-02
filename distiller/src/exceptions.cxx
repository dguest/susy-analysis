#include "exceptions.hh"

#include <stdexcept>
#include <string>

// base class of errors we don't halt on
TolerableError::TolerableError(std::string msg):
  std::runtime_error(msg) {}

// thrown when there's something wrong with the d3pd (mismatch of vector
// lengths for example)
EventReadError::EventReadError(std::string msg) throw():
  TolerableError(msg)
{
}

// thrown when we can't match an electron to a jet, not sure why this
// should happen
NoJetMatchError::NoJetMatchError():
  TolerableError("no jet found to match electron")
{}
