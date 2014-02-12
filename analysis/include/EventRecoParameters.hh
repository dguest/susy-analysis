#ifndef EVENT_PARAMETERS_HH
#define EVENT_PARAMETERS_HH

struct EventRecoParameters { 
  bool pass_met_trigger; 
  bool pass_mu_trigger; 
  bool pass_el_trigger; 
  bool pass_event_quality; 
  bool pass_ossf; 
  bool pass_osdf; 

  int n_preselected_jets; 
  int n_signal_jets; 
  int n_veto_electrons; 
  int n_veto_muons; 
  int n_control_electrons; 
  int n_control_muons; 

  float min_jetmet_dphi; 
  float met_eff; 
  float mct; 
  float mcc; 
  float mt; 
  float mll; 
  float htx; 
  float max_lepton_pt;
}; 

#endif 
