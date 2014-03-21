#include "EventPreselector.hh"

#include "SusyBuffer.h"
#include "Grl.hh"
#include "EventBits.hh"
#include "PileupReweighting.hh"
#include "RunInfo.hh"
#include "cutflag.hh"
#include "SUSYTools/SUSYObjDef.h"
#include "constants_preselection.hh"
#include "PreselectionInfo.hh"

EventPreselector::EventPreselector(unsigned flags, const RunInfo& info) : 
  m_flags(flags), 
  m_grl(0), 
  m_prw(0)
{
  if (info.grl.size()) { 
    m_grl = new Grl(info.grl); 
  }
  if (flags & cutflag::truth && info.pu_config.size() > 0) { 
    m_prw = new PileupReweighting(info, flags); 
  }

}

EventPreselector::~EventPreselector() { 
  delete m_grl; 
  delete m_prw;
}

PreselectionInfo EventPreselector::get_preselection_info(
  const SusyBuffer& buffer, SUSYObjDef& def) const { 
  ull_t pass_bits = 0; 

  unsigned run_number = buffer.RunNumber;
  if (m_prw) { 
    run_number = m_prw->random_run_number(buffer.RunNumber);
  }

  float trigger_sf = 1.0;

#define SET_BIT(name) if (buffer.name) pass_bits |= pass::name

  if (run_number >= FIRST_XE80_RUN) {
    SET_BIT(EF_xe80_tclcw_loose);
  } else {
    SET_BIT(EF_xe80T_tclcw_loose);
    trigger_sf = 0.9;
  }
  SET_BIT(EF_xe80_tclcw_tight);
  
  SET_BIT(EF_mu18_tight_mu8_EFFS);
  SET_BIT(EF_2e12Tvh_loose1);

  SET_BIT(EF_mu24i_tight);
  SET_BIT(EF_e24vhi_medium1);
  SET_BIT(EF_mu36_tight);
  SET_BIT(EF_e60_medium1);

#undef SET_BIT

  pass_bits |= get_presel_bits(buffer, def);

  if(def.IsGoodVertex(buffer.vx_nTracks)) {
    pass_bits |= pass::vxp_gt_4trk; 
  }

  float prw = m_prw ? m_prw->get_pileup_weight(buffer) : 1.0; 
  return {pass_bits, prw, trigger_sf}; 
}

ull_t EventPreselector::get_presel_bits(const SusyBuffer& buffer, 
					SUSYObjDef& def) const { 
  ull_t pass_bits = 0;
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
