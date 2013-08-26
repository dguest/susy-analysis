#ifndef I_JET_TAG_FILTER_HH
#define I_JET_TAG_FILTER_HH

#include <vector> 
class Jet; 

class IJetTagFilter
{
public: 
  virtual bool pass(const std::vector<Jet>& jets) const = 0; 
  virtual double jet_scalefactor(const std::vector<Jet>& jets) const = 0; 
  virtual ~IJetTagFilter(){}; 
};

#endif 
