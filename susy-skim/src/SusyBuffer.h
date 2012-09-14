//////////////////////// -*-c++-*-//////////////////////////
// This class has been automatically generated on
// Wed Jun 20 17:06:57 2012 by ROOT version 5.30/00
// from TTree susy/susy
// found on file: file1.root
//////////////////////////////////////////////////////////

#ifndef susy_h
#define susy_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <vector> 


namespace branches { 
  const unsigned trigger = 1u << 0; 
  const unsigned run_num = 1u << 1; 
  const unsigned others  = 1u << 5; 
  const unsigned all     = trigger | others; 
}


using std::vector; 

class SmartChain; 

class SusyBuffer {
public :
  
  // TChain          *fChain;   //!pointer to the analyzed TTree or TChain
  // Int_t           fCurrent; //!current Tree number in a TChain

  // Declaration of leaf types
  int top_hfor_type; 
  unsigned int RunNumber; 
  Bool_t          EF_xe60_verytight_noMu;
  Bool_t          EF_xe70_noMu;
  Float_t         averageIntPerXing;
  UInt_t          mc_channel_number;
  UInt_t          larError;
  Int_t           el_n;
  vector<int>     *el_author;
  vector<unsigned int> *el_OQ;
  vector<int>     *el_nConv;
  vector<int>     *el_nSingleTrackConv;
  vector<int>     *el_nDoubleTrackConv;
  vector<unsigned int> *el_OQRecalc;
  vector<int>     *el_mediumPP;
  vector<int>     *el_mediumPPIso;
  vector<float>   *el_weta2;
  vector<float>   *el_nucone20;
  vector<float>   *el_nucone30;
  vector<float>   *el_nucone40;
  vector<float>   *el_cl_E;
  vector<float>   *el_cl_eta;
  vector<float>   *el_cl_phi;
  vector<float>   *el_trackphi;
  vector<float>   *el_tracketa;
  vector<int>     *el_nBLHits;
  vector<int>     *el_nPixHits;
  vector<int>     *el_nSCTHits;
  vector<int>     *el_nTRTHits;
  vector<int>     *el_nTRTHighTHits;
  vector<int>     *el_nPixHoles;
  vector<int>     *el_nSCTHoles;
  vector<int>     *el_nTRTHoles;
  vector<int>     *el_nBLSharedHits;
  vector<int>     *el_nPixSharedHits;
  vector<int>     *el_nSCTSharedHits;
  vector<int>     *el_nBLayerOutliers;
  vector<int>     *el_nPixelOutliers;
  vector<int>     *el_nSCTOutliers;
  vector<int>     *el_nTRTOutliers;
  vector<int>     *el_nTRTHighTOutliers;
  vector<int>     *el_nSiHits;
  vector<int>     *el_nGoodHitsPixeldEdx;
  Int_t           mu_staco_n;
  vector<float>   *mu_staco_E;
  vector<float>   *mu_staco_pt;
  vector<float>   *mu_staco_eta;
  vector<float>   *mu_staco_phi;
  vector<float>   *mu_staco_charge;
  vector<float>   *mu_staco_nucone20;
  vector<float>   *mu_staco_nucone30;
  vector<float>   *mu_staco_nucone40;
  vector<float>   *mu_staco_ptcone20;
  vector<float>   *mu_staco_ptcone30;
  vector<float>   *mu_staco_ptcone40;
  vector<int>     *mu_staco_isCombinedMuon;
  vector<int>     *mu_staco_isSegmentTaggedMuon;
  vector<int>     *mu_staco_loose;
  vector<float>   *mu_staco_phi_exPV;
  vector<float>   *mu_staco_id_theta_exPV;
  vector<float>   *mu_staco_id_qoverp_exPV;
  vector<float>   *mu_staco_me_theta_exPV;
  vector<float>   *mu_staco_me_qoverp_exPV;
  vector<float>   *mu_staco_ms_phi;
  vector<float>   *mu_staco_ms_theta;
  vector<float>   *mu_staco_ms_qoverp;
  vector<float>   *mu_staco_id_theta;
  vector<int>     *mu_staco_nOutliersOnTrack;
  vector<int>     *mu_staco_nBLHits;
  vector<int>     *mu_staco_nPixHits;
  vector<int>     *mu_staco_nSCTHits;
  vector<int>     *mu_staco_nTRTHits;
  vector<int>     *mu_staco_nTRTHighTHits;
  vector<int>     *mu_staco_nBLSharedHits;
  vector<int>     *mu_staco_nPixSharedHits;
  vector<int>     *mu_staco_nPixHoles;
  vector<int>     *mu_staco_nSCTSharedHits;
  vector<int>     *mu_staco_nSCTHoles;
  vector<int>     *mu_staco_nTRTOutliers;
  vector<int>     *mu_staco_nTRTHighTOutliers;
  vector<int>     *mu_staco_nGangedPixels;
  vector<int>     *mu_staco_nPixelDeadSensors;
  vector<int>     *mu_staco_nSCTDeadSensors;
  vector<int>     *mu_staco_nTRTDeadStraws;
  vector<int>     *mu_staco_expectBLayerHit;
  vector<int>     *mu_staco_nMDTHits;
  vector<int>     *mu_staco_nMDTHoles;
  vector<int>     *mu_staco_nCSCEtaHits;
  vector<int>     *mu_staco_nCSCEtaHoles;
  vector<int>     *mu_staco_nCSCPhiHits;
  vector<int>     *mu_staco_nCSCPhiHoles;
  vector<int>     *mu_staco_nRPCEtaHits;
  vector<int>     *mu_staco_nRPCEtaHoles;
  vector<int>     *mu_staco_nRPCPhiHits;
  vector<int>     *mu_staco_nRPCPhiHoles;
  vector<int>     *mu_staco_nTGCEtaHits;
  vector<int>     *mu_staco_nTGCEtaHoles;
  vector<int>     *mu_staco_nTGCPhiHits;
  vector<int>     *mu_staco_nTGCPhiHoles;
  vector<int>     *mu_staco_nMDTBIHits;
  vector<int>     *mu_staco_nMDTBMHits;
  vector<int>     *mu_staco_nMDTBOHits;
  vector<int>     *mu_staco_nMDTBEEHits;
  vector<int>     *mu_staco_nMDTBIS78Hits;
  vector<int>     *mu_staco_nMDTEIHits;
  vector<int>     *mu_staco_nMDTEMHits;
  vector<int>     *mu_staco_nMDTEOHits;
  vector<int>     *mu_staco_nMDTEEHits;
  vector<int>     *mu_staco_nRPCLayer1EtaHits;
  vector<int>     *mu_staco_nRPCLayer2EtaHits;
  vector<int>     *mu_staco_nRPCLayer3EtaHits;
  vector<int>     *mu_staco_nRPCLayer1PhiHits;
  vector<int>     *mu_staco_nRPCLayer2PhiHits;
  vector<int>     *mu_staco_nRPCLayer3PhiHits;
  vector<int>     *mu_staco_nTGCLayer1EtaHits;
  vector<int>     *mu_staco_nTGCLayer2EtaHits;
  vector<int>     *mu_staco_nTGCLayer3EtaHits;
  vector<int>     *mu_staco_nTGCLayer4EtaHits;
  vector<int>     *mu_staco_nTGCLayer1PhiHits;
  vector<int>     *mu_staco_nTGCLayer2PhiHits;
  vector<int>     *mu_staco_nTGCLayer3PhiHits;
  vector<int>     *mu_staco_nTGCLayer4PhiHits;
  vector<float>   *mu_staco_EFCB_dr;
  vector<int>     *mu_staco_EFCB_index;
  vector<float>   *mu_staco_EFMG_dr;
  vector<int>     *mu_staco_EFMG_index;
  vector<float>   *mu_staco_EFME_dr;
  vector<int>     *mu_staco_EFME_index;
  Float_t         MET_SoftJets_etx;
  Float_t         MET_SoftJets_ety;
  Float_t         MET_SoftJets_sumet;
  Float_t         MET_CellOut_etx;
  Float_t         MET_CellOut_ety;
  Float_t         MET_CellOut_sumet;
  Float_t         MET_CellOut_etx_CentralReg;
  Float_t         MET_CellOut_ety_CentralReg;
  Float_t         MET_CellOut_sumet_CentralReg;
  Float_t         MET_CellOut_etx_EndcapRegion;
  Float_t         MET_CellOut_ety_EndcapRegion;
  Float_t         MET_CellOut_sumet_EndcapRegion;
  Float_t         MET_CellOut_etx_ForwardReg;
  Float_t         MET_CellOut_ety_ForwardReg;
  Float_t         MET_CellOut_sumet_ForwardReg;
  Float_t         MET_RefGamma_em_etx;
  Float_t         MET_RefGamma_em_ety;
  Float_t         MET_RefGamma_em_sumet;
  Float_t         MET_CellOut_Eflow_etx;
  Float_t         MET_CellOut_Eflow_ety;
  Float_t         MET_CellOut_Eflow_sumet;
  Float_t         MET_Simplified20_RefFinal_etx;
  Float_t         MET_Simplified20_RefFinal_ety;
  Float_t         MET_Simplified20_RefFinal_etx_CentralReg;
  Float_t         MET_Simplified20_RefFinal_ety_CentralReg;
  Float_t         MET_Simplified20_RefFinal_etx_EndcapRegion;
  Float_t         MET_Simplified20_RefFinal_ety_EndcapRegion;
  Float_t         MET_Simplified20_RefFinal_etx_ForwardReg;
  Float_t         MET_Simplified20_RefFinal_ety_ForwardReg;
  vector<vector<float> > *el_MET_Simplified20_wpx;
  vector<vector<float> > *el_MET_Simplified20_wpy;
  vector<vector<float> > *el_MET_Simplified20_wet;
  vector<vector<unsigned int> > *el_MET_Simplified20_statusWord;
  vector<vector<float> > *jet_AntiKt4TopoNewEM_MET_Simplified20_wpx;
  vector<vector<float> > *jet_AntiKt4TopoNewEM_MET_Simplified20_wpy;
  vector<vector<float> > *jet_AntiKt4TopoNewEM_MET_Simplified20_wet;
  vector<vector<unsigned int> > *jet_AntiKt4TopoNewEM_MET_Simplified20_statusWord;
  Int_t           jet_AntiKt4TopoNewEM_n;
  vector<float>   *jet_AntiKt4TopoNewEM_E;
  vector<float>   *jet_AntiKt4TopoNewEM_pt;
  vector<float>   *jet_AntiKt4TopoNewEM_eta;
  vector<float>   *jet_AntiKt4TopoNewEM_phi;
  vector<float>   *jet_AntiKt4TopoNewEM_EtaOrigin;
  vector<float>   *jet_AntiKt4TopoNewEM_PhiOrigin;
  vector<float>   *jet_AntiKt4TopoNewEM_MOrigin;
  vector<float>   *jet_AntiKt4TopoNewEM_EtaOriginEM;
  vector<float>   *jet_AntiKt4TopoNewEM_PhiOriginEM;
  vector<float>   *jet_AntiKt4TopoNewEM_MOriginEM;
  vector<float>   *jet_AntiKt4TopoNewEM_n90;
  vector<float>   *jet_AntiKt4TopoNewEM_Timing;
  vector<float>   *jet_AntiKt4TopoNewEM_LArQuality;
  vector<float>   *jet_AntiKt4TopoNewEM_nTrk;
  vector<float>   *jet_AntiKt4TopoNewEM_sumPtTrk;
  vector<float>   *jet_AntiKt4TopoNewEM_HECQuality;
  vector<float>   *jet_AntiKt4TopoNewEM_NegativeE;
  vector<float>   *jet_AntiKt4TopoNewEM_AverageLArQF;
  vector<float>   *jet_AntiKt4TopoNewEM_BCH_CORR_CELL;
  vector<float>   *jet_AntiKt4TopoNewEM_BCH_CORR_DOTX;
  vector<float>   *jet_AntiKt4TopoNewEM_BCH_CORR_JET;
  vector<float>   *jet_AntiKt4TopoNewEM_BCH_CORR_JET_FORCELL;
  vector<float>   *jet_AntiKt4TopoNewEM_ENG_BAD_CELLS;
  vector<float>   *jet_AntiKt4TopoNewEM_fracSamplingMax;
  vector<float>   *jet_AntiKt4TopoNewEM_hecf;
  vector<float>   *jet_AntiKt4TopoNewEM_emfrac;
  vector<float>   *jet_AntiKt4TopoNewEM_EMJES;
  vector<float>   *jet_AntiKt4TopoNewEM_EMJES_EtaCorr;
  vector<float>   *jet_AntiKt4TopoNewEM_EMJESnooffset;
  vector<float>   *jet_AntiKt4TopoNewEM_emscale_E;
  vector<float>   *jet_AntiKt4TopoNewEM_emscale_eta;
  vector<vector<float> > *jet_AntiKt4TopoNewEM_ptconst_default;
  vector<vector<float> > *jet_AntiKt4TopoNewEM_etaconst_default;
  vector<vector<float> > *jet_AntiKt4TopoNewEM_phiconst_default;
  vector<float>   *jet_AntiKt4TopoNewEM_flavor_weight_JetFitterCOMBNN;
  vector<int>     *jet_AntiKt4TopoNewEM_flavor_truth_label;
  vector<float>   *jet_AntiKt4TopoNewEM_EF_dr;
  vector<int>     *jet_AntiKt4TopoNewEM_EF_matched;
  vector<int>     *jet_AntiKt4TopoNewEM_flavor_component_jfitcomb_pu;
  vector<int>     *jet_AntiKt4TopoNewEM_flavor_component_jfitcomb_pb;
  vector<int>     *jet_AntiKt4TopoNewEM_flavor_component_jfitcomb_pc;
  vector<int>     *vx_nTracks;
  Int_t           trig_L2_el_n;
  Int_t           trig_EF_el_n;
  vector<int>     *trig_EF_el_author;
  vector<int>     *trig_EF_el_mediumPP;
  vector<int>     *trig_EF_el_mediumPPIso;
  vector<vector<int> > *trig_RoI_EF_b_Rec__TrackParticleContainer;
  vector<vector<int> > *trig_RoI_EF_b_Rec__TrackParticleContainerStatus;

  double sharktopus; 

  SusyBuffer(SmartChain *tree, unsigned use_branches = branches::all);
  // virtual ~SusyBuffer();
  // virtual Int_t    Cut(Long64_t entry);
  // virtual Int_t    GetEntry(Long64_t entry);
  // virtual Long64_t LoadTree(Long64_t entry);
  // virtual void     Init(TChain *tree, const unsigned use_br = branches::all);
  // virtual void     Loop();
  // virtual Bool_t   Notify();
  // virtual void     Show(Long64_t entry = -1);
};

#endif

