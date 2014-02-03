#include "EventPreselector.hh"

#include "SusyBuffer.h"
#include "Grl.hh"
#include "EventBits.hh"
#include "cutflag.hh"
#include "SUSYTools/SUSYObjDef.h"

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

ull_t EventPreselector::get_preselection_flags(const SusyBuffer& buffer, 
					       SUSYObjDef& def) { 
  ull_t pass_bits = 0; 

  if (buffer.xe80_tclcw_tight || 
      buffer.xe80T_tclcw_loose || 
      buffer.xe80_tclcw_loose || 
      buffer.EF_mu18_tight_mu8_EFFS || 
      buffer.EF_mu24i_tight || 
      buffer.EF_mu36_tight ||          
      buffer.EF_2e12Tvh_loose1 ||
      buffer.EF_e24vhi_medium1 ||
      buffer.EF_e60_medium1) pass_bits |= pass::trigger; 

  if ( m_flags & cutflag::is_data ) { 
    if(buffer.larError != 2)  pass_bits |= pass::lar_error; 
    if(!buffer.tileError) pass_bits |= pass::tile_error; 
    if ( (buffer.coreFlags & 0x40000) == 0) pass_bits |= pass::core; 
    if (m_grl) { 
      if (m_grl->has_lb(buffer.RunNumber, buffer.lbn)) 
	pass_bits |= pass::grl; 
    }
    bool has_tile_trip = def.IsTileTrip(buffer.RunNumber, buffer.lbn, 
					buffer.EventNumber); 
    if (!has_tile_trip) { 
      pass_bits |= pass::tile_trip; 
    }
  }
  else { 
    pass_bits |= pass::lar_error; 
    pass_bits |= pass::tile_error; 
    pass_bits |= pass::core; 
    pass_bits |= pass::grl; 
    pass_bits |= pass::tile_trip; 
  }

  return pass_bits; 
}
