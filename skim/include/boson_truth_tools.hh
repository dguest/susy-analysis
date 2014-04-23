#ifndef BOSON_TRUTH_TOOLS_HH
#define BOSON_TRUTH_TOOLS_HH

#include <stdexcept>
#include <string>

class SusyBuffer;

float get_boson_truth_pt(const SusyBuffer& buffer);

class BosonError: public std::range_error
{
public:
  BosonError(const std::string& what);
};

#endif
