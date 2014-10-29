#ifndef BOSON_TRUTH_TOOLS_HH
#define BOSON_TRUTH_TOOLS_HH

#include <stdexcept>
#include <string>

class SusyBuffer;

// Return codes for the following functions:
//  * positive values: system pT in MeV
//  * -1: too few daughter particles found to computer system pT
//  * -2: too many daughter particles found
float get_boson_truth_pt(const SusyBuffer& buffer);
float get_ttbar_truth_pt(const SusyBuffer& buffer);

class BosonError: public std::range_error
{
public:
  BosonError(const std::string& what);
};

#endif
