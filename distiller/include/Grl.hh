#ifndef GRL_HH
#define GRL_HH

#include <string> 
#include <boost/noncopyable.hpp>

namespace Root { 
  class TGRLCollection; 
}

class Grl : boost::noncopyable
{ 
public: 
  Grl(std::string grl_location = ""); 
  ~Grl(); 
  bool has_lb(int run_number, int lbn); 
private: 
  Root::TGRLCollection* m_grl; 
}; 
  

#endif 
