#include "StackerExceptions.hh"

MissingBranch::MissingBranch(const std::string& what):
  std::runtime_error(what)
{
}


