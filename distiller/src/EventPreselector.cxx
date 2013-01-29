#include "EventPreselector.hh"

#include "SusyBuffer.h"
#include "Grl.hh"
#include "EventBits.hh"
#include "RunBits.hh"

EventPreselector::EventPreselector(unsigned flags, std::string grl) : 
  m_flags(flags), 
  m_grl(0)
{
  if (grl.size()) { 
    m_grl = new Grl(grl); 
  }
}

EventPreselector::~EventPreselector() { 
  delete m_grl; 
  m_grl = 0; 
}

unsigned EventPreselector::get_preselection_flags(const SusyBuffer& buffer) { 
  unsigned pass_bits = 0; 
  if(buffer.trigger)    pass_bits |= pass::trigger; 
  if(!buffer.larError)  pass_bits |= pass::lar_error; 

  if ( m_flags & cutflag::is_data ) { 
    if (buffer.coreFlags & 0x40000) pass_bits |= pass::core; 
    if (m_grl) { 
      if (m_grl->has_lb(buffer.RunNumber, buffer.lbn)) 
	pass_bits |= pass::grl; 
    }
  }
  else { 
    pass_bits |= pass::core; 
    pass_bits |= pass::grl; 
  }
  return pass_bits; 
}
