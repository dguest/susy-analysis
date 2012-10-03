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

  // fChain->SetMakeClass(1);

  using namespace branches; 
   
  bool other = others & br; 

  std::string jc = "AntiKt4LCTopo"; 

  fChain->SetBranchStatus("*",0); 

  fChain->SetBranchAddress("RunNumber", &RunNumber, true);  

  fChain->SetBranchAddress(names.trigger.c_str(), &trigger, true); 
  fChain->SetBranchAddress("coreFlags", &coreFlags, true); 

  fChain->SetBranchAddress("jet_" + jc + "_jvtxf", 
			   &jet_jvtxf, true); 

  // MET garbage
  fChain->SetBranchAddress("jet_" + jc + "_MET_Egamma10NoTau_wet", 
			   &jet_MET_Egamma10NoTau_wet, true); 
  fChain->SetBranchAddress("jet_" + jc + "_MET_Egamma10NoTau_wpx",
			   &jet_MET_Egamma10NoTau_wpx, true); 
  fChain->SetBranchAddress("jet_" + jc + "_MET_Egamma10NoTau_wpy", 
			   &jet_MET_Egamma10NoTau_wpy, true); 
  fChain->SetBranchAddress("jet_" + jc + "_MET_Egamma10NoTau_statusWord", 
			   &jet_MET_Egamma10NoTau_statusWord, 
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

 
  fChain->SetBranchAddress("averageIntPerXing", &averageIntPerXing, true); 
  fChain->SetBranchAddress("larError", &larError, true); 
  fChain->SetBranchAddress("el_n", &el_n, true); 
  fChain->SetBranchAddress("el_author", &el_author, true); 
  fChain->SetBranchAddress("el_OQ", &el_OQ, true); 
  fChain->SetBranchAddress("el_mediumPP", &el_mediumPP, true); 
  fChain->SetBranchAddress("el_cl_E", &el_cl_E, true); 
  fChain->SetBranchAddress("el_cl_eta", &el_cl_eta, true); 
  fChain->SetBranchAddress("el_cl_phi", &el_cl_phi, true); 
  fChain->SetBranchAddress("el_cl_pt", &el_cl_pt, true); 
  fChain->SetBranchAddress("el_trackphi", &el_trackphi, true); 
  fChain->SetBranchAddress("el_tracketa", &el_tracketa, true); 
  fChain->SetBranchAddress("el_nPixHits", &el_nPixHits, true); 
  fChain->SetBranchAddress("el_nSCTHits", &el_nSCTHits, true); 
  fChain->SetBranchAddress("mu_staco_n", &mu_staco_n, true); 
  fChain->SetBranchAddress("mu_staco_E", &mu_staco_E, true); 
  fChain->SetBranchAddress("mu_staco_pt", &mu_staco_pt, true); 
  fChain->SetBranchAddress("mu_staco_eta", &mu_staco_eta, true); 
  fChain->SetBranchAddress("mu_staco_phi", &mu_staco_phi, true); 
  fChain->SetBranchAddress("mu_staco_charge", &mu_staco_charge, true); 
  fChain->SetBranchAddress("mu_staco_isCombinedMuon", &mu_staco_isCombinedMuon, true); 
  fChain->SetBranchAddress("mu_staco_isSegmentTaggedMuon", &mu_staco_isSegmentTaggedMuon, true); 
  fChain->SetBranchAddress("mu_staco_loose", &mu_staco_loose, true); 
  fChain->SetBranchAddress("mu_staco_id_theta_exPV", &mu_staco_id_theta_exPV, true); 
  fChain->SetBranchAddress("mu_staco_id_qoverp_exPV", &mu_staco_id_qoverp_exPV, true); 
  fChain->SetBranchAddress("mu_staco_me_theta_exPV", &mu_staco_me_theta_exPV, true); 
  fChain->SetBranchAddress("mu_staco_me_qoverp_exPV", &mu_staco_me_qoverp_exPV, true); 
  fChain->SetBranchAddress("mu_staco_ms_phi", &mu_staco_ms_phi, true); 
  fChain->SetBranchAddress("mu_staco_ms_theta", &mu_staco_ms_theta, true); 
  fChain->SetBranchAddress("mu_staco_ms_qoverp", &mu_staco_ms_qoverp, true); 
  fChain->SetBranchAddress("mu_staco_id_theta", &mu_staco_id_theta, true); 
  fChain->SetBranchAddress("mu_staco_nBLHits", &mu_staco_nBLHits, true); 
  fChain->SetBranchAddress("mu_staco_nPixHits", &mu_staco_nPixHits, true); 
  fChain->SetBranchAddress("mu_staco_nSCTHits", &mu_staco_nSCTHits, true); 
  fChain->SetBranchAddress("mu_staco_nTRTHits", &mu_staco_nTRTHits, true); 
  fChain->SetBranchAddress("mu_staco_nPixHoles", &mu_staco_nPixHoles, true); 
  fChain->SetBranchAddress("mu_staco_nSCTHoles", &mu_staco_nSCTHoles, true); 
  fChain->SetBranchAddress("mu_staco_nTRTOutliers", &mu_staco_nTRTOutliers, true); 
  fChain->SetBranchAddress("mu_staco_nPixelDeadSensors", &mu_staco_nPixelDeadSensors, true); 
  fChain->SetBranchAddress("mu_staco_nSCTDeadSensors", &mu_staco_nSCTDeadSensors, true); 
  fChain->SetBranchAddress("mu_staco_expectBLayerHit", &mu_staco_expectBLayerHit, true); 
  fChain->SetBranchAddress("jet_" + jc + "_n", &jet_n, true); 
  fChain->SetBranchAddress("jet_" + jc + "_E", &jet_E, true); 
  fChain->SetBranchAddress("jet_" + jc + "_pt", &jet_pt, true); 
  fChain->SetBranchAddress("jet_" + jc + "_eta", &jet_eta, true); 
  fChain->SetBranchAddress("jet_" + jc + "_phi", &jet_phi, true); 
  fChain->SetBranchAddress("jet_" + jc + "_EtaOrigin", &jet_EtaOrigin, true); 
  fChain->SetBranchAddress("jet_" + jc + "_PhiOrigin", &jet_PhiOrigin, true); 
  fChain->SetBranchAddress("jet_" + jc + "_MOrigin", &jet_MOrigin, true); 
  fChain->SetBranchAddress("jet_" + jc + "_Timing", &jet_Timing, true); 
  fChain->SetBranchAddress("jet_" + jc + "_LArQuality", &jet_LArQuality, true); 
  fChain->SetBranchAddress("jet_" + jc + "_sumPtTrk", &jet_sumPtTrk, true); 
  fChain->SetBranchAddress("jet_" + jc + "_HECQuality", &jet_HECQuality, true); 
  fChain->SetBranchAddress("jet_" + jc + "_NegativeE", &jet_NegativeE, true); 
  fChain->SetBranchAddress("jet_" + jc + "_AverageLArQF", &jet_AverageLArQF, true); 
  fChain->SetBranchAddress("jet_" + jc + "_BCH_CORR_CELL", &jet_BCH_CORR_CELL, true); 
  fChain->SetBranchAddress("jet_" + jc + "_BCH_CORR_DOTX", &jet_BCH_CORR_DOTX, true); 
  fChain->SetBranchAddress("jet_" + jc + "_BCH_CORR_JET", &jet_BCH_CORR_JET, true); 
  fChain->SetBranchAddress("jet_" + jc + "_SamplingMax", 
			   &jet_SamplingMax, true); 
  fChain->SetBranchAddress("jet_" + jc + "_fracSamplingMax", 
			   &jet_fracSamplingMax, true); 
  fChain->SetBranchAddress("jet_" + jc + "_hecf", &jet_hecf, true); 
  fChain->SetBranchAddress("jet_" + jc + "_emfrac", &jet_emfrac, true); 
  fChain->SetBranchAddress("jet_" + jc + "_emscale_E", &jet_emscale_E, true); 
  fChain->SetBranchAddress("jet_" + jc + "_emscale_eta", &jet_emscale_eta, true); 
  fChain->SetBranchAddress("jet_" + jc + "_flavor_weight_JetFitterCOMBNN", &jet_flavor_weight_JetFitterCOMBNN, true); 
  fChain->SetBranchAddress("jet_" + jc + "_flavor_truth_label", &jet_flavor_truth_label, true); 
  fChain->SetBranchAddress("jet_" + jc + "_flavor_component_jfitcomb_pu", &jet_flavor_component_jfitcomb_pu, true);
  fChain->SetBranchAddress("jet_" + jc + "_flavor_component_jfitcomb_pb", &jet_flavor_component_jfitcomb_pb, true);
  fChain->SetBranchAddress("jet_" + jc + "_flavor_component_jfitcomb_pc", &jet_flavor_component_jfitcomb_pc, true);
  fChain->SetBranchAddress("vx_nTracks", &vx_nTracks, true); 

  if (br & unleash_sharktopus){  
    fChain->SetBranchAddress("sharktopus",&sharktopus, true); 
  }

}

#endif 

