#ifndef JET_2D_HISTS
#define JET_2D_HISTS

class Jet; 
namespace H5 { 
  class CommonFG; 
}
class Histogram; 

#include <string> 
#include <boost/noncopyable.hpp>
#include "typedefs.hh"

class Jet2DHists: public boost::noncopyable
{
public: 
  Jet2DHists(const unsigned flags); 
  ~Jet2DHists(); 
  void write_to(H5::CommonFG&); 
  void fill(const Jet&, double weight = 1.0); 
private: 
  Histogram* m_anti_b_vs_anti_light; 
}; 

#endif
