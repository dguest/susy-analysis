#ifndef EVENT_PRESELECTOR_HH
#define EVENT_PRESELECTOR_HH

#include <boost/noncopyable.hpp>
#include <string>
#include "typedefs.hh"

class Grl; 
class SusyBuffer; 
class SUSYObjDef; 

class EventPreselector: public boost::noncopyable
{ 
public: 
  EventPreselector(unsigned flags, std::string grl = ""); 
  ~EventPreselector(); 
  ull_t get_preselection_flags(const SusyBuffer& buffer, 
			       SUSYObjDef& def); 
private: 
  const unsigned m_flags; 
  Grl* m_grl; 
}; 
#endif 
