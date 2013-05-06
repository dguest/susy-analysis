#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <stdexcept>

#include "SusyBuffer.h"
#include "SmartChain.hh"
#include "RunBits.hh"

SusyBuffer::SusyBuffer(SmartChain *fChain, const unsigned br, 
		       BranchNames names)
{

  using namespace cutflag; 

  std::string jc = "jet_AntiKt4LCTopo"; 

  fChain->SetBranchStatus("*",0); 

  fChain->SetBranch("RunNumber", &RunNumber);  
  fChain->SetBranch("EventNumber", &EventNumber); 
  fChain->SetBranch("lbn", &lbn); 

  fChain->SetBranch(names.trigger, &trigger); 
  fChain->SetBranch("EF_mu24i_tight", &mu_trigger); 
  fChain->SetBranch("coreFlags", &coreFlags); 
  
  fChain->SetBranch("top_hfor_type", &hfor_type); 

  fChain->SetBranch(jc + "_jvtxf", 
		    &jet_jvtxf); 
  fChain->SetBranch("averageIntPerXing", &averageIntPerXing); 
  fChain->SetBranch("larError", &larError); 
  fChain->SetBranch("tileError", &tileError); 

  fChain->SetBranch("Eventshape_rhoKt4LC", &Eventshape_rhoKt4LC); 

  // MET garbage
  fChain->SetBranch(jc + "_MET_Egamma10NoTau_wet", 
		    &jet_MET_Egamma10NoTau_wet); 
  fChain->SetBranch(jc + "_MET_Egamma10NoTau_wpx",
		    &jet_MET_Egamma10NoTau_wpx); 
  fChain->SetBranch(jc + "_MET_Egamma10NoTau_wpy", 
		    &jet_MET_Egamma10NoTau_wpy); 
  fChain->SetBranch(jc + "_MET_Egamma10NoTau_statusWord", 
		    &jet_MET_Egamma10NoTau_statusWord); 

  fChain->SetBranch("el_MET_Egamma10NoTau_wet", 
		    &el_MET_Egamma10NoTau_wet); 
  fChain->SetBranch("el_MET_Egamma10NoTau_wpx",
		    &el_MET_Egamma10NoTau_wpx); 
  fChain->SetBranch("el_MET_Egamma10NoTau_wpy", 
		    &el_MET_Egamma10NoTau_wpy); 
  fChain->SetBranch("el_MET_Egamma10NoTau_statusWord", 
		    &el_MET_Egamma10NoTau_statusWord); 


  fChain->SetBranch("MET_Egamma10NoTau_CellOut_etx"   ,
		    &MET_Egamma10NoTau_CellOut_etx);  
  fChain->SetBranch("MET_Egamma10NoTau_CellOut_ety"   ,
		    &MET_Egamma10NoTau_CellOut_ety);    
  fChain->SetBranch("MET_Egamma10NoTau_CellOut_sumet" ,
		    &MET_Egamma10NoTau_CellOut_sumet);

  fChain->SetBranch("MET_Egamma10NoTau_CellOut_Eflow_STVF_etx",
		    &MET_CellOut_Eflow_STVF_etx);  
  fChain->SetBranch("MET_Egamma10NoTau_CellOut_Eflow_STVF_ety",
		    &MET_CellOut_Eflow_STVF_ety);  
  fChain->SetBranch("MET_Egamma10NoTau_CellOut_Eflow_STVF_sumet",
		    &MET_CellOut_Eflow_STVF_sumet);  

  fChain->SetBranch("MET_Egamma10NoTau_RefGamma_etx"  ,
		    &MET_Egamma10NoTau_RefGamma_etx);  
  fChain->SetBranch("MET_Egamma10NoTau_RefGamma_ety"  ,
		    &MET_Egamma10NoTau_RefGamma_ety);   
  fChain->SetBranch("MET_Egamma10NoTau_RefGamma_sumet",
		    &MET_Egamma10NoTau_RefGamma_sumet);

  fChain->SetBranch("MET_RefFinal_etx", &MET_RefFinal_etx); 
  fChain->SetBranch("MET_RefFinal_ety", &MET_RefFinal_ety); 

 
  fChain->SetBranch("el_n", &el_n); 
  fChain->SetBranch("el_author", &el_author); 
  fChain->SetBranch("el_OQ", &el_OQ); 
  fChain->SetBranch("el_mediumPP", &el_mediumPP); 
  fChain->SetBranch("el_tightPP", &el_tightPP); // for IsSignal
  fChain->SetBranch("el_ptcone20", &el_ptcone20); // for IsSignal
  fChain->SetBranch("el_trackd0pv", &el_trackd0pv); // for IsSignal
  fChain->SetBranch("el_trackz0pv", &el_trackz0pv); // for IsSignal
  fChain->SetBranch("el_charge", &el_charge); 
  fChain->SetBranch("el_cl_E", &el_cl_E); 
  fChain->SetBranch("el_cl_eta", &el_cl_eta); 
  fChain->SetBranch("el_cl_phi", &el_cl_phi); 
  fChain->SetBranch("el_cl_pt", &el_cl_pt); 
  fChain->SetBranch("el_trackphi", &el_trackphi); 
  fChain->SetBranch("el_tracketa", &el_tracketa); 
  fChain->SetBranch("el_nPixHits", &el_nPixHits); 
  fChain->SetBranch("el_nSCTHits", &el_nSCTHits); 
  fChain->SetBranch("mu_staco_n", &mu_staco_n); 
  fChain->SetBranch("mu_staco_E", &mu_staco_E); 
  fChain->SetBranch("mu_staco_pt", &mu_staco_pt); 
  fChain->SetBranch("mu_staco_eta", &mu_staco_eta); 
  fChain->SetBranch("mu_staco_phi", &mu_staco_phi); 
  fChain->SetBranch("mu_staco_ptcone20", &mu_staco_ptcone20); 
  fChain->SetBranch("mu_staco_charge", &mu_staco_charge); 
  fChain->SetBranch("mu_staco_isCombinedMuon", &mu_staco_isCombinedMuon); 
  fChain->SetBranch("mu_staco_isSegmentTaggedMuon", &mu_staco_isSegmentTaggedMuon); 
  fChain->SetBranch("mu_staco_loose", &mu_staco_loose); 
  fChain->SetBranch("mu_staco_id_theta_exPV", &mu_staco_id_theta_exPV); 
  fChain->SetBranch("mu_staco_id_qoverp_exPV", &mu_staco_id_qoverp_exPV); 
  fChain->SetBranch("mu_staco_me_theta_exPV", &mu_staco_me_theta_exPV); 
  fChain->SetBranch("mu_staco_me_qoverp_exPV", &mu_staco_me_qoverp_exPV); 
  fChain->SetBranch("mu_staco_ms_phi", &mu_staco_ms_phi); 
  fChain->SetBranch("mu_staco_ms_theta", &mu_staco_ms_theta); 
  fChain->SetBranch("mu_staco_ms_qoverp", &mu_staco_ms_qoverp); 
  fChain->SetBranch("mu_staco_id_theta", &mu_staco_id_theta); 
  fChain->SetBranch("mu_staco_nBLHits", &mu_staco_nBLHits); 
  fChain->SetBranch("mu_staco_nPixHits", &mu_staco_nPixHits); 
  fChain->SetBranch("mu_staco_nSCTHits", &mu_staco_nSCTHits); 
  fChain->SetBranch("mu_staco_nTRTHits", &mu_staco_nTRTHits); 
  fChain->SetBranch("mu_staco_nPixHoles", &mu_staco_nPixHoles); 
  fChain->SetBranch("mu_staco_nSCTHoles", &mu_staco_nSCTHoles); 
  fChain->SetBranch("mu_staco_nTRTOutliers", &mu_staco_nTRTOutliers); 
  fChain->SetBranch("mu_staco_nPixelDeadSensors", &mu_staco_nPixelDeadSensors); 
  fChain->SetBranch("mu_staco_nSCTDeadSensors", &mu_staco_nSCTDeadSensors); 
  fChain->SetBranch("mu_staco_expectBLayerHit", &mu_staco_expectBLayerHit); 

  fChain->SetBranch(jc + "_n", &jet_n); 
  fChain->SetBranch(jc + "_pt", &jet_pt); 
  fChain->SetBranch(jc + "_eta", &jet_eta); 
  fChain->SetBranch(jc + "_phi", &jet_phi); 
  fChain->SetBranch(jc + "_E", &jet_E); 
  fChain->SetBranch(jc + "_constscale_eta", &jet_constscale_eta); 
  fChain->SetBranch(jc + "_constscale_phi", &jet_constscale_phi); 
  fChain->SetBranch(jc + "_constscale_E",   &jet_constscale_E); 
  fChain->SetBranch(jc + "_constscale_m", &jet_constscale_m); 
  fChain->SetBranch(jc + "_ActiveAreaPx",   &jet_ActiveAreaPx); 
  fChain->SetBranch(jc + "_ActiveAreaPy",   &jet_ActiveAreaPy); 
  fChain->SetBranch(jc + "_ActiveAreaPz",   &jet_ActiveAreaPz); 
  fChain->SetBranch(jc + "_ActiveAreaE",   &jet_ActiveAreaE); 
  fChain->SetBranch(jc + "_emfrac", &jet_emfrac); 
  fChain->SetBranch(jc + "_hecf", &jet_hecf); 
  fChain->SetBranch(jc + "_LArQuality", &jet_LArQuality); 
  fChain->SetBranch(jc + "_HECQuality", &jet_HECQuality); 
  fChain->SetBranch(jc + "_AverageLArQF", &jet_AverageLArQF); 
  fChain->SetBranch(jc + "_Timing", &jet_Timing); 
  fChain->SetBranch(jc + "_sumPtTrk", &jet_sumPtTrk); 
  fChain->SetBranch(jc +"_fracSamplingMax", &jet_fracSamplingMax); 
  fChain->SetBranch(jc + "_SamplingMax", &jet_SamplingMax); 
  fChain->SetBranch(jc + "_NegativeE", &jet_NegativeE); 
  fChain->SetBranch(jc + "_flavor_weight_JetFitterCOMBNN", &jet_flavor_weight_JetFitterCOMBNN); 

  if (br & truth ) { 
    set_mc_branches(fChain, br, jc);
  }

  fChain->SetBranch(jc + "_flavor_component_jfitcomb_pu", 
		    &jet_flavor_component_jfitcomb_pu);
  fChain->SetBranch(jc + "_flavor_component_jfitcomb_pb", 
		    &jet_flavor_component_jfitcomb_pb);
  fChain->SetBranch(jc + "_flavor_component_jfitcomb_pc", 
		    &jet_flavor_component_jfitcomb_pc);
  fChain->SetBranch("vx_nTracks", &vx_nTracks); 

  if (br & mv3) { 
    fChain->SetBranch(jc + "_flavor_weight_MV3_bVSu", 
		      &jet_flavor_weight_MV3_bVSu); 
    fChain->SetBranch(jc + "_flavor_weight_MV3_bVSc", 
		      &jet_flavor_weight_MV3_bVSc); 
    fChain->SetBranch(jc + "_flavor_weight_MV3_cVSu", 
		      &jet_flavor_weight_MV3_cVSu); 
  }

  fChain->SetBranch(jc + "_flavor_component_jfitc_pu", 
		    &jet_flavor_component_jfitc_pu);
  fChain->SetBranch(jc + "_flavor_component_jfitc_pb", 
		      &jet_flavor_component_jfitc_pb);
  fChain->SetBranch(jc + "_flavor_component_jfitc_pc", 
		    &jet_flavor_component_jfitc_pc);

  if (br & unleash_sharktopus){  
    fChain->SetBranch("sharktopus",&sharktopus); 
  }

}

void SusyBuffer::set_mc_branches(SmartChain* chain, 
				 const unsigned br, 
				 std::string jc)
{
  using namespace cutflag; 
  chain->SetBranch(jc + "_flavor_truth_label", 
		     &jet_flavor_truth_label); 
  chain->SetBranch("mc_event_weight", &mc_event_weight); 

  // we can't use the mc_event_weight with sherpa tag 
  if (br & boson_pt_cut_70) { 
    // FIXME: these branches should be set for all MC, this is a temporary 
    //        fix while the skims run. 
    chain->SetBranch("mcevt_weight", &mcevt_weight); 
    chain->SetBranch("mc_n", &mc_n); 
    chain->SetBranch("mc_pt", &mc_pt); 
    chain->SetBranch("mc_eta", &mc_eta); 
    chain->SetBranch("mc_phi", &mc_phi); 
    chain->SetBranch("mc_m", &mc_m); 

    chain->SetBranch("mc_status", &mc_status); 
    chain->SetBranch("mc_pdgId", &mc_pdgId); 
  }
  
  if (br & spartid) { 
    chain->SetBranch("SUSY_Spart1_pdgId", &spart1_pdgid); 
    chain->SetBranch("SUSY_Spart2_pdgId", &spart2_pdgid); 
  }
}



