#ifndef SusyBuffer_cxx
#define SusyBuffer_cxx
#include "SusyBuffer.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <stdexcept>

#include "cutflow.hh"

SusyBuffer::SusyBuffer(SmartChain *fChain, unsigned br)
{

   // Set object pointer
   el_author = 0;
   el_OQ = 0;
   el_nConv = 0;
   el_nSingleTrackConv = 0;
   el_nDoubleTrackConv = 0;
   el_OQRecalc = 0;
   el_mediumPP = 0;
   el_mediumPPIso = 0;
   el_weta2 = 0;
   el_nucone20 = 0;
   el_nucone30 = 0;
   el_nucone40 = 0;
   el_cl_E = 0;
   el_cl_eta = 0;
   el_cl_phi = 0;
   el_trackphi = 0;
   el_tracketa = 0;
   el_nBLHits = 0;
   el_nPixHits = 0;
   el_nSCTHits = 0;
   el_nTRTHits = 0;
   el_nTRTHighTHits = 0;
   el_nPixHoles = 0;
   el_nSCTHoles = 0;
   el_nTRTHoles = 0;
   el_nBLSharedHits = 0;
   el_nPixSharedHits = 0;
   el_nSCTSharedHits = 0;
   el_nBLayerOutliers = 0;
   el_nPixelOutliers = 0;
   el_nSCTOutliers = 0;
   el_nTRTOutliers = 0;
   el_nTRTHighTOutliers = 0;
   el_nSiHits = 0;
   el_nGoodHitsPixeldEdx = 0;
   mu_staco_E = 0;
   mu_staco_pt = 0;
   mu_staco_eta = 0;
   mu_staco_phi = 0;
   mu_staco_charge = 0;
   mu_staco_nucone20 = 0;
   mu_staco_nucone30 = 0;
   mu_staco_nucone40 = 0;
   mu_staco_ptcone20 = 0;
   mu_staco_ptcone30 = 0;
   mu_staco_ptcone40 = 0;
   mu_staco_isCombinedMuon = 0;
   mu_staco_isSegmentTaggedMuon = 0;
   mu_staco_loose = 0;
   mu_staco_phi_exPV = 0;
   mu_staco_id_theta_exPV = 0;
   mu_staco_id_qoverp_exPV = 0;
   mu_staco_me_theta_exPV = 0;
   mu_staco_me_qoverp_exPV = 0;
   mu_staco_ms_phi = 0;
   mu_staco_ms_theta = 0;
   mu_staco_ms_qoverp = 0;
   mu_staco_id_theta = 0;
   mu_staco_nOutliersOnTrack = 0;
   mu_staco_nBLHits = 0;
   mu_staco_nPixHits = 0;
   mu_staco_nSCTHits = 0;
   mu_staco_nTRTHits = 0;
   mu_staco_nTRTHighTHits = 0;
   mu_staco_nBLSharedHits = 0;
   mu_staco_nPixSharedHits = 0;
   mu_staco_nPixHoles = 0;
   mu_staco_nSCTSharedHits = 0;
   mu_staco_nSCTHoles = 0;
   mu_staco_nTRTOutliers = 0;
   mu_staco_nTRTHighTOutliers = 0;
   mu_staco_nGangedPixels = 0;
   mu_staco_nPixelDeadSensors = 0;
   mu_staco_nSCTDeadSensors = 0;
   mu_staco_nTRTDeadStraws = 0;
   mu_staco_expectBLayerHit = 0;
   mu_staco_nMDTHits = 0;
   mu_staco_nMDTHoles = 0;
   mu_staco_nCSCEtaHits = 0;
   mu_staco_nCSCEtaHoles = 0;
   mu_staco_nCSCPhiHits = 0;
   mu_staco_nCSCPhiHoles = 0;
   mu_staco_nRPCEtaHits = 0;
   mu_staco_nRPCEtaHoles = 0;
   mu_staco_nRPCPhiHits = 0;
   mu_staco_nRPCPhiHoles = 0;
   mu_staco_nTGCEtaHits = 0;
   mu_staco_nTGCEtaHoles = 0;
   mu_staco_nTGCPhiHits = 0;
   mu_staco_nTGCPhiHoles = 0;
   mu_staco_nMDTBIHits = 0;
   mu_staco_nMDTBMHits = 0;
   mu_staco_nMDTBOHits = 0;
   mu_staco_nMDTBEEHits = 0;
   mu_staco_nMDTBIS78Hits = 0;
   mu_staco_nMDTEIHits = 0;
   mu_staco_nMDTEMHits = 0;
   mu_staco_nMDTEOHits = 0;
   mu_staco_nMDTEEHits = 0;
   mu_staco_nRPCLayer1EtaHits = 0;
   mu_staco_nRPCLayer2EtaHits = 0;
   mu_staco_nRPCLayer3EtaHits = 0;
   mu_staco_nRPCLayer1PhiHits = 0;
   mu_staco_nRPCLayer2PhiHits = 0;
   mu_staco_nRPCLayer3PhiHits = 0;
   mu_staco_nTGCLayer1EtaHits = 0;
   mu_staco_nTGCLayer2EtaHits = 0;
   mu_staco_nTGCLayer3EtaHits = 0;
   mu_staco_nTGCLayer4EtaHits = 0;
   mu_staco_nTGCLayer1PhiHits = 0;
   mu_staco_nTGCLayer2PhiHits = 0;
   mu_staco_nTGCLayer3PhiHits = 0;
   mu_staco_nTGCLayer4PhiHits = 0;
   mu_staco_EFCB_dr = 0;
   mu_staco_EFCB_index = 0;
   mu_staco_EFMG_dr = 0;
   mu_staco_EFMG_index = 0;
   mu_staco_EFME_dr = 0;
   mu_staco_EFME_index = 0;
   el_MET_Simplified20_wpx = 0;
   el_MET_Simplified20_wpy = 0;
   el_MET_Simplified20_wet = 0;
   el_MET_Simplified20_statusWord = 0;
   jet_AntiKt4TopoNewEM_MET_Simplified20_wpx = 0;
   jet_AntiKt4TopoNewEM_MET_Simplified20_wpy = 0;
   jet_AntiKt4TopoNewEM_MET_Simplified20_wet = 0;
   jet_AntiKt4TopoNewEM_MET_Simplified20_statusWord = 0;
   jet_AntiKt4TopoNewEM_E = 0;
   jet_AntiKt4TopoNewEM_pt = 0;
   jet_AntiKt4TopoNewEM_eta = 0;
   jet_AntiKt4TopoNewEM_phi = 0;
   jet_AntiKt4TopoNewEM_EtaOrigin = 0;
   jet_AntiKt4TopoNewEM_PhiOrigin = 0;
   jet_AntiKt4TopoNewEM_MOrigin = 0;
   jet_AntiKt4TopoNewEM_EtaOriginEM = 0;
   jet_AntiKt4TopoNewEM_PhiOriginEM = 0;
   jet_AntiKt4TopoNewEM_MOriginEM = 0;
   jet_AntiKt4TopoNewEM_n90 = 0;
   jet_AntiKt4TopoNewEM_Timing = 0;
   jet_AntiKt4TopoNewEM_LArQuality = 0;
   jet_AntiKt4TopoNewEM_nTrk = 0;
   jet_AntiKt4TopoNewEM_sumPtTrk = 0;
   jet_AntiKt4TopoNewEM_HECQuality = 0;
   jet_AntiKt4TopoNewEM_NegativeE = 0;
   jet_AntiKt4TopoNewEM_AverageLArQF = 0;
   jet_AntiKt4TopoNewEM_BCH_CORR_CELL = 0;
   jet_AntiKt4TopoNewEM_BCH_CORR_DOTX = 0;
   jet_AntiKt4TopoNewEM_BCH_CORR_JET = 0;
   jet_AntiKt4TopoNewEM_BCH_CORR_JET_FORCELL = 0;
   jet_AntiKt4TopoNewEM_ENG_BAD_CELLS = 0;
   jet_AntiKt4TopoNewEM_fracSamplingMax = 0;
   jet_AntiKt4TopoNewEM_hecf = 0;
   jet_AntiKt4TopoNewEM_emfrac = 0;
   jet_AntiKt4TopoNewEM_EMJES = 0;
   jet_AntiKt4TopoNewEM_EMJES_EtaCorr = 0;
   jet_AntiKt4TopoNewEM_EMJESnooffset = 0;
   jet_AntiKt4TopoNewEM_emscale_E = 0;
   jet_AntiKt4TopoNewEM_emscale_eta = 0;
   jet_AntiKt4TopoNewEM_ptconst_default = 0;
   jet_AntiKt4TopoNewEM_etaconst_default = 0;
   jet_AntiKt4TopoNewEM_phiconst_default = 0;
   jet_AntiKt4TopoNewEM_flavor_weight_JetFitterCOMBNN = 0;
   jet_AntiKt4TopoNewEM_flavor_truth_label = 0;
   jet_AntiKt4TopoNewEM_EF_dr = 0;
   jet_AntiKt4TopoNewEM_EF_matched = 0;
   jet_AntiKt4TopoNewEM_flavor_component_jfitcomb_pu = 0;
   jet_AntiKt4TopoNewEM_flavor_component_jfitcomb_pb = 0;
   jet_AntiKt4TopoNewEM_flavor_component_jfitcomb_pc = 0;
   vx_nTracks = 0;
   trig_EF_el_author = 0;
   trig_EF_el_mediumPP = 0;
   trig_EF_el_mediumPPIso = 0;
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

   fChain->SetBranchStatus("top_hfor_type",other);
   fChain->SetBranchAddress("top_hfor_type", &top_hfor_type);  
   fChain->SetBranchStatus("RunNumber", br & run_num);
   fChain->SetBranchAddress("RunNumber", &RunNumber);  


   fChain->SetBranchStatus("EF_xe60_verytight_noMu", br & trigger);
   fChain->SetBranchAddress("EF_xe60_verytight_noMu", &EF_xe60_verytight_noMu); 
   fChain->SetBranchStatus("EF_xe70_noMu", br & trigger);
   fChain->SetBranchAddress("EF_xe70_noMu", &EF_xe70_noMu); 
   fChain->SetBranchStatus("averageIntPerXing",other);
   fChain->SetBranchAddress("averageIntPerXing", &averageIntPerXing); 
   fChain->SetBranchStatus("mc_channel_number",other);
   fChain->SetBranchAddress("mc_channel_number", &mc_channel_number); 
   fChain->SetBranchStatus("larError",other);
   fChain->SetBranchAddress("larError", &larError); 
   fChain->SetBranchStatus("el_n",other);
   fChain->SetBranchAddress("el_n", &el_n); 
   fChain->SetBranchStatus("el_author",other);
   fChain->SetBranchAddress("el_author", &el_author); 
   fChain->SetBranchStatus("el_OQ",other);
   fChain->SetBranchAddress("el_OQ", &el_OQ); 
   fChain->SetBranchStatus("el_nConv",other);
   fChain->SetBranchAddress("el_nConv", &el_nConv); 
   fChain->SetBranchStatus("el_nSingleTrackConv",other);
   fChain->SetBranchAddress("el_nSingleTrackConv", &el_nSingleTrackConv); 
   fChain->SetBranchStatus("el_nDoubleTrackConv",other);
   fChain->SetBranchAddress("el_nDoubleTrackConv", &el_nDoubleTrackConv); 
   fChain->SetBranchStatus("el_OQRecalc",other);
   fChain->SetBranchAddress("el_OQRecalc", &el_OQRecalc); 
   fChain->SetBranchStatus("el_mediumPP",other);
   fChain->SetBranchAddress("el_mediumPP", &el_mediumPP); 
   fChain->SetBranchStatus("el_mediumPPIso",other);
   fChain->SetBranchAddress("el_mediumPPIso", &el_mediumPPIso); 
   fChain->SetBranchStatus("el_weta2",other);
   fChain->SetBranchAddress("el_weta2", &el_weta2); 
   fChain->SetBranchStatus("el_nucone20",other);
   fChain->SetBranchAddress("el_nucone20", &el_nucone20); 
   fChain->SetBranchStatus("el_nucone30",other);
   fChain->SetBranchAddress("el_nucone30", &el_nucone30); 
   fChain->SetBranchStatus("el_nucone40",other);
   fChain->SetBranchAddress("el_nucone40", &el_nucone40); 
   fChain->SetBranchStatus("el_cl_E",other);
   fChain->SetBranchAddress("el_cl_E", &el_cl_E); 
   fChain->SetBranchStatus("el_cl_eta",other);
   fChain->SetBranchAddress("el_cl_eta", &el_cl_eta); 
   fChain->SetBranchStatus("el_cl_phi",other);
   fChain->SetBranchAddress("el_cl_phi", &el_cl_phi); 
   fChain->SetBranchStatus("el_trackphi",other);
   fChain->SetBranchAddress("el_trackphi", &el_trackphi); 
   fChain->SetBranchStatus("el_tracketa",other);
   fChain->SetBranchAddress("el_tracketa", &el_tracketa); 
   fChain->SetBranchStatus("el_nBLHits",other);
   fChain->SetBranchAddress("el_nBLHits", &el_nBLHits); 
   fChain->SetBranchStatus("el_nPixHits",other);
   fChain->SetBranchAddress("el_nPixHits", &el_nPixHits); 
   fChain->SetBranchStatus("el_nSCTHits",other);
   fChain->SetBranchAddress("el_nSCTHits", &el_nSCTHits); 
   fChain->SetBranchStatus("el_nTRTHits",other);
   fChain->SetBranchAddress("el_nTRTHits", &el_nTRTHits); 
   fChain->SetBranchStatus("el_nTRTHighTHits",other);
   fChain->SetBranchAddress("el_nTRTHighTHits", &el_nTRTHighTHits); 
   fChain->SetBranchStatus("el_nPixHoles",other);
   fChain->SetBranchAddress("el_nPixHoles", &el_nPixHoles); 
   fChain->SetBranchStatus("el_nSCTHoles",other);
   fChain->SetBranchAddress("el_nSCTHoles", &el_nSCTHoles); 
   fChain->SetBranchStatus("el_nTRTHoles",other);
   fChain->SetBranchAddress("el_nTRTHoles", &el_nTRTHoles); 
   fChain->SetBranchStatus("el_nBLSharedHits",other);
   fChain->SetBranchAddress("el_nBLSharedHits", &el_nBLSharedHits); 
   fChain->SetBranchStatus("el_nPixSharedHits",other);
   fChain->SetBranchAddress("el_nPixSharedHits", &el_nPixSharedHits); 
   fChain->SetBranchStatus("el_nSCTSharedHits",other);
   fChain->SetBranchAddress("el_nSCTSharedHits", &el_nSCTSharedHits); 
   fChain->SetBranchStatus("el_nBLayerOutliers",other);
   fChain->SetBranchAddress("el_nBLayerOutliers", &el_nBLayerOutliers); 
   fChain->SetBranchStatus("el_nPixelOutliers",other);
   fChain->SetBranchAddress("el_nPixelOutliers", &el_nPixelOutliers); 
   fChain->SetBranchStatus("el_nSCTOutliers",other);
   fChain->SetBranchAddress("el_nSCTOutliers", &el_nSCTOutliers); 
   fChain->SetBranchStatus("el_nTRTOutliers",other);
   fChain->SetBranchAddress("el_nTRTOutliers", &el_nTRTOutliers); 
   fChain->SetBranchStatus("el_nTRTHighTOutliers",other);
   fChain->SetBranchAddress("el_nTRTHighTOutliers", &el_nTRTHighTOutliers); 
   fChain->SetBranchStatus("el_nSiHits",other);
   fChain->SetBranchAddress("el_nSiHits", &el_nSiHits); 
   fChain->SetBranchStatus("el_nGoodHitsPixeldEdx",other);
   fChain->SetBranchAddress("el_nGoodHitsPixeldEdx", &el_nGoodHitsPixeldEdx); 
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
   fChain->SetBranchStatus("mu_staco_nucone20",other);
   fChain->SetBranchAddress("mu_staco_nucone20", &mu_staco_nucone20); 
   fChain->SetBranchStatus("mu_staco_nucone30",other);
   fChain->SetBranchAddress("mu_staco_nucone30", &mu_staco_nucone30); 
   fChain->SetBranchStatus("mu_staco_nucone40",other);
   fChain->SetBranchAddress("mu_staco_nucone40", &mu_staco_nucone40); 
   fChain->SetBranchStatus("mu_staco_ptcone20",other);
   fChain->SetBranchAddress("mu_staco_ptcone20", &mu_staco_ptcone20); 
   fChain->SetBranchStatus("mu_staco_ptcone30",other);
   fChain->SetBranchAddress("mu_staco_ptcone30", &mu_staco_ptcone30); 
   fChain->SetBranchStatus("mu_staco_ptcone40",other);
   fChain->SetBranchAddress("mu_staco_ptcone40", &mu_staco_ptcone40); 
   fChain->SetBranchStatus("mu_staco_isCombinedMuon",other);
   fChain->SetBranchAddress("mu_staco_isCombinedMuon", &mu_staco_isCombinedMuon); 
   fChain->SetBranchStatus("mu_staco_isSegmentTaggedMuon",other);
   fChain->SetBranchAddress("mu_staco_isSegmentTaggedMuon", &mu_staco_isSegmentTaggedMuon); 
   fChain->SetBranchStatus("mu_staco_loose",other);
   fChain->SetBranchAddress("mu_staco_loose", &mu_staco_loose); 
   fChain->SetBranchStatus("mu_staco_phi_exPV",other);
   fChain->SetBranchAddress("mu_staco_phi_exPV", &mu_staco_phi_exPV); 
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
   fChain->SetBranchStatus("mu_staco_nOutliersOnTrack",other);
   fChain->SetBranchAddress("mu_staco_nOutliersOnTrack", &mu_staco_nOutliersOnTrack); 
   fChain->SetBranchStatus("mu_staco_nBLHits",other);
   fChain->SetBranchAddress("mu_staco_nBLHits", &mu_staco_nBLHits); 
   fChain->SetBranchStatus("mu_staco_nPixHits",other);
   fChain->SetBranchAddress("mu_staco_nPixHits", &mu_staco_nPixHits); 
   fChain->SetBranchStatus("mu_staco_nSCTHits",other);
   fChain->SetBranchAddress("mu_staco_nSCTHits", &mu_staco_nSCTHits); 
   fChain->SetBranchStatus("mu_staco_nTRTHits",other);
   fChain->SetBranchAddress("mu_staco_nTRTHits", &mu_staco_nTRTHits); 
   fChain->SetBranchStatus("mu_staco_nTRTHighTHits",other);
   fChain->SetBranchAddress("mu_staco_nTRTHighTHits", &mu_staco_nTRTHighTHits); 
   fChain->SetBranchStatus("mu_staco_nBLSharedHits",other);
   fChain->SetBranchAddress("mu_staco_nBLSharedHits", &mu_staco_nBLSharedHits); 
   fChain->SetBranchStatus("mu_staco_nPixSharedHits",other);
   fChain->SetBranchAddress("mu_staco_nPixSharedHits", &mu_staco_nPixSharedHits); 
   fChain->SetBranchStatus("mu_staco_nPixHoles",other);
   fChain->SetBranchAddress("mu_staco_nPixHoles", &mu_staco_nPixHoles); 
   fChain->SetBranchStatus("mu_staco_nSCTSharedHits",other);
   fChain->SetBranchAddress("mu_staco_nSCTSharedHits", &mu_staco_nSCTSharedHits); 
   fChain->SetBranchStatus("mu_staco_nSCTHoles",other);
   fChain->SetBranchAddress("mu_staco_nSCTHoles", &mu_staco_nSCTHoles); 
   fChain->SetBranchStatus("mu_staco_nTRTOutliers",other);
   fChain->SetBranchAddress("mu_staco_nTRTOutliers", &mu_staco_nTRTOutliers); 
   fChain->SetBranchStatus("mu_staco_nTRTHighTOutliers",other);
   fChain->SetBranchAddress("mu_staco_nTRTHighTOutliers", &mu_staco_nTRTHighTOutliers); 
   fChain->SetBranchStatus("mu_staco_nGangedPixels",other);
   fChain->SetBranchAddress("mu_staco_nGangedPixels", &mu_staco_nGangedPixels); 
   fChain->SetBranchStatus("mu_staco_nPixelDeadSensors",other);
   fChain->SetBranchAddress("mu_staco_nPixelDeadSensors", &mu_staco_nPixelDeadSensors); 
   fChain->SetBranchStatus("mu_staco_nSCTDeadSensors",other);
   fChain->SetBranchAddress("mu_staco_nSCTDeadSensors", &mu_staco_nSCTDeadSensors); 
   fChain->SetBranchStatus("mu_staco_nTRTDeadStraws",other);
   fChain->SetBranchAddress("mu_staco_nTRTDeadStraws", &mu_staco_nTRTDeadStraws); 
   fChain->SetBranchStatus("mu_staco_expectBLayerHit",other);
   fChain->SetBranchAddress("mu_staco_expectBLayerHit", &mu_staco_expectBLayerHit); 
   fChain->SetBranchStatus("mu_staco_nMDTHits",other);
   fChain->SetBranchAddress("mu_staco_nMDTHits", &mu_staco_nMDTHits); 
   fChain->SetBranchStatus("mu_staco_nMDTHoles",other);
   fChain->SetBranchAddress("mu_staco_nMDTHoles", &mu_staco_nMDTHoles); 
   fChain->SetBranchStatus("mu_staco_nCSCEtaHits",other);
   fChain->SetBranchAddress("mu_staco_nCSCEtaHits", &mu_staco_nCSCEtaHits); 
   fChain->SetBranchStatus("mu_staco_nCSCEtaHoles",other);
   fChain->SetBranchAddress("mu_staco_nCSCEtaHoles", &mu_staco_nCSCEtaHoles); 
   fChain->SetBranchStatus("mu_staco_nCSCPhiHits",other);
   fChain->SetBranchAddress("mu_staco_nCSCPhiHits", &mu_staco_nCSCPhiHits); 
   fChain->SetBranchStatus("mu_staco_nCSCPhiHoles",other);
   fChain->SetBranchAddress("mu_staco_nCSCPhiHoles", &mu_staco_nCSCPhiHoles); 
   fChain->SetBranchStatus("mu_staco_nRPCEtaHits",other);
   fChain->SetBranchAddress("mu_staco_nRPCEtaHits", &mu_staco_nRPCEtaHits); 
   fChain->SetBranchStatus("mu_staco_nRPCEtaHoles",other);
   fChain->SetBranchAddress("mu_staco_nRPCEtaHoles", &mu_staco_nRPCEtaHoles); 
   fChain->SetBranchStatus("mu_staco_nRPCPhiHits",other);
   fChain->SetBranchAddress("mu_staco_nRPCPhiHits", &mu_staco_nRPCPhiHits); 
   fChain->SetBranchStatus("mu_staco_nRPCPhiHoles",other);
   fChain->SetBranchAddress("mu_staco_nRPCPhiHoles", &mu_staco_nRPCPhiHoles); 
   fChain->SetBranchStatus("mu_staco_nTGCEtaHits",other);
   fChain->SetBranchAddress("mu_staco_nTGCEtaHits", &mu_staco_nTGCEtaHits); 
   fChain->SetBranchStatus("mu_staco_nTGCEtaHoles",other);
   fChain->SetBranchAddress("mu_staco_nTGCEtaHoles", &mu_staco_nTGCEtaHoles); 
   fChain->SetBranchStatus("mu_staco_nTGCPhiHits",other);
   fChain->SetBranchAddress("mu_staco_nTGCPhiHits", &mu_staco_nTGCPhiHits); 
   fChain->SetBranchStatus("mu_staco_nTGCPhiHoles",other);
   fChain->SetBranchAddress("mu_staco_nTGCPhiHoles", &mu_staco_nTGCPhiHoles); 
   fChain->SetBranchStatus("mu_staco_nMDTBIHits",other);
   fChain->SetBranchAddress("mu_staco_nMDTBIHits", &mu_staco_nMDTBIHits); 
   fChain->SetBranchStatus("mu_staco_nMDTBMHits",other);
   fChain->SetBranchAddress("mu_staco_nMDTBMHits", &mu_staco_nMDTBMHits); 
   fChain->SetBranchStatus("mu_staco_nMDTBOHits",other);
   fChain->SetBranchAddress("mu_staco_nMDTBOHits", &mu_staco_nMDTBOHits); 
   fChain->SetBranchStatus("mu_staco_nMDTBEEHits",other);
   fChain->SetBranchAddress("mu_staco_nMDTBEEHits", &mu_staco_nMDTBEEHits); 
   fChain->SetBranchStatus("mu_staco_nMDTBIS78Hits",other);
   fChain->SetBranchAddress("mu_staco_nMDTBIS78Hits", &mu_staco_nMDTBIS78Hits); 
   fChain->SetBranchStatus("mu_staco_nMDTEIHits",other);
   fChain->SetBranchAddress("mu_staco_nMDTEIHits", &mu_staco_nMDTEIHits); 
   fChain->SetBranchStatus("mu_staco_nMDTEMHits",other);
   fChain->SetBranchAddress("mu_staco_nMDTEMHits", &mu_staco_nMDTEMHits); 
   fChain->SetBranchStatus("mu_staco_nMDTEOHits",other);
   fChain->SetBranchAddress("mu_staco_nMDTEOHits", &mu_staco_nMDTEOHits); 
   fChain->SetBranchStatus("mu_staco_nMDTEEHits",other);
   fChain->SetBranchAddress("mu_staco_nMDTEEHits", &mu_staco_nMDTEEHits); 
   fChain->SetBranchStatus("mu_staco_nRPCLayer1EtaHits",other);
   fChain->SetBranchAddress("mu_staco_nRPCLayer1EtaHits", &mu_staco_nRPCLayer1EtaHits); 
   fChain->SetBranchStatus("mu_staco_nRPCLayer2EtaHits",other);
   fChain->SetBranchAddress("mu_staco_nRPCLayer2EtaHits", &mu_staco_nRPCLayer2EtaHits); 
   fChain->SetBranchStatus("mu_staco_nRPCLayer3EtaHits",other);
   fChain->SetBranchAddress("mu_staco_nRPCLayer3EtaHits", &mu_staco_nRPCLayer3EtaHits); 
   fChain->SetBranchStatus("mu_staco_nRPCLayer1PhiHits",other);
   fChain->SetBranchAddress("mu_staco_nRPCLayer1PhiHits", &mu_staco_nRPCLayer1PhiHits); 
   fChain->SetBranchStatus("mu_staco_nRPCLayer2PhiHits",other);
   fChain->SetBranchAddress("mu_staco_nRPCLayer2PhiHits", &mu_staco_nRPCLayer2PhiHits); 
   fChain->SetBranchStatus("mu_staco_nRPCLayer3PhiHits",other);
   fChain->SetBranchAddress("mu_staco_nRPCLayer3PhiHits", &mu_staco_nRPCLayer3PhiHits); 
   fChain->SetBranchStatus("mu_staco_nTGCLayer1EtaHits",other);
   fChain->SetBranchAddress("mu_staco_nTGCLayer1EtaHits", &mu_staco_nTGCLayer1EtaHits); 
   fChain->SetBranchStatus("mu_staco_nTGCLayer2EtaHits",other);
   fChain->SetBranchAddress("mu_staco_nTGCLayer2EtaHits", &mu_staco_nTGCLayer2EtaHits); 
   fChain->SetBranchStatus("mu_staco_nTGCLayer3EtaHits",other);
   fChain->SetBranchAddress("mu_staco_nTGCLayer3EtaHits", &mu_staco_nTGCLayer3EtaHits); 
   fChain->SetBranchStatus("mu_staco_nTGCLayer4EtaHits",other);
   fChain->SetBranchAddress("mu_staco_nTGCLayer4EtaHits", &mu_staco_nTGCLayer4EtaHits); 
   fChain->SetBranchStatus("mu_staco_nTGCLayer1PhiHits",other);
   fChain->SetBranchAddress("mu_staco_nTGCLayer1PhiHits", &mu_staco_nTGCLayer1PhiHits); 
   fChain->SetBranchStatus("mu_staco_nTGCLayer2PhiHits",other);
   fChain->SetBranchAddress("mu_staco_nTGCLayer2PhiHits", &mu_staco_nTGCLayer2PhiHits); 
   fChain->SetBranchStatus("mu_staco_nTGCLayer3PhiHits",other);
   fChain->SetBranchAddress("mu_staco_nTGCLayer3PhiHits", &mu_staco_nTGCLayer3PhiHits); 
   fChain->SetBranchStatus("mu_staco_nTGCLayer4PhiHits",other);
   fChain->SetBranchAddress("mu_staco_nTGCLayer4PhiHits", &mu_staco_nTGCLayer4PhiHits); 
   fChain->SetBranchStatus("mu_staco_EFCB_dr",other);
   fChain->SetBranchAddress("mu_staco_EFCB_dr", &mu_staco_EFCB_dr); 
   fChain->SetBranchStatus("mu_staco_EFCB_index",other);
   fChain->SetBranchAddress("mu_staco_EFCB_index", &mu_staco_EFCB_index); 
   fChain->SetBranchStatus("mu_staco_EFMG_dr",other);
   fChain->SetBranchAddress("mu_staco_EFMG_dr", &mu_staco_EFMG_dr); 
   fChain->SetBranchStatus("mu_staco_EFMG_index",other);
   fChain->SetBranchAddress("mu_staco_EFMG_index", &mu_staco_EFMG_index); 
   fChain->SetBranchStatus("mu_staco_EFME_dr",other);
   fChain->SetBranchAddress("mu_staco_EFME_dr", &mu_staco_EFME_dr); 
   fChain->SetBranchStatus("mu_staco_EFME_index",other);
   fChain->SetBranchAddress("mu_staco_EFME_index", &mu_staco_EFME_index); 
   fChain->SetBranchStatus("MET_SoftJets_etx",other);
   fChain->SetBranchAddress("MET_SoftJets_etx", &MET_SoftJets_etx); 
   fChain->SetBranchStatus("MET_SoftJets_ety",other);
   fChain->SetBranchAddress("MET_SoftJets_ety", &MET_SoftJets_ety); 
   fChain->SetBranchStatus("MET_SoftJets_sumet",other);
   fChain->SetBranchAddress("MET_SoftJets_sumet", &MET_SoftJets_sumet); 
   fChain->SetBranchStatus("MET_CellOut_etx",other);
   fChain->SetBranchAddress("MET_CellOut_etx", &MET_CellOut_etx); 
   fChain->SetBranchStatus("MET_CellOut_ety",other);
   fChain->SetBranchAddress("MET_CellOut_ety", &MET_CellOut_ety); 
   fChain->SetBranchStatus("MET_CellOut_sumet",other);
   fChain->SetBranchAddress("MET_CellOut_sumet", &MET_CellOut_sumet); 
   fChain->SetBranchStatus("MET_CellOut_etx_CentralReg",other);
   fChain->SetBranchAddress("MET_CellOut_etx_CentralReg", &MET_CellOut_etx_CentralReg); 
   fChain->SetBranchStatus("MET_CellOut_ety_CentralReg",other);
   fChain->SetBranchAddress("MET_CellOut_ety_CentralReg", &MET_CellOut_ety_CentralReg); 
   fChain->SetBranchStatus("MET_CellOut_sumet_CentralReg",other);
   fChain->SetBranchAddress("MET_CellOut_sumet_CentralReg", &MET_CellOut_sumet_CentralReg); 
   fChain->SetBranchStatus("MET_CellOut_etx_EndcapRegion",other);
   fChain->SetBranchAddress("MET_CellOut_etx_EndcapRegion", &MET_CellOut_etx_EndcapRegion); 
   fChain->SetBranchStatus("MET_CellOut_ety_EndcapRegion",other);
   fChain->SetBranchAddress("MET_CellOut_ety_EndcapRegion", &MET_CellOut_ety_EndcapRegion); 
   fChain->SetBranchStatus("MET_CellOut_sumet_EndcapRegion",other);
   fChain->SetBranchAddress("MET_CellOut_sumet_EndcapRegion", &MET_CellOut_sumet_EndcapRegion); 
   fChain->SetBranchStatus("MET_CellOut_etx_ForwardReg",other);
   fChain->SetBranchAddress("MET_CellOut_etx_ForwardReg", &MET_CellOut_etx_ForwardReg); 
   fChain->SetBranchStatus("MET_CellOut_ety_ForwardReg",other);
   fChain->SetBranchAddress("MET_CellOut_ety_ForwardReg", &MET_CellOut_ety_ForwardReg); 
   fChain->SetBranchStatus("MET_CellOut_sumet_ForwardReg",other);
   fChain->SetBranchAddress("MET_CellOut_sumet_ForwardReg", &MET_CellOut_sumet_ForwardReg); 
   fChain->SetBranchStatus("MET_RefGamma_em_etx",other);
   fChain->SetBranchAddress("MET_RefGamma_em_etx", &MET_RefGamma_em_etx); 
   fChain->SetBranchStatus("MET_RefGamma_em_ety",other);
   fChain->SetBranchAddress("MET_RefGamma_em_ety", &MET_RefGamma_em_ety); 
   fChain->SetBranchStatus("MET_RefGamma_em_sumet",other);
   fChain->SetBranchAddress("MET_RefGamma_em_sumet", &MET_RefGamma_em_sumet); 
   fChain->SetBranchStatus("MET_CellOut_Eflow_etx",other);
   fChain->SetBranchAddress("MET_CellOut_Eflow_etx", &MET_CellOut_Eflow_etx); 
   fChain->SetBranchStatus("MET_CellOut_Eflow_ety",other);
   fChain->SetBranchAddress("MET_CellOut_Eflow_ety", &MET_CellOut_Eflow_ety); 
   fChain->SetBranchStatus("MET_CellOut_Eflow_sumet",other);
   fChain->SetBranchAddress("MET_CellOut_Eflow_sumet", &MET_CellOut_Eflow_sumet); 
   fChain->SetBranchStatus("MET_Simplified20_RefFinal_etx",other);
   fChain->SetBranchAddress("MET_Simplified20_RefFinal_etx", &MET_Simplified20_RefFinal_etx); 
   fChain->SetBranchStatus("MET_Simplified20_RefFinal_ety",other);
   fChain->SetBranchAddress("MET_Simplified20_RefFinal_ety", &MET_Simplified20_RefFinal_ety); 
   fChain->SetBranchStatus("MET_Simplified20_RefFinal_etx_CentralReg",other);
   fChain->SetBranchAddress("MET_Simplified20_RefFinal_etx_CentralReg", &MET_Simplified20_RefFinal_etx_CentralReg); 
   fChain->SetBranchStatus("MET_Simplified20_RefFinal_ety_CentralReg",other);
   fChain->SetBranchAddress("MET_Simplified20_RefFinal_ety_CentralReg", &MET_Simplified20_RefFinal_ety_CentralReg); 
   fChain->SetBranchStatus("MET_Simplified20_RefFinal_etx_EndcapRegion",other);
   fChain->SetBranchAddress("MET_Simplified20_RefFinal_etx_EndcapRegion", &MET_Simplified20_RefFinal_etx_EndcapRegion); 
   fChain->SetBranchStatus("MET_Simplified20_RefFinal_ety_EndcapRegion",other);
   fChain->SetBranchAddress("MET_Simplified20_RefFinal_ety_EndcapRegion", &MET_Simplified20_RefFinal_ety_EndcapRegion); 
   fChain->SetBranchStatus("MET_Simplified20_RefFinal_etx_ForwardReg",other);
   fChain->SetBranchAddress("MET_Simplified20_RefFinal_etx_ForwardReg", &MET_Simplified20_RefFinal_etx_ForwardReg); 
   fChain->SetBranchStatus("MET_Simplified20_RefFinal_ety_ForwardReg",other);
   fChain->SetBranchAddress("MET_Simplified20_RefFinal_ety_ForwardReg", &MET_Simplified20_RefFinal_ety_ForwardReg); 
   fChain->SetBranchStatus("el_MET_Simplified20_wpx",other);
   fChain->SetBranchAddress("el_MET_Simplified20_wpx", &el_MET_Simplified20_wpx); 
   fChain->SetBranchStatus("el_MET_Simplified20_wpy",other);
   fChain->SetBranchAddress("el_MET_Simplified20_wpy", &el_MET_Simplified20_wpy); 
   fChain->SetBranchStatus("el_MET_Simplified20_wet",other);
   fChain->SetBranchAddress("el_MET_Simplified20_wet", &el_MET_Simplified20_wet); 
   fChain->SetBranchStatus("el_MET_Simplified20_statusWord",other);
   fChain->SetBranchAddress("el_MET_Simplified20_statusWord", &el_MET_Simplified20_statusWord); 
   fChain->SetBranchStatus("jet_AntiKt4TopoNewEM_MET_Simplified20_wpx",other);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_MET_Simplified20_wpx", &jet_AntiKt4TopoNewEM_MET_Simplified20_wpx); 
   fChain->SetBranchStatus("jet_AntiKt4TopoNewEM_MET_Simplified20_wpy",other);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_MET_Simplified20_wpy", &jet_AntiKt4TopoNewEM_MET_Simplified20_wpy); 
   fChain->SetBranchStatus("jet_AntiKt4TopoNewEM_MET_Simplified20_wet",other);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_MET_Simplified20_wet", &jet_AntiKt4TopoNewEM_MET_Simplified20_wet); 
   fChain->SetBranchStatus("jet_AntiKt4TopoNewEM_MET_Simplified20_statusWord",other);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_MET_Simplified20_statusWord", &jet_AntiKt4TopoNewEM_MET_Simplified20_statusWord); 
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
   fChain->SetBranchStatus("jet_AntiKt4TopoNewEM_EtaOriginEM",other);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_EtaOriginEM", &jet_AntiKt4TopoNewEM_EtaOriginEM); 
   fChain->SetBranchStatus("jet_AntiKt4TopoNewEM_PhiOriginEM",other);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_PhiOriginEM", &jet_AntiKt4TopoNewEM_PhiOriginEM); 
   fChain->SetBranchStatus("jet_AntiKt4TopoNewEM_MOriginEM",other);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_MOriginEM", &jet_AntiKt4TopoNewEM_MOriginEM); 
   fChain->SetBranchStatus("jet_AntiKt4TopoNewEM_n90",other);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_n90", &jet_AntiKt4TopoNewEM_n90); 
   fChain->SetBranchStatus("jet_AntiKt4TopoNewEM_Timing",other);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_Timing", &jet_AntiKt4TopoNewEM_Timing); 
   fChain->SetBranchStatus("jet_AntiKt4TopoNewEM_LArQuality",other);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_LArQuality", &jet_AntiKt4TopoNewEM_LArQuality); 
   fChain->SetBranchStatus("jet_AntiKt4TopoNewEM_nTrk",other);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_nTrk", &jet_AntiKt4TopoNewEM_nTrk); 
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
   fChain->SetBranchStatus("jet_AntiKt4TopoNewEM_BCH_CORR_JET_FORCELL",other);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_BCH_CORR_JET_FORCELL", &jet_AntiKt4TopoNewEM_BCH_CORR_JET_FORCELL); 
   fChain->SetBranchStatus("jet_AntiKt4TopoNewEM_ENG_BAD_CELLS",other);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_ENG_BAD_CELLS", &jet_AntiKt4TopoNewEM_ENG_BAD_CELLS); 
   fChain->SetBranchStatus("jet_AntiKt4TopoNewEM_fracSamplingMax",other);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_fracSamplingMax", &jet_AntiKt4TopoNewEM_fracSamplingMax); 
   fChain->SetBranchStatus("jet_AntiKt4TopoNewEM_hecf",other);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_hecf", &jet_AntiKt4TopoNewEM_hecf); 
   fChain->SetBranchStatus("jet_AntiKt4TopoNewEM_emfrac",other);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_emfrac", &jet_AntiKt4TopoNewEM_emfrac); 
   fChain->SetBranchStatus("jet_AntiKt4TopoNewEM_EMJES",other);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_EMJES", &jet_AntiKt4TopoNewEM_EMJES); 
   fChain->SetBranchStatus("jet_AntiKt4TopoNewEM_EMJES_EtaCorr",other);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_EMJES_EtaCorr", &jet_AntiKt4TopoNewEM_EMJES_EtaCorr); 
   fChain->SetBranchStatus("jet_AntiKt4TopoNewEM_EMJESnooffset",other);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_EMJESnooffset", &jet_AntiKt4TopoNewEM_EMJESnooffset); 
   fChain->SetBranchStatus("jet_AntiKt4TopoNewEM_emscale_E",other);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_emscale_E", &jet_AntiKt4TopoNewEM_emscale_E); 
   fChain->SetBranchStatus("jet_AntiKt4TopoNewEM_emscale_eta",other);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_emscale_eta", &jet_AntiKt4TopoNewEM_emscale_eta); 
   fChain->SetBranchStatus("jet_AntiKt4TopoNewEM_ptconst_default",other);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_ptconst_default", &jet_AntiKt4TopoNewEM_ptconst_default); 
   fChain->SetBranchStatus("jet_AntiKt4TopoNewEM_etaconst_default",other);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_etaconst_default", &jet_AntiKt4TopoNewEM_etaconst_default); 
   fChain->SetBranchStatus("jet_AntiKt4TopoNewEM_phiconst_default",other);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_phiconst_default", &jet_AntiKt4TopoNewEM_phiconst_default); 
   fChain->SetBranchStatus("jet_AntiKt4TopoNewEM_flavor_weight_JetFitterCOMBNN",other);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_flavor_weight_JetFitterCOMBNN", &jet_AntiKt4TopoNewEM_flavor_weight_JetFitterCOMBNN); 
   fChain->SetBranchStatus("jet_AntiKt4TopoNewEM_flavor_truth_label",other);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_flavor_truth_label", &jet_AntiKt4TopoNewEM_flavor_truth_label); 
   fChain->SetBranchStatus("jet_AntiKt4TopoNewEM_EF_dr",other);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_EF_dr", &jet_AntiKt4TopoNewEM_EF_dr); 
   fChain->SetBranchStatus("jet_AntiKt4TopoNewEM_EF_matched",other);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_EF_matched", &jet_AntiKt4TopoNewEM_EF_matched); 
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_flavor_component_jfitcomb_pu", &jet_AntiKt4TopoNewEM_flavor_component_jfitcomb_pu);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_flavor_component_jfitcomb_pb", &jet_AntiKt4TopoNewEM_flavor_component_jfitcomb_pb);
   fChain->SetBranchAddress("jet_AntiKt4TopoNewEM_flavor_component_jfitcomb_pc", &jet_AntiKt4TopoNewEM_flavor_component_jfitcomb_pc);
   fChain->SetBranchStatus("vx_nTracks",other);
   fChain->SetBranchAddress("vx_nTracks", &vx_nTracks); 
   fChain->SetBranchStatus("trig_L2_el_n",other);
   fChain->SetBranchAddress("trig_L2_el_n", &trig_L2_el_n); 
   fChain->SetBranchStatus("trig_EF_el_n",other);
   fChain->SetBranchAddress("trig_EF_el_n", &trig_EF_el_n); 
   fChain->SetBranchStatus("trig_EF_el_author",other);
   fChain->SetBranchAddress("trig_EF_el_author", &trig_EF_el_author); 
   fChain->SetBranchStatus("trig_EF_el_mediumPP",other);
   fChain->SetBranchAddress("trig_EF_el_mediumPP", &trig_EF_el_mediumPP); 
   fChain->SetBranchStatus("trig_EF_el_mediumPPIso",other);
   fChain->SetBranchAddress("trig_EF_el_mediumPPIso", &trig_EF_el_mediumPPIso);

   fChain->SetBranchStatus("sharktopus", 1); 
   fChain->SetBranchAddress("sharktopus",&sharktopus); 

   // Notify();
}

#endif 

// Bool_t susy::Notify()
// {
//    // The Notify() function is called when a new file is opened. This
//    // can be either for a new TTree in a TChain or when when a new TTree
//    // is started when using PROOF. It is normally not necessary to make changes
//    // to the generated code, but the routine can be extended by the
//    // user if needed. The return value is currently not used.

//    return kTRUE;
// }

// void susy::Show(Long64_t entry)
// {
// // Print contents of entry.
// // If entry is not specified, print current entry
//    if (!fChain) return;
//    fChain->Show(entry);
// }
// Int_t susy::Cut(Long64_t entry)
// {
// // This function may be called from Loop.
// // returns  1 if entry is accepted.
// // returns -1 otherwise.
//    return 1;
// }


// void susy::Loop()
// {
// //   In a ROOT session, you can do:
// //      Root > .L susy.C
// //      Root > susy t
// //      Root > t.GetEntry(12); // Fill t data members with entry number 12
// //      Root > t.Show();       // Show values of entry 12
// //      Root > t.Show(16);     // Read and show values of entry 16
// //      Root > t.Loop();       // Loop on all entries
// //

// //     This is the loop skeleton where:
// //    jentry is the global entry number in the chain
// //    ientry is the entry number in the current Tree
// //  Note that the argument to GetEntry must be:
// //    jentry for TChain::GetEntry
// //    ientry for TTree::GetEntry and TBranch::GetEntry
// //
// //       To read only selected branches, Insert statements like:
// // METHOD1:
// //    fChain->SetBranchStatus("*",0);  // disable all branches
// //    fChain->SetBranchStatus("branchname",1);  // activate branchname
// // METHOD2: replace line
// //    fChain->GetEntry(jentry);       //read all branches
// //by  b_branchname->GetEntry(ientry); //read only this branch
//    if (fChain == 0) return;

//    Long64_t nentries = fChain->GetEntriesFast();

//    Long64_t nbytes = 0, nb = 0;
//    for (Long64_t jentry=0; jentry<nentries;jentry++) {
//       Long64_t ientry = LoadTree(jentry);
//       if (ientry < 0) break;
//       nb = fChain->GetEntry(jentry);   nbytes += nb;
//       // if (Cut(ientry) < 0) continue;
//    }
// }
