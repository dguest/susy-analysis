#include <stdexcept>

#include "SusyBuffer.h"
#include "SmartChain.hh"
#include "McParticleBuffer.hh"
#include "cutflag.hh"

// branch setting macro, nothing too fancy
#define SET(ch,name)				\
  ch->SetBranch(#name, &name)

// jet branch setting macro. A bit more fancy (assumes a variable called 
// jet_NAME exists)
#define JET_SET(ch,NAME)			\
  ch->SetBranch(jc + "_" #NAME, &jet_ ## NAME)

SusyBuffer::SusyBuffer(SmartChain *fChain, const unsigned br): 
  m_has_truth(false)
{

  using namespace cutflag; 

  std::string jc = "jet_AntiKt4LCTopo"; 


  fChain->SetBranchStatus("*",0); 

  SET(fChain,RunNumber);  
  SET(fChain,EventNumber); 
  SET(fChain,lbn); 

  // --- trigger branches ----
  // met
  SET(fChain, EF_xe80_tclcw_tight);
  SET(fChain, EF_xe80T_tclcw_loose);
  try { 
    // this thing is sometimes missing in the Egamma stream... 
    // doesn't really matter there (since we're not using met triggers)
    SET(fChain, EF_xe80_tclcw_loose);
  } catch (MissingBranchError& err) { 
    EF_xe80_tclcw_loose = false;
  }
  // muon
  SET(fChain,EF_mu18_tight_mu8_EFFS); 
  SET(fChain,EF_mu24i_tight);
  SET(fChain,EF_mu36_tight);
  // electron 
  SET(fChain,EF_2e12Tvh_loose1); 
  SET(fChain,EF_e24vhi_medium1); 
  SET(fChain,EF_e60_medium1); 

  SET(fChain,coreFlags); 
  
  fChain->SetBranch("top_hfor_type", &hfor_type); 

  JET_SET(fChain,jvtxf); 
  SET(fChain,averageIntPerXing); 
  SET(fChain,larError); 
  SET(fChain,tileError); 

  SET(fChain,Eventshape_rhoKt4LC); 

  // MET garbage

  JET_SET(fChain,MET_Egamma10NoTau_wet); 
  
  JET_SET(fChain,MET_Egamma10NoTau_wpx); 
  JET_SET(fChain,MET_Egamma10NoTau_wpy);
  JET_SET(fChain,MET_Egamma10NoTau_statusWord);

  SET(fChain,el_MET_Egamma10NoTau_wet); 
  SET(fChain,el_MET_Egamma10NoTau_wpx); 
  SET(fChain,el_MET_Egamma10NoTau_wpy); 
  SET(fChain,el_MET_Egamma10NoTau_statusWord); 

  SET(fChain,MET_Egamma10NoTau_CellOut_etx);  
  SET(fChain,MET_Egamma10NoTau_CellOut_ety);    
  SET(fChain,MET_Egamma10NoTau_CellOut_sumet);

  fChain->SetBranch("MET_Egamma10NoTau_CellOut_Eflow_STVF_etx",
		    &MET_CellOut_Eflow_STVF_etx);  
  fChain->SetBranch("MET_Egamma10NoTau_CellOut_Eflow_STVF_ety",
		    &MET_CellOut_Eflow_STVF_ety);  
  fChain->SetBranch("MET_Egamma10NoTau_CellOut_Eflow_STVF_sumet",
		    &MET_CellOut_Eflow_STVF_sumet);  

  SET(fChain,MET_Egamma10NoTau_RefGamma_etx);  
  SET(fChain,MET_Egamma10NoTau_RefGamma_ety);   
  SET(fChain,MET_Egamma10NoTau_RefGamma_sumet);
 
  SET(fChain,el_n); 
  SET(fChain,el_eta); 
  SET(fChain,el_phi); 
  SET(fChain,el_author); 
  SET(fChain,el_OQ); 
  SET(fChain,el_mediumPP); 
  SET(fChain,el_tightPP); // for IsSignal
  SET(fChain,el_ptcone20); // for IsSignal
  // ACHTUNG: these were removed because of a bad skim...
  // SET(fChain,el_trackd0pv); // for IsSignal
  // SET(fChain,el_trackz0pv); // for IsSignal
  SET(fChain,el_charge); 
  SET(fChain,el_cl_E); 
  SET(fChain,el_cl_eta); 
  SET(fChain,el_cl_phi); 
  // SET(fChain,el_cl_pt); 
  SET(fChain,el_trackphi); 
  SET(fChain,el_tracketa); 
  SET(fChain,el_nPixHits); 
  SET(fChain,el_nSCTHits); 
  SET(fChain,mu_staco_n); 
  SET(fChain,mu_staco_pt); 
  SET(fChain,mu_staco_eta); 
  SET(fChain,mu_staco_phi); 
  SET(fChain,mu_staco_ptcone20); 
  SET(fChain,mu_staco_charge); 
  SET(fChain,mu_staco_isCombinedMuon); 
  SET(fChain,mu_staco_isSegmentTaggedMuon); 
  SET(fChain,mu_staco_loose); 
  SET(fChain,mu_staco_id_theta_exPV); 
  SET(fChain,mu_staco_id_qoverp_exPV); 
  SET(fChain,mu_staco_me_theta_exPV); 
  SET(fChain,mu_staco_me_qoverp_exPV); 
  SET(fChain,mu_staco_ms_phi); 
  SET(fChain,mu_staco_ms_theta); 
  SET(fChain,mu_staco_ms_qoverp); 
  SET(fChain,mu_staco_id_theta); 
  SET(fChain,mu_staco_nPixHits); 
  SET(fChain,mu_staco_nSCTHits); 
  SET(fChain,mu_staco_nTRTHits); 
  SET(fChain,mu_staco_nPixHoles); 
  SET(fChain,mu_staco_nSCTHoles); 
  SET(fChain,mu_staco_nTRTOutliers); 
  SET(fChain,mu_staco_nPixelDeadSensors); 
  SET(fChain,mu_staco_nSCTDeadSensors); 
  SET(fChain,mu_staco_energyLossPar); 

  SET(fChain,mu_staco_qoverp_exPV); 
  SET(fChain,mu_staco_cov_qoverp_exPV); 
  SET(fChain,mu_staco_d0_exPV); 
  SET(fChain,mu_staco_z0_exPV); 

  JET_SET(fChain,n); 
  JET_SET(fChain,pt);
  JET_SET(fChain,eta);
  JET_SET(fChain,phi);
  JET_SET(fChain,E);
  JET_SET(fChain,constscale_eta);
  JET_SET(fChain,constscale_phi);
  JET_SET(fChain,constscale_E); 
  JET_SET(fChain,constscale_m);
  JET_SET(fChain,ActiveAreaPx); 
  JET_SET(fChain,ActiveAreaPy); 
  JET_SET(fChain,ActiveAreaPz); 
  JET_SET(fChain,ActiveAreaE);
  JET_SET(fChain,BCH_CORR_JET);
  JET_SET(fChain,emfrac); 
  JET_SET(fChain,hecf); 
  JET_SET(fChain,LArQuality); 
  JET_SET(fChain,HECQuality); 
  JET_SET(fChain,AverageLArQF); 
  JET_SET(fChain,Timing);
  try { 
    fChain->SetBranch(jc + "_sumPtTrk", &jet_sumPtTrk); 
    fChain->fake_set(jc + "_sumPtTrk_pv0_500MeV"); // may need later
  } catch (const MissingBranchError& err) {	   
    fChain->SetBranch(jc + "_sumPtTrk_pv0_500MeV", &jet_sumPtTrk); 
  }
  
  JET_SET(fChain,fracSamplingMax);
  JET_SET(fChain,SamplingMax);
  JET_SET(fChain,NegativeE);
  JET_SET(fChain,flavor_weight_JetFitterCOMBNN);

  if (br & truth ) { 
    set_mc_branches(fChain, br, jc);
  }

  JET_SET(fChain,flavor_component_jfitcomb_pu); 
  JET_SET(fChain,flavor_component_jfitcomb_pb); 
  JET_SET(fChain,flavor_component_jfitcomb_pc); 
  JET_SET(fChain,flavor_component_jfitc_pu); 
  JET_SET(fChain,flavor_component_jfitc_pb); 
  JET_SET(fChain,flavor_component_jfitc_pc); 

  SET(fChain,vx_nTracks); 

  if (br & unleash_sharktopus){  
    SET(fChain,sharktopus); 
  }

  SET(fChain,trk_pt); 
  SET(fChain,trk_eta); 
  SET(fChain,trk_phi_wrtPV); 
  SET(fChain,trk_d0_wrtPV); 
  SET(fChain,trk_z0_wrtPV); 
  SET(fChain,trk_ndof); 
  SET(fChain,trk_chi2); 
  SET(fChain,trk_nPixHits); 
  SET(fChain,trk_nSCTHits); 

}

void SusyBuffer::set_mc_branches(SmartChain* chain, 
				 const unsigned br, 
				 std::string jc)
{
  using namespace cutflag; 
  SET(chain,mc_channel_number);
  JET_SET(chain,flavor_truth_label); 

  try { 
    // in the skims we make this branch from mcevt_weight. 
    chain->SetBranch("skimmed_mcevt_weight", &mc_event_weight); 
  } catch (MissingBranchError& err) { 
    // for testing this branch is probably fine (never figured out whether
    // the supposed bug was fixed)
    chain->SetBranch("mc_event_weight", &mc_event_weight); 
  }

  mc_particles.set_branches(chain); 

  SET(chain,MET_Truth_NonInt_etx); 
  SET(chain,MET_Truth_NonInt_ety);

  if (br & spartid) { 
    chain->SetBranch("SUSY_Spart1_pdgId", &spart1_pdgid); 
    chain->SetBranch("SUSY_Spart2_pdgId", &spart2_pdgid); 
  } else { 
    chain->fake_set("SUSY_Spart1_pdgId"); 
    chain->fake_set("SUSY_Spart2_pdgId"); 
  }
  m_has_truth = true; 
}

bool SusyBuffer::has_truth() const { return m_has_truth; }


#undef SET
#undef JET_SET



