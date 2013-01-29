#ifndef EVENT_PRESELECTOR_HH
#define EVENT_PRESELECTOR_HH

#include <boost/noncopyable.hpp>
#include <string>

class Grl; 
class SusyBuffer; 

class EventPreselector: public boost::noncopyable
{ 
public: 
  EventPreselector(unsigned flags, std::string grl); 
  ~EventPreselector(); 
  unsigned get_preselection_flags(const SusyBuffer& buffer); 
private: 
  const unsigned m_flags; 
  Grl* m_grl; 
}; 
#endif 
