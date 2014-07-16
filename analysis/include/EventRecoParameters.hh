#ifndef EVENT_PARAMETERS_HH
#define EVENT_PARAMETERS_HH

struct EventRecoParameters {
  bool pass_met_trigger;
  bool pass_two_el_trigger;
  bool pass_two_mu_trigger;
  bool pass_single_el_trigger;
  bool pass_single_mu_trigger;
  bool pass_event_quality;
  bool pass_ossf;
  bool pass_osdf;

  int n_preselected_jets;
  int n_signal_jets;
  int n_baseline_electrons;
  int n_baseline_muons;
  int n_signal_electrons;
  int n_signal_muons;

  float min_jetmet_dphi;
  float lepmet_dphi;
  float met_eff;
  float mct;
  float mcc;
  float mt;
  float mll;
  float htx;

  float first_lepton_pt;
  int first_lepton_pdgid;
  float second_lepton_pt;
  int second_lepton_pdgid;
};

#endif
