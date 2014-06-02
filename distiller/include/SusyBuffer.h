//////////////////////// -*-c++-*-//////////////////////////
// This class has been automatically generated on
// Wed Jun 20 17:06:57 2012 by ROOT version 5.30/00
// from TTree susy/susy
// found on file: file1.root
//////////////////////////////////////////////////////////

#ifndef susy_h
#define susy_h

#include <vector>
#include <string>
#include "McParticleBuffer.hh"


class SmartChain;

class SusyBuffer {
public :
  unsigned int RunNumber;
  unsigned int EventNumber;
  unsigned lbn;

  // --- trigger branches ---
  bool EF_xe80_tclcw_tight;
  bool EF_xe80T_tclcw_loose;
  bool EF_xe80_tclcw_loose;
  bool      EF_mu18_tight_mu8_EFFS;
  bool      EF_mu24i_tight;
  bool      EF_mu36_tight;
  bool EF_2e12Tvh_loose1;
  bool EF_e24vhi_medium1;
  bool EF_e60_medium1;

  std::vector<int>* trig_EF_el_EF_e24vhi_medium1;
  std::vector<int>* trig_EF_el_EF_e60_medium1;
  std::vector<int>* trig_EF_el_EF_2e12Tvh_loose1;
  std::vector<float>* trig_EF_el_eta;
  std::vector<float>* trig_EF_el_phi;

  std::vector<int>* trig_EF_trigmuonef_EF_mu18_tight_mu8_EFFS;
  std::vector<int>* trig_EF_trigmuonef_EF_mu24i_tight;
  std::vector<int>* trig_EF_trigmuonef_EF_mu36_tight;
  std::vector<std::vector<float> >* trig_EF_trigmuonef_track_CB_eta;
  std::vector<std::vector<float> >* trig_EF_trigmuonef_track_CB_phi;
  std::vector<std::vector<int> >* trig_EF_trigmuonef_track_CB_hasCB;

  float average_int_per_xing() const;
  unsigned        larError;
  unsigned        tileError;
  unsigned        coreFlags;

  //  --- truth info ---
  unsigned int mc_channel_number;
  int hfor_type;
  int spart1_pdgid;
  int spart2_pdgid;
  float mc_event_weight;

  // needed for boson filter
  McParticleBuffer mc_particles;

  // for overlap removal in stop samples
  float MET_Truth_NonInt_etx;
  float MET_Truth_NonInt_ety;

  // --- Random Event ---
  float Eventshape_rhoKt4LC;

  //  --- MET things ---
  std::vector<std::vector<float> >*    jet_MET_Egamma10NoTau_wet;
  std::vector<std::vector<float> >*    jet_MET_Egamma10NoTau_wpx;
  std::vector<std::vector<float> >*    jet_MET_Egamma10NoTau_wpy;
  std::vector<std::vector<unsigned> >* jet_MET_Egamma10NoTau_statusWord;

  std::vector<std::vector<float> >*    el_MET_Egamma10NoTau_wet;
  std::vector<std::vector<float> >*    el_MET_Egamma10NoTau_wpx;
  std::vector<std::vector<float> >*    el_MET_Egamma10NoTau_wpy;
  std::vector<std::vector<unsigned> >* el_MET_Egamma10NoTau_statusWord;

  float MET_Egamma10NoTau_CellOut_etx; //CellOut
  float MET_Egamma10NoTau_CellOut_ety; //CellOut
  float MET_Egamma10NoTau_CellOut_sumet; //CellOut
  float MET_CellOut_Eflow_STVF_etx;
  float MET_CellOut_Eflow_STVF_ety;
  float MET_CellOut_Eflow_STVF_sumet;
  float MET_Egamma10NoTau_RefGamma_etx;
  float MET_Egamma10NoTau_RefGamma_ety;
  float MET_Egamma10NoTau_RefGamma_sumet;

  std::vector<float>   *jet_jvtxf;
  int             el_n;
  std::vector<float>   *el_eta;
  std::vector<float>   *el_phi;
  std::vector<int>     *el_author;
  std::vector<unsigned int> *el_OQ;
  // vector<unsigned int> *el_OQRecalc;
  std::vector<int>     *el_mediumPP;
  std::vector<int>     *el_tightPP;
  // std::vector<float>     *el_ptcone20;
  std::vector<float>     *el_ptcone30;
  std::vector<float>     *el_topoEtcone30_corrected;
  std::vector<float>     *el_trackIPEstimate_d0_unbiasedpvunbiased;
  std::vector<float>     *el_trackIPEstimate_z0_unbiasedpvunbiased;
  std::vector<float>     *el_trackIPEstimate_sigd0_unbiasedpvunbiased;
  // std::vector<float>     *el_trackd0pv;
  // std::vector<float>     *el_trackz0pv;
  std::vector<float>     *el_charge;
  std::vector<float>   *el_cl_E;
  std::vector<float>   *el_cl_eta;
  std::vector<float>   *el_cl_phi;
  // std::vector<float>   *el_cl_pt;
  std::vector<float>   *el_trackphi;
  std::vector<float>   *el_tracketa;
  std::vector<int>     *el_nPixHits;
  std::vector<int>     *el_nSCTHits;
  int           mu_staco_n;
  std::vector<float>   *mu_staco_pt;
  std::vector<float>   *mu_staco_eta;
  std::vector<float>   *mu_staco_phi;
  // std::vector<float>   *mu_staco_ptcone20;
  std::vector<float>   *mu_staco_ptcone30_trkelstyle;
  std::vector<float>   *mu_staco_etcone30;
  std::vector<float>   *mu_staco_trackIPEstimate_d0_unbiasedpvunbiased;
  std::vector<float>   *mu_staco_trackIPEstimate_z0_unbiasedpvunbiased;
  std::vector<float>   *mu_staco_trackIPEstimate_sigd0_unbiasedpvunbiased;
  std::vector<float>   *mu_staco_charge;
  std::vector<int>     *mu_staco_isCombinedMuon;
  std::vector<int>     *mu_staco_isSegmentTaggedMuon;
  std::vector<int>     *mu_staco_loose;
  std::vector<float>   *mu_staco_id_theta_exPV;
  std::vector<float>   *mu_staco_id_qoverp_exPV;
  std::vector<float>   *mu_staco_me_theta_exPV;
  std::vector<float>   *mu_staco_me_qoverp_exPV;
  std::vector<float>   *mu_staco_ms_phi;
  std::vector<float>   *mu_staco_ms_theta;
  std::vector<float>   *mu_staco_ms_qoverp;
  std::vector<float>   *mu_staco_id_theta;
  std::vector<int>     *mu_staco_nPixHits;
  std::vector<int>     *mu_staco_nSCTHits;
  std::vector<int>     *mu_staco_nTRTHits;
  std::vector<int>     *mu_staco_nPixHoles;
  std::vector<int>     *mu_staco_nSCTHoles;
  std::vector<int>     *mu_staco_nTRTOutliers;
  std::vector<int>     *mu_staco_nPixelDeadSensors;
  std::vector<int>     *mu_staco_nSCTDeadSensors;
  std::vector<float>   *mu_staco_energyLossPar;

  // muon quality cuts
  std::vector<float>   *mu_staco_qoverp_exPV;
  std::vector<float>   *mu_staco_cov_qoverp_exPV;
  std::vector<float>   *mu_staco_d0_exPV;
  std::vector<float>   *mu_staco_z0_exPV;

  // Jet Stuff
  int           jet_n;
  std::vector<float>   *jet_pt;
  std::vector<float>   *jet_eta;
  std::vector<float>   *jet_phi;
  std::vector<float>   *jet_E;
  std::vector<float>   *jet_constscale_eta;
  std::vector<float>   *jet_constscale_phi;
  std::vector<float>   *jet_constscale_E;
  std::vector<float>   *jet_constscale_m;
  std::vector<float>   *jet_ActiveAreaPx;
  std::vector<float>   *jet_ActiveAreaPy;
  std::vector<float>   *jet_ActiveAreaPz;
  std::vector<float>   *jet_ActiveAreaE;
  std::vector<float>   *jet_BCH_CORR_JET;
  // Event Shape is defined above
  std::vector<float>   *jet_emfrac;
  std::vector<float>   *jet_hecf;
  std::vector<float>   *jet_LArQuality;
  std::vector<float>   *jet_HECQuality;
  std::vector<float>   *jet_AverageLArQF;
  std::vector<float>   *jet_Timing;
  std::vector<float>   *jet_sumPtTrk;
  std::vector<float>   *jet_fracSamplingMax;
  std::vector<int>     *jet_SamplingMax;
  std::vector<float>   *jet_NegativeE;
  std::vector<int>     *jet_flavor_truth_label;
  // mu = averageIntPerXing defined elsewhere
  std::vector<int>     *vx_nTracks;
  // run number is somewhere else

  std::vector<float>   *jet_flavor_weight_JetFitterCOMBNN;
  std::vector<float>     *jet_flavor_component_jfitcomb_pu;
  std::vector<float>     *jet_flavor_component_jfitcomb_pb;
  std::vector<float>     *jet_flavor_component_jfitcomb_pc;
  std::vector<float>     *jet_flavor_component_jfitc_pu;
  std::vector<float>     *jet_flavor_component_jfitc_pb;
  std::vector<float>     *jet_flavor_component_jfitc_pc;

  // track branches for single track veto
  std::vector<float>* trk_pt;
  std::vector<float>* trk_eta;
  std::vector<float>* trk_phi_wrtPV;
  std::vector<float>* trk_d0_wrtPV;
  std::vector<float>* trk_z0_wrtPV;
  std::vector<int>* trk_ndof;
  std::vector<float>* trk_chi2;
  std::vector<int>* trk_nPixHits;
  std::vector<int>* trk_nSCTHits;

  double sharktopus;

  SusyBuffer(SmartChain *tree, const unsigned use_branches);
  bool has_truth() const;

private:
  void set_mc_branches(SmartChain* chain, const unsigned br,
		       std::string jc);
  bool m_has_truth;
  float        averageIntPerXing;
};

#endif

