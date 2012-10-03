#ifndef SusyBuffer_cxx
#define SusyBuffer_cxx
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <stdexcept>

#include "SusyBuffer.h"
#include "cutflow.hh"

SusyBuffer::SusyBuffer(SmartChain *fChain, unsigned br, BranchNames names)
{


  jet_AntiKt4LCTopo_MET_Egamma10NoTau_wet = 0;       
  jet_AntiKt4LCTopo_MET_Egamma10NoTau_wpx = 0;       
  jet_AntiKt4LCTopo_MET_Egamma10NoTau_wpy = 0;       
  jet_AntiKt4LCTopo_MET_Egamma10NoTau_statusWord = 0;

  // Set object pointer
  el_author = 0;
  el_OQ = 0;
  el_mediumPP = 0;
  el_cl_E = 0;
  el_cl_eta = 0;
  el_cl_phi = 0;
  el_trackphi = 0;
  el_tracketa = 0;
  el_nPixHits = 0;
  el_nSCTHits = 0;
  mu_staco_E = 0;
  mu_staco_pt = 0;
  mu_staco_eta = 0;
  mu_staco_phi = 0;
  mu_staco_charge = 0;
  mu_staco_isCombinedMuon = 0;
  mu_staco_isSegmentTaggedMuon = 0;
  mu_staco_loose = 0;
  mu_staco_id_theta_exPV = 0;
  mu_staco_id_qoverp_exPV = 0;
  mu_staco_me_theta_exPV = 0;
  mu_staco_me_qoverp_exPV = 0;
  mu_staco_ms_phi = 0;
  mu_staco_ms_theta = 0;
  mu_staco_ms_qoverp = 0;
  mu_staco_id_theta = 0;
  mu_staco_nBLHits = 0;
  mu_staco_nPixHits = 0;
  mu_staco_nSCTHits = 0;
  mu_staco_nTRTHits = 0;
  mu_staco_nPixHoles = 0;
  mu_staco_nSCTHoles = 0;
  mu_staco_nTRTOutliers = 0;
  mu_staco_nPixelDeadSensors = 0;
  mu_staco_nSCTDeadSensors = 0;
  mu_staco_expectBLayerHit = 0;
  jet_AntiKt4TopoNewEM_E = 0;
  jet_AntiKt4TopoNewEM_pt = 0;
  jet_AntiKt4TopoNewEM_eta = 0;
  jet_AntiKt4TopoNewEM_phi = 0;
  jet_AntiKt4TopoNewEM_EtaOrigin = 0;
  jet_AntiKt4TopoNewEM_PhiOrigin = 0;
  jet_AntiKt4TopoNewEM_MOrigin = 0;
  jet_AntiKt4TopoNewEM_Timing = 0;
  jet_AntiKt4TopoNewEM_LArQuality = 0;
  jet_AntiKt4TopoNewEM_sumPtTrk = 0;
  jet_AntiKt4TopoNewEM_HECQuality = 0;
  jet_AntiKt4TopoNewEM_NegativeE = 0;
  jet_AntiKt4TopoNewEM_AverageLArQF = 0;
  jet_AntiKt4TopoNewEM_BCH_CORR_CELL = 0;
  jet_AntiKt4TopoNewEM_BCH_CORR_DOTX = 0;
  jet_AntiKt4TopoNewEM_BCH_CORR_JET = 0;
  jet_AntiKt4TopoNewEM_fracSamplingMax = 0;
  jet_AntiKt4TopoNewEM_hecf = 0;
  jet_AntiKt4TopoNewEM_emfrac = 0;
  jet_AntiKt4TopoNewEM_emscale_E = 0;
  jet_AntiKt4TopoNewEM_emscale_eta = 0;
  jet_AntiKt4TopoNewEM_flavor_weight_JetFitterCOMBNN = 0;
  jet_AntiKt4TopoNewEM_flavor_truth_label = 0;
  jet_AntiKt4TopoNewEM_flavor_component_jfitcomb_pu = 0;
  jet_AntiKt4TopoNewEM_flavor_component_jfitcomb_pb = 0;
  jet_AntiKt4TopoNewEM_flavor_component_jfitcomb_pc = 0;
  vx_nTracks = 0;
  trig_RoI_EF_b_Rec__TrackParticleContainer = 0;
  trig_RoI_EF_b_Rec__TrackParticleContainerStatus = 0;
  // Set branch addresses and branch pointers

  sharktopus = 0; 

  // fChain = chain;
  // fCurrent = -1;

  // // fChain->SetMakeClass(1);

  using namespace branches; 
   
  bool other = others & br; 

  fChain->SetBranchStatus("*",0); 

  fChain->SetBranchStatus("RunNumber", br & run_num);
  fChain->SetBranchAddress("RunNumber", &RunNumber);  

  fChain->SetBranchAddress(names.trigger.c_str(), &trigger, true); 
  fChain->SetBranchAddress("coreFlags", &coreFlags, true); 

  fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_jvtxf", 
			   &jet_AntiKt4TopoNewEM_jvtxf, true); 

  // MET garbage
  fChain->SetBranchAddress("jet_AntiKt4LCTopo_pt", 
			   &jet_AntiKt4LCTopo_pt, true); 
  fChain->SetBranchAddress("jet_AntiKt4LCTopo_MET_Egamma10NoTau_wet", 
			   &jet_AntiKt4LCTopo_MET_Egamma10NoTau_wet, true); 
  fChain->SetBranchAddress("jet_AntiKt4LCTopo_MET_Egamma10NoTau_wpx",
			   &jet_AntiKt4LCTopo_MET_Egamma10NoTau_wpx, true); 
  fChain->SetBranchAddress("jet_AntiKt4LCTopo_MET_Egamma10NoTau_wpy", 
			   &jet_AntiKt4LCTopo_MET_Egamma10NoTau_wpy, true); 
  fChain->SetBranchAddress("jet_AntiKt4LCTopo_MET_Egamma10NoTau_statusWord", 
			   &jet_AntiKt4LCTopo_MET_Egamma10NoTau_statusWord, 
			   true); 

  fChain->SetBranchAddress("el_MET_Egamma10NoTau_wet", 
			   &el_MET_Egamma10NoTau_wet, true); 
  fChain->SetBranchAddress("el_MET_Egamma10NoTau_wpx",
			   &el_MET_Egamma10NoTau_wpx, true); 
  fChain->SetBranchAddress("el_MET_Egamma10NoTau_wpy", 
			   &el_MET_Egamma10NoTau_wpy, true); 
  fChain->SetBranchAddress("el_MET_Egamma10NoTau_statusWord", 
			   &el_MET_Egamma10NoTau_statusWord, 
			   true); 


  fChain->SetBranchAddress("MET_Egamma10NoTau_SoftJets_etx"  ,
			   &MET_Egamma10NoTau_SoftJets_etx, true);
  fChain->SetBranchAddress("MET_Egamma10NoTau_SoftJets_ety"  ,
			   &MET_Egamma10NoTau_SoftJets_ety, true);   
  fChain->SetBranchAddress("MET_Egamma10NoTau_SoftJets_sumet",
			   &MET_Egamma10NoTau_SoftJets_sumet, true);
  fChain->SetBranchAddress("MET_Egamma10NoTau_CellOut_etx"   ,
			   &MET_Egamma10NoTau_CellOut_etx, true); 
  fChain->SetBranchAddress("MET_Egamma10NoTau_CellOut_ety"   ,
			   &MET_Egamma10NoTau_CellOut_ety, true);    
  fChain->SetBranchAddress("MET_Egamma10NoTau_CellOut_sumet" ,
			   &MET_Egamma10NoTau_CellOut_sumet, true);
  fChain->SetBranchAddress("MET_Egamma10NoTau_CellOut_etx"   ,
			   &MET_Egamma10NoTau_CellOut_etx, true);  
  fChain->SetBranchAddress("MET_Egamma10NoTau_CellOut_ety"   ,
			   &MET_Egamma10NoTau_CellOut_ety, true);    
  fChain->SetBranchAddress("MET_Egamma10NoTau_CellOut_sumet" ,
			   &MET_Egamma10NoTau_CellOut_sumet, true);
  fChain->SetBranchAddress("MET_Egamma10NoTau_RefGamma_etx"  ,
			   &MET_Egamma10NoTau_RefGamma_etx, true);  
  fChain->SetBranchAddress("MET_Egamma10NoTau_RefGamma_ety"  ,
			   &MET_Egamma10NoTau_RefGamma_ety, true);   
  fChain->SetBranchAddress("MET_Egamma10NoTau_RefGamma_sumet",
			   &MET_Egamma10NoTau_RefGamma_sumet, true);


 
  fChain->SetBranchStatus("averageIntPerXing",other);
  fChain->SetBranchAddress("averageIntPerXing", &averageIntPerXing); 
  fChain->SetBranchStatus("larError",other);
  fChain->SetBranchAddress("larError", &larError); 
  fChain->SetBranchStatus("el_n",other);
  fChain->SetBranchAddress("el_n", &el_n); 
  fChain->SetBranchStatus("el_author",other);
  fChain->SetBranchAddress("el_author", &el_author); 
  fChain->SetBranchStatus("el_OQ",other);
  fChain->SetBranchAddress("el_OQ", &el_OQ); 
  fChain->SetBranchStatus("el_mediumPP",other);
  fChain->SetBranchAddress("el_mediumPP", &el_mediumPP); 
  fChain->SetBranchStatus("el_cl_E",other);
  fChain->SetBranchAddress("el_cl_E", &el_cl_E); 
  fChain->SetBranchStatus("el_cl_eta",other);
  fChain->SetBranchAddress("el_cl_eta", &el_cl_eta); 
  fChain->SetBranchStatus("el_cl_phi",other);
  fChain->SetBranchAddress("el_cl_phi", &el_cl_phi); 
  fChain->SetBranchAddress("el_cl_pt", &el_cl_pt, true); 
  fChain->SetBranchStatus("el_trackphi",other);
  fChain->SetBranchAddress("el_trackphi", &el_trackphi); 
  fChain->SetBranchStatus("el_tracketa",other);
  fChain->SetBranchAddress("el_tracketa", &el_tracketa); 
  fChain->SetBranchStatus("el_nPixHits",other);
  fChain->SetBranchAddress("el_nPixHits", &el_nPixHits); 
  fChain->SetBranchStatus("el_nSCTHits",other);
  fChain->SetBranchAddress("el_nSCTHits", &el_nSCTHits); 
  fChain->SetBranchStatus("mu_staco_n",other);
  fChain->SetBranchAddress("mu_staco_n", &mu_staco_n); 
  fChain->SetBranchStatus("mu_staco_E",other);
  fChain->SetBranchAddress("mu_staco_E", &mu_staco_E); 
  fChain->SetBranchStatus("mu_staco_pt",other);
  fChain->SetBranchAddress("mu_staco_pt", &mu_staco_pt); 
  fChain->SetBranchStatus("mu_staco_eta",other);
  fChain->SetBranchAddress("mu_staco_eta", &mu_staco_eta); 
  fChain->SetBranchStatus("mu_staco_phi",other);
  fChain->SetBranchAddress("mu_staco_phi", &mu_staco_phi); 
  fChain->SetBranchStatus("mu_staco_charge",other);
  fChain->SetBranchAddress("mu_staco_charge", &mu_staco_charge); 
  fChain->SetBranchStatus("mu_staco_isCombinedMuon",other);
  fChain->SetBranchAddress("mu_staco_isCombinedMuon", &mu_staco_isCombinedMuon); 
  fChain->SetBranchStatus("mu_staco_isSegmentTaggedMuon",other);
  fChain->SetBranchAddress("mu_staco_isSegmentTaggedMuon", &mu_staco_isSegmentTaggedMuon); 
  fChain->SetBranchStatus("mu_staco_loose",other);
  fChain->SetBranchAddress("mu_staco_loose", &mu_staco_loose); 
  fChain->SetBranchStatus("mu_staco_id_theta_exPV",other);
  fChain->SetBranchAddress("mu_staco_id_theta_exPV", &mu_staco_id_theta_exPV); 
  fChain->SetBranchStatus("mu_staco_id_qoverp_exPV",other);
  fChain->SetBranchAddress("mu_staco_id_qoverp_exPV", &mu_staco_id_qoverp_exPV); 
  fChain->SetBranchStatus("mu_staco_me_theta_exPV",other);
  fChain->SetBranchAddress("mu_staco_me_theta_exPV", &mu_staco_me_theta_exPV); 
  fChain->SetBranchStatus("mu_staco_me_qoverp_exPV",other);
  fChain->SetBranchAddress("mu_staco_me_qoverp_exPV", &mu_staco_me_qoverp_exPV); 
  fChain->SetBranchStatus("mu_staco_ms_phi",other);
  fChain->SetBranchAddress("mu_staco_ms_phi", &mu_staco_ms_phi); 
  fChain->SetBranchStatus("mu_staco_ms_theta",other);
  fChain->SetBranchAddress("mu_staco_ms_theta", &mu_staco_ms_theta); 
  fChain->SetBranchStatus("mu_staco_ms_qoverp",other);
  fChain->SetBranchAddress("mu_staco_ms_qoverp", &mu_staco_ms_qoverp); 
  fChain->SetBranchStatus("mu_staco_id_theta",other);
  fChain->SetBranchAddress("mu_staco_id_theta", &mu_staco_id_theta); 
  fChain->SetBranchStatus("mu_staco_nBLHits",other);
  fChain->SetBranchAddress("mu_staco_nBLHits", &mu_staco_nBLHits); 
  fChain->SetBranchStatus("mu_staco_nPixHits",other);
  fChain->SetBranchAddress("mu_staco_nPixHits", &mu_staco_nPixHits); 
  fChain->SetBranchStatus("mu_staco_nSCTHits",other);
  fChain->SetBranchAddress("mu_staco_nSCTHits", &mu_staco_nSCTHits); 
  fChain->SetBranchStatus("mu_staco_nTRTHits",other);
  fChain->SetBranchAddress("mu_staco_nTRTHits", &mu_staco_nTRTHits); 
  fChain->SetBranchStatus("mu_staco_nPixHoles",other);
  fChain->SetBranchAddress("mu_staco_nPixHoles", &mu_staco_nPixHoles); 
  fChain->SetBranchStatus("mu_staco_nSCTHoles",other);
  fChain->SetBranchAddress("mu_staco_nSCTHoles", &mu_staco_nSCTHoles); 
  fChain->SetBranchStatus("mu_staco_nTRTOutliers",other);
  fChain->SetBranchAddress("mu_staco_nTRTOutliers", &mu_staco_nTRTOutliers); 
  fChain->SetBranchStatus("mu_staco_nPixelDeadSensors",other);
  fChain->SetBranchAddress("mu_staco_nPixelDeadSensors", &mu_staco_nPixelDeadSensors); 
  fChain->SetBranchStatus("mu_staco_nSCTDeadSensors",other);
  fChain->SetBranchAddress("mu_staco_nSCTDeadSensors", &mu_staco_nSCTDeadSensors); 
  fChain->SetBranchStatus("mu_staco_expectBLayerHit",other);
  fChain->SetBranchAddress("mu_staco_expectBLayerHit", &mu_staco_expectBLayerHit); 
  fChain->SetBranchStatus("jet_AntiKt4TopoNewEM_n",other);
  fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_n", &jet_AntiKt4TopoNewEM_n); 
  fChain->SetBranchStatus("jet_AntiKt4TopoNewEM_E",other);
  fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_E", &jet_AntiKt4TopoNewEM_E); 
  fChain->SetBranchStatus("jet_AntiKt4TopoNewEM_pt",other);
  fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_pt", &jet_AntiKt4TopoNewEM_pt); 
  fChain->SetBranchStatus("jet_AntiKt4TopoNewEM_eta",other);
  fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_eta", &jet_AntiKt4TopoNewEM_eta); 
  fChain->SetBranchStatus("jet_AntiKt4TopoNewEM_phi",other);
  fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_phi", &jet_AntiKt4TopoNewEM_phi); 
  fChain->SetBranchStatus("jet_AntiKt4TopoNewEM_EtaOrigin",other);
  fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_EtaOrigin", &jet_AntiKt4TopoNewEM_EtaOrigin); 
  fChain->SetBranchStatus("jet_AntiKt4TopoNewEM_PhiOrigin",other);
  fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_PhiOrigin", &jet_AntiKt4TopoNewEM_PhiOrigin); 
  fChain->SetBranchStatus("jet_AntiKt4TopoNewEM_MOrigin",other);
  fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_MOrigin", &jet_AntiKt4TopoNewEM_MOrigin); 
  fChain->SetBranchStatus("jet_AntiKt4TopoNewEM_Timing",other);
  fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_Timing", &jet_AntiKt4TopoNewEM_Timing); 
  fChain->SetBranchStatus("jet_AntiKt4TopoNewEM_LArQuality",other);
  fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_LArQuality", &jet_AntiKt4TopoNewEM_LArQuality); 
  fChain->SetBranchStatus("jet_AntiKt4TopoNewEM_sumPtTrk",other);
  fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_sumPtTrk", &jet_AntiKt4TopoNewEM_sumPtTrk); 
  fChain->SetBranchStatus("jet_AntiKt4TopoNewEM_HECQuality",other);
  fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_HECQuality", &jet_AntiKt4TopoNewEM_HECQuality); 
  fChain->SetBranchStatus("jet_AntiKt4TopoNewEM_NegativeE",other);
  fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_NegativeE", &jet_AntiKt4TopoNewEM_NegativeE); 
  fChain->SetBranchStatus("jet_AntiKt4TopoNewEM_AverageLArQF",other);
  fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_AverageLArQF", &jet_AntiKt4TopoNewEM_AverageLArQF); 
  fChain->SetBranchStatus("jet_AntiKt4TopoNewEM_BCH_CORR_CELL",other);
  fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_BCH_CORR_CELL", &jet_AntiKt4TopoNewEM_BCH_CORR_CELL); 
  fChain->SetBranchStatus("jet_AntiKt4TopoNewEM_BCH_CORR_DOTX",other);
  fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_BCH_CORR_DOTX", &jet_AntiKt4TopoNewEM_BCH_CORR_DOTX); 
  fChain->SetBranchStatus("jet_AntiKt4TopoNewEM_BCH_CORR_JET",other);
  fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_BCH_CORR_JET", &jet_AntiKt4TopoNewEM_BCH_CORR_JET); 
  fChain->SetBranchStatus("jet_AntiKt4TopoNewEM_fracSamplingMax",other);
  fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_SamplingMax", 
			   &jet_AntiKt4TopoNewEM_SamplingMax, true); 
  fChain->SetBranchStatus("jet_AntiKt4TopoNewEM_hecf",other);
  fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_hecf", &jet_AntiKt4TopoNewEM_hecf); 
  fChain->SetBranchStatus("jet_AntiKt4TopoNewEM_emfrac",other);
  fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_emfrac", &jet_AntiKt4TopoNewEM_emfrac); 
  fChain->SetBranchStatus("jet_AntiKt4TopoNewEM_emscale_E",other);
  fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_emscale_E", &jet_AntiKt4TopoNewEM_emscale_E); 
  fChain->SetBranchStatus("jet_AntiKt4TopoNewEM_emscale_eta",other);
  fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_emscale_eta", &jet_AntiKt4TopoNewEM_emscale_eta); 
  fChain->SetBranchStatus("jet_AntiKt4TopoNewEM_flavor_weight_JetFitterCOMBNN",other);
  fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_flavor_weight_JetFitterCOMBNN", &jet_AntiKt4TopoNewEM_flavor_weight_JetFitterCOMBNN); 
  fChain->SetBranchStatus("jet_AntiKt4TopoNewEM_flavor_truth_label",other);
  fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_flavor_truth_label", &jet_AntiKt4TopoNewEM_flavor_truth_label); 
  fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_flavor_component_jfitcomb_pu", &jet_AntiKt4TopoNewEM_flavor_component_jfitcomb_pu, other);
  fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_flavor_component_jfitcomb_pb", &jet_AntiKt4TopoNewEM_flavor_component_jfitcomb_pb, other);
  fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_flavor_component_jfitcomb_pc", &jet_AntiKt4TopoNewEM_flavor_component_jfitcomb_pc, other);
  fChain->SetBranchStatus("vx_nTracks",other);
  fChain->SetBranchAddress("vx_nTracks", &vx_nTracks); 

  if (br & unleash_sharktopus){  
    fChain->SetBranchStatus("sharktopus", 1); 
    fChain->SetBranchAddress("sharktopus",&sharktopus); 
  }

}

#endif 

