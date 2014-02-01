#ifndef EVENT_PRESELECTOR_HH
#define EVENT_PRESELECTOR_HH

#include <string>
#include "typedefs.hh"

class Grl; 
class SusyBuffer; 
class SUSYObjDef; 

class EventPreselector
{ 
public: 
  EventPreselector(unsigned flags, std::string grl = ""); 
  ~EventPreselector(); 
  EventPreselector(EventPreselector&) = delete; 
  EventPreselector& operator=(EventPreselector&) = delete; 
  ull_t get_preselection_flags(const SusyBuffer& buffer, 
			       SUSYObjDef& def); 
private: 
  const unsigned m_flags; 
  Grl* m_grl; 
}; 
#endif 
