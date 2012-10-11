//////////////////////// -*-c++-*-//////////////////////////
// This class has been automatically generated on
// Wed Jun 20 17:06:57 2012 by ROOT version 5.30/00
// from TTree susy/susy
// found on file: file1.root
//////////////////////////////////////////////////////////

#ifndef susy_h
#define susy_h

#include <vector> 


namespace branches { 
  const unsigned trigger = 1u << 0; 
  const unsigned run_num = 1u << 1; 
  const unsigned others  = 1u << 5; 
  const unsigned all     = trigger | others | run_num; 
  const unsigned unleash_sharktopus  = 1u << 6; 
}

struct BranchNames 
{ 
  std::string trigger; 
}; 

using std::vector; 

class SmartChain; 

class SusyBuffer {
public :
  
  // TChain          *fChain;   //!pointer to the analyzed TTree or TChain
  // Int_t           fCurrent; //!current Tree number in a TChain

  // Declaration of leaf types
  unsigned int RunNumber; 
  Bool_t          trigger; 
  Float_t         averageIntPerXing;
  UInt_t          larError;
  unsigned        coreFlags; 

  // MET things
  vector<float>*          jet_pt;
  vector<vector<float> >*    jet_MET_Egamma10NoTau_wet;
  vector<vector<float> >*    jet_MET_Egamma10NoTau_wpx;
  vector<vector<float> >*    jet_MET_Egamma10NoTau_wpy;
  vector<vector<unsigned> >* jet_MET_Egamma10NoTau_statusWord;

  vector<vector<float> >*    el_MET_Egamma10NoTau_wet;
  vector<vector<float> >*    el_MET_Egamma10NoTau_wpx;
  vector<vector<float> >*    el_MET_Egamma10NoTau_wpy;
  vector<vector<unsigned> >* el_MET_Egamma10NoTau_statusWord;

  float MET_Egamma10NoTau_SoftJets_etx;
  float MET_Egamma10NoTau_SoftJets_ety;
  float MET_Egamma10NoTau_SoftJets_sumet;                                
  float MET_Egamma10NoTau_CellOut_etx; //CellOut
  float MET_Egamma10NoTau_CellOut_ety; //CellOut
  float MET_Egamma10NoTau_CellOut_sumet; //CellOut
  float MET_Egamma10NoTau_RefGamma_etx;
  float MET_Egamma10NoTau_RefGamma_ety;
  float MET_Egamma10NoTau_RefGamma_sumet;

  
  vector<float>   *jet_jvtxf; 
  Int_t           el_n;
  vector<int>     *el_author;
  vector<unsigned int> *el_OQ;
  // vector<unsigned int> *el_OQRecalc;
  vector<int>     *el_mediumPP;
  vector<float>   *el_cl_E;
  vector<float>   *el_cl_eta;
  vector<float>   *el_cl_phi;
  vector<float>   *el_cl_pt;
  vector<float>   *el_trackphi;
  vector<float>   *el_tracketa;
  vector<int>     *el_nPixHits;
  vector<int>     *el_nSCTHits;
  Int_t           mu_staco_n;
  vector<float>   *mu_staco_E;
  vector<float>   *mu_staco_pt;
  vector<float>   *mu_staco_eta;
  vector<float>   *mu_staco_phi;
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
  Int_t           jet_n;
  vector<float>   *jet_E;
  vector<float>   *jet_eta;
  vector<float>   *jet_phi;
  vector<float>   *jet_EtaOrigin;
  vector<float>   *jet_PhiOrigin;
  vector<float>   *jet_MOrigin;
  vector<float>   *jet_Timing;
  vector<float>   *jet_LArQuality;
  vector<float>   *jet_sumPtTrk;
  vector<float>   *jet_HECQuality;
  vector<float>   *jet_NegativeE;
  vector<float>   *jet_AverageLArQF;
  vector<float>   *jet_BCH_CORR_CELL;
  vector<float>   *jet_BCH_CORR_DOTX;
  vector<float>   *jet_BCH_CORR_JET;
  vector<float>   *jet_fracSamplingMax;
  vector<int>     *jet_SamplingMax;
  vector<float>   *jet_hecf;
  vector<float>   *jet_emfrac;
  vector<float>   *jet_emscale_E;
  vector<float>   *jet_emscale_eta;
  vector<float>   *jet_constscale_E;
  vector<float>   *jet_constscale_eta;
  vector<float>   *jet_flavor_weight_JetFitterCOMBNN;
  vector<int>     *jet_flavor_truth_label;
  vector<float>     *jet_flavor_component_jfitcomb_pu;
  vector<float>     *jet_flavor_component_jfitcomb_pb;
  vector<float>     *jet_flavor_component_jfitcomb_pc;
  vector<int>     *vx_nTracks;
  vector<vector<int> > *trig_RoI_EF_b_Rec__TrackParticleContainer;
  vector<vector<int> > *trig_RoI_EF_b_Rec__TrackParticleContainerStatus;

  double sharktopus; 

  SusyBuffer(SmartChain *tree, unsigned use_branches, 
	     BranchNames names );
};

#endif

