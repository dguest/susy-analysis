//////////////////////// -*-c++-*-//////////////////////////
// This class has been automatically generated on
// Wed Jun 20 17:06:57 2012 by ROOT version 5.30/00
// from TTree susy/susy
// found on file: file1.root
//////////////////////////////////////////////////////////

#ifndef susy_h
#define susy_h

#include <vector> 


struct BranchNames 
{ 
  std::string trigger; 
}; 

using std::vector; 

class SmartChain; 

class SusyBuffer {
public :

  // Declaration of leaf types
  unsigned int RunNumber; 
  unsigned int EventNumber; 
  unsigned lbn; 
  bool          trigger; 
  bool          mu_trigger; 
  float         averageIntPerXing;
  unsigned        larError;
  unsigned        tileError;
  unsigned        coreFlags; 

  // truth info
  int hfor_type; 
  int spart1_pdgid; 
  int spart2_pdgid; 
  float mc_event_weight; 
  // hack for sherpa bug
  std::vector<std::vector<double> >* mcevt_weight; 

  int mc_n; 
  std::vector<float>* mc_pt; 
  std::vector<float>* mc_eta; 
  std::vector<float>* mc_phi; 
  std::vector<float>* mc_m; 
  std::vector<int>* mc_status; 
  std::vector<int>* mc_pdgId; 

  //Random Event
  float Eventshape_rhoKt4LC; 

  // MET things
  vector<vector<float> >*    jet_MET_Egamma10NoTau_wet;
  vector<vector<float> >*    jet_MET_Egamma10NoTau_wpx;
  vector<vector<float> >*    jet_MET_Egamma10NoTau_wpy;
  vector<vector<unsigned> >* jet_MET_Egamma10NoTau_statusWord;

  vector<vector<float> >*    el_MET_Egamma10NoTau_wet;
  vector<vector<float> >*    el_MET_Egamma10NoTau_wpx;
  vector<vector<float> >*    el_MET_Egamma10NoTau_wpy;
  vector<vector<unsigned> >* el_MET_Egamma10NoTau_statusWord;

  float MET_Egamma10NoTau_CellOut_etx; //CellOut
  float MET_Egamma10NoTau_CellOut_ety; //CellOut
  float MET_Egamma10NoTau_CellOut_sumet; //CellOut
  float MET_CellOut_Eflow_STVF_etx;
  float MET_CellOut_Eflow_STVF_ety; 
  float MET_CellOut_Eflow_STVF_sumet;
  float MET_Egamma10NoTau_RefGamma_etx;
  float MET_Egamma10NoTau_RefGamma_ety;
  float MET_Egamma10NoTau_RefGamma_sumet;

  float MET_RefFinal_etx; 
  float MET_RefFinal_ety; 
  
  vector<float>   *jet_jvtxf; 
  int             el_n;
  vector<int>     *el_author;
  vector<unsigned int> *el_OQ;
  // vector<unsigned int> *el_OQRecalc;
  vector<int>     *el_mediumPP;
  vector<int>     *el_tightPP;
  vector<float>     *el_ptcone20;
  vector<float>     *el_trackd0pv;
  vector<float>     *el_trackz0pv;
  vector<float>     *el_charge;
  vector<float>   *el_cl_E;
  vector<float>   *el_cl_eta;
  vector<float>   *el_cl_phi;
  vector<float>   *el_cl_pt;
  vector<float>   *el_trackphi;
  vector<float>   *el_tracketa;
  vector<int>     *el_nPixHits;
  vector<int>     *el_nSCTHits;
  int           mu_staco_n;
  vector<float>   *mu_staco_E;
  vector<float>   *mu_staco_pt;
  vector<float>   *mu_staco_eta;
  vector<float>   *mu_staco_phi;
  vector<float>   *mu_staco_ptcone20; 
  vector<float>   *mu_staco_charge;
  vector<int>     *mu_staco_isCombinedMuon;
  vector<int>     *mu_staco_isSegmentTaggedMuon;
  vector<int>     *mu_staco_loose;
  vector<float>   *mu_staco_id_theta_exPV;
  vector<float>   *mu_staco_id_qoverp_exPV;
  vector<float>   *mu_staco_me_theta_exPV;
  vector<float>   *mu_staco_me_qoverp_exPV;
  vector<float>   *mu_staco_ms_phi;
  vector<float>   *mu_staco_ms_theta;
  vector<float>   *mu_staco_ms_qoverp;
  vector<float>   *mu_staco_id_theta;
  vector<int>     *mu_staco_nBLHits;
  vector<int>     *mu_staco_nPixHits;
  vector<int>     *mu_staco_nSCTHits;
  vector<int>     *mu_staco_nTRTHits;
  vector<int>     *mu_staco_nPixHoles;
  vector<int>     *mu_staco_nSCTHoles;
  vector<int>     *mu_staco_nTRTOutliers;
  vector<int>     *mu_staco_nPixelDeadSensors;
  vector<int>     *mu_staco_nSCTDeadSensors;
  vector<int>     *mu_staco_expectBLayerHit;

  // Jet Stuff
  int           jet_n;
  vector<float>   *jet_pt;
  vector<float>   *jet_eta;
  vector<float>   *jet_phi;
  vector<float>   *jet_E;
  vector<float>   *jet_constscale_eta;
  vector<float>   *jet_constscale_phi;
  vector<float>   *jet_constscale_E;
  vector<float>   *jet_constscale_m;
  vector<float>   *jet_ActiveAreaPx; 
  vector<float>   *jet_ActiveAreaPy; 
  vector<float>   *jet_ActiveAreaPz; 
  vector<float>   *jet_ActiveAreaE; 
  // Event Shape is defined above
  vector<float>   *jet_emfrac;
  vector<float>   *jet_hecf;
  vector<float>   *jet_LArQuality;
  vector<float>   *jet_HECQuality;
  vector<float>   *jet_AverageLArQF;
  vector<float>   *jet_Timing;
  vector<float>   *jet_sumPtTrk;
  vector<float>   *jet_fracSamplingMax;
  vector<int>     *jet_SamplingMax;
  vector<float>   *jet_NegativeE;
  vector<int>     *jet_flavor_truth_label;
  // mu = averageIntPerXing defined elsewhere 
  vector<int>     *vx_nTracks;
  // run number is somewhere else 

  vector<float>   *jet_flavor_weight_JetFitterCOMBNN;
  vector<float>     *jet_flavor_component_jfitcomb_pu;
  vector<float>     *jet_flavor_component_jfitcomb_pb;
  vector<float>     *jet_flavor_component_jfitcomb_pc;

  vector<float>     *jet_flavor_component_jfitc_pu;
  vector<float>     *jet_flavor_component_jfitc_pb;
  vector<float>     *jet_flavor_component_jfitc_pc;
  vector<float>     *jet_flavor_weight_MV3_bVSu;
  vector<float>     *jet_flavor_weight_MV3_bVSc;
  vector<float>     *jet_flavor_weight_MV3_cVSu;


  double sharktopus; 

  SusyBuffer(SmartChain *tree, const unsigned use_branches, 
	     BranchNames names );

private: 
  void set_mc_branches(SmartChain* chain, const unsigned br, 
		       std::string jc); 
};

#endif

