#include <stdexcept>

#include "SusyBuffer.h"
#include "SmartChain.hh"
#include "McParticleBuffer.hh"
#include "cutflag.hh"

SusyBuffer::SusyBuffer(SmartChain *fChain, const unsigned br)
{

#define SET(name) \
  fChain->SetBranch(#name, &name)

  using namespace cutflag; 

  std::string jc = "jet_AntiKt4LCTopo"; 

  fChain->SetBranchStatus("*",0); 

  SET(RunNumber);  
  SET(EventNumber); 
  SET(lbn); 

  // --- trigger branches ----
  // met
  fChain->SetBranch("EF_xe80_tclcw_tight", &xe80_tclcw_tight); 
  fChain->SetBranch("EF_xe80T_tclcw_loose", &xe80T_tclcw_loose); 
  fChain->SetBranch("EF_xe80_tclcw_loose", &xe80_tclcw_loose); 
  // muon
  SET(EF_mu18_tight_mu8_EFFS); 
  SET(EF_mu24i_tight);
  SET(EF_mu36_tight);
  // electron 
  SET(EF_2e12Tvh_loose1); 
  SET(EF_e24vhi_medium1); 
  SET(EF_e60_medium1); 

  SET(coreFlags); 
  
  fChain->SetBranch("top_hfor_type", &hfor_type); 

  fChain->SetBranch(jc + "_jvtxf", &jet_jvtxf); 
  SET(averageIntPerXing); 
  SET(larError); 
  SET(tileError); 

  SET(Eventshape_rhoKt4LC); 

  // MET garbage
  fChain->SetBranch(jc + "_MET_Egamma10NoTau_wet", &jet_MET_Egamma10NoTau_wet); 
  fChain->SetBranch(jc + "_MET_Egamma10NoTau_wpx", &jet_MET_Egamma10NoTau_wpx); 
  fChain->SetBranch(jc + "_MET_Egamma10NoTau_wpy", &jet_MET_Egamma10NoTau_wpy); 
  fChain->SetBranch(jc + "_MET_Egamma10NoTau_statusWord", 
		    &jet_MET_Egamma10NoTau_statusWord); 

  SET(el_MET_Egamma10NoTau_wet); 
  SET(el_MET_Egamma10NoTau_wpx); 
  SET(el_MET_Egamma10NoTau_wpy); 
  SET(el_MET_Egamma10NoTau_statusWord); 


  SET(MET_Egamma10NoTau_CellOut_etx);  
  SET(MET_Egamma10NoTau_CellOut_ety);    
  SET(MET_Egamma10NoTau_CellOut_sumet);

  fChain->SetBranch("MET_Egamma10NoTau_CellOut_Eflow_STVF_etx",
		    &MET_CellOut_Eflow_STVF_etx);  
  fChain->SetBranch("MET_Egamma10NoTau_CellOut_Eflow_STVF_ety",
		    &MET_CellOut_Eflow_STVF_ety);  
  fChain->SetBranch("MET_Egamma10NoTau_CellOut_Eflow_STVF_sumet",
		    &MET_CellOut_Eflow_STVF_sumet);  

  SET(MET_Egamma10NoTau_RefGamma_etx);  
  SET(MET_Egamma10NoTau_RefGamma_ety);   
  SET(MET_Egamma10NoTau_RefGamma_sumet);

 
  SET(el_n); 
  SET(el_eta); 
  SET(el_phi); 
  SET(el_author); 
  SET(el_OQ); 
  SET(el_mediumPP); 
  SET(el_tightPP); // for IsSignal
  SET(el_ptcone20); // for IsSignal
  SET(el_trackd0pv); // for IsSignal
  SET(el_trackz0pv); // for IsSignal
  SET(el_charge); 
  SET(el_cl_E); 
  SET(el_cl_eta); 
  SET(el_cl_phi); 
  // SET(el_cl_pt); 
  SET(el_trackphi); 
  SET(el_tracketa); 
  SET(el_nPixHits); 
  SET(el_nSCTHits); 
  SET(mu_staco_n); 
  SET(mu_staco_pt); 
  SET(mu_staco_eta); 
  SET(mu_staco_phi); 
  SET(mu_staco_ptcone20); 
  SET(mu_staco_charge); 
  SET(mu_staco_isCombinedMuon); 
  SET(mu_staco_isSegmentTaggedMuon); 
  SET(mu_staco_loose); 
  SET(mu_staco_id_theta_exPV); 
  SET(mu_staco_id_qoverp_exPV); 
  SET(mu_staco_me_theta_exPV); 
  SET(mu_staco_me_qoverp_exPV); 
  SET(mu_staco_ms_phi); 
  SET(mu_staco_ms_theta); 
  SET(mu_staco_ms_qoverp); 
  SET(mu_staco_id_theta); 
  SET(mu_staco_nPixHits); 
  SET(mu_staco_nSCTHits); 
  SET(mu_staco_nTRTHits); 
  SET(mu_staco_nPixHoles); 
  SET(mu_staco_nSCTHoles); 
  SET(mu_staco_nTRTOutliers); 
  SET(mu_staco_nPixelDeadSensors); 
  SET(mu_staco_nSCTDeadSensors); 
  SET(mu_staco_energyLossPar); 

  SET(mu_staco_qoverp_exPV); 
  SET(mu_staco_cov_qoverp_exPV); 
  SET(mu_staco_d0_exPV); 
  SET(mu_staco_z0_exPV); 

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
  fChain->SetBranch(jc + "_BCH_CORR_JET",  &jet_BCH_CORR_JET); 
  fChain->SetBranch(jc + "_emfrac", &jet_emfrac); 
  fChain->SetBranch(jc + "_hecf", &jet_hecf); 
  fChain->SetBranch(jc + "_LArQuality", &jet_LArQuality); 
  fChain->SetBranch(jc + "_HECQuality", &jet_HECQuality); 
  fChain->SetBranch(jc + "_AverageLArQF", &jet_AverageLArQF); 
  fChain->SetBranch(jc + "_Timing", &jet_Timing); 
  try { 
    fChain->SetBranch(jc + "_sumPtTrk", &jet_sumPtTrk); 
    fChain->fake_set(jc + "_sumPtTrk_pv0_500MeV"); // may need later
  } catch (const MissingBranchError& err) {	   
    fChain->SetBranch(jc + "_sumPtTrk_pv0_500MeV", &jet_sumPtTrk); 
  }
  
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
  SET(vx_nTracks); 

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
    SET(sharktopus); 
  }

  SET(trk_pt); 
  SET(trk_eta); 
  SET(trk_phi_wrtPV); 
  SET(trk_d0_wrtPV); 
  SET(trk_z0_wrtPV); 
  SET(trk_ndof); 
  SET(trk_chi2); 
  SET(trk_nPixHits); 
  SET(trk_nSCTHits); 

}

void SusyBuffer::set_mc_branches(SmartChain* chain, 
				 const unsigned br, 
				 std::string jc)
{
  using namespace cutflag; 
  chain->SetBranch("mc_channel_number", &mc_channel_number); 
  chain->SetBranch(jc + "_flavor_truth_label", 
		     &jet_flavor_truth_label); 
  try { 
    // in the skims we make this branch from mcevt_weight. 
    chain->SetBranch("skimmed_mcevt_weight", &mc_event_weight); 
  } catch (MissingBranchError& err) { 
    // for testing this branch is probably fine (never figured out whether
    // the supposed bug was fixed)
    chain->SetBranch("mc_event_weight", &mc_event_weight); 
  }

  mc_particles.set_branches(chain); 

  chain->SetBranch("MET_Truth_NonInt_etx", &MET_Truth_NonInt_etx); 
  chain->SetBranch("MET_Truth_NonInt_ety", &MET_Truth_NonInt_ety);

  if (br & spartid) { 
    chain->SetBranch("SUSY_Spart1_pdgId", &spart1_pdgid); 
    chain->SetBranch("SUSY_Spart2_pdgId", &spart2_pdgid); 
  } else { 
    chain->fake_set("SUSY_Spart1_pdgId"); 
    chain->fake_set("SUSY_Spart2_pdgId"); 
  }
}

#undef SET


