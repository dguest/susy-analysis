#include "OutputFilter.hh"
#include "RunInfo.hh"
#include "typedefs.hh"
#include "EventBits.hh"
#include "cutflag.hh"

OutputFilter::OutputFilter(const RunInfo&, unsigned run_flags ) { 
  m_save_all_events = (run_flags & cutflag::save_all_events); 
}; 

bool OutputFilter::should_save_event(const ull_t event_mask) { 
  if (m_save_all_events) return true; 
  const ull_t preselection = pass::grl | pass::lar_error | pass::tile_error |
    pass::core | pass::tile_trip | pass::vxp_gt_4trk | pass::jet_clean | 
    pass::leading_jet;
  // -- signal region --
  const ull_t signal_selection = preselection | pass::met | pass::n_jet; 
  if ( (signal_selection & event_mask) == signal_selection) return true; 

  // -- W mu control region 
  const ull_t wmunu_sel = preselection | pass::met | pass::control_muon; 
  if ( (wmunu_sel & event_mask) == wmunu_sel) return true; 

  // // -- Z control regions
  // const ull_t zmumu_selection = preselection | pass::os_zmass_mu_pair; 
  // if ( (zmumu_selection & event_mask) == zmumu_selection) return true; 

  // const ull_t zee_selection = preselection | pass::os_zmass_el_pair; 
  // if ( (zee_selection & event_mask) == zee_selection) return true; 
  return false; 
}
  
