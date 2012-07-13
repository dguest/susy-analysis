#include "main.hh"
#include "susy.h"
#include <iostream>
#include <string> 
#include <cassert>
#include <map>
#include "TChain.h"
#include "TVector2.h"
#include <math.h>
#include "SUSYTools/SUSYObjDef.h"
#include "SUSYTools/FakeMetEstimator.h"

/* NOTES:

need to get proper condition for first trigger statement (not isSignal)
remove isSignal = true

el_wet  set to zero, not passed properly

where Jan used jetsEMscale, we just have baseline_jets container
    (may need to fix this later)

all the cuts have been negated now, for the continue statement logic

putting off the badz0wrtPVmuon (superfluous at the moment, not easy to implement)

DONE: m_combNN_btag_wt = 0; 	// TODO: find this branch and set it  

run number is set, why?

 */

int main (int narg, char* argv[])
{
  cout<< "in main" << endl;

  
  TChain* input_chain = 0; 
  if (narg > 1) { 
    cout << "if command line file inpt" << endl;
    input_chain = new TChain("susy"); 
    for (int n = 1; n < narg; n++) { 
      
      input_chain->Add(argv[n]); 
    }
  }

  FakeMetEstimator fakeMetEst;

  SUSYObjDef def; 
  RunInfo info; 
  info.is_data = false; 
  info.run_number = 180614; 

  susy buffer(input_chain); 
  TTree* tree = buffer.fChain; 
  int n_entries = tree->GetEntriesFast(); 
  std::cout << n_entries << " in chain" << std::endl; 

  def.initialize(true); 
  int counter = 0;

  std::map<std::string, int> cut_counters; 
  cut_counters["10_events"]=0;
  cut_counters["11_lar_error"] = 0; 
  cut_counters["12_lar_hole_veto"]=0;
  cut_counters["13_badjet"]=0;
  cut_counters["14_GoodPV"]=0;
  cut_counters["15_ishfor"]=0;
  cut_counters["16_trigger"]=0;
  cut_counters["17_UsedJetsSize"]=0;
  cut_counters["18_UsedJetsPt"]=0;
  cut_counters["19_MET"]=0;
  cut_counters["20_goodEl"]=0;
  cut_counters["21_goodMu"]=0;
  cut_counters["22_deltaPhi"]=0;
  cut_counters["23_ctag"]=0;

  // looping through events

  

  for (int evt_n = 0; evt_n < 10; evt_n++) { 
    cut_counters["10_events"]++;
    tree->GetEntry(evt_n); 

    
    def.Reset(); 
    
   bool trigger=false;
   bool isSignal=false;
      
   //if(basename2.Contains("Stop_") && basename2.Contains("private") && basename2.Contains("cl_xqcut")){
   if(isSignal){
     trigger=false;
     gRandom= new TRandom3(0);
  
     //We have no runnumbers in our Stop-samples, so we create a random number to determine what trigger is being used for which event (depending on the luminosities of the periods and the used triggers) 
     // EF_xe70_noMu: perdiod B-K
     // EF_xe60_verytight_noMu: period L-M
  
     float rndnr=gRandom->Rndm(0)*4689.68;
     //cout << rndnr << endl;
     if(rndnr>2281.26){
       if(buffer.EF_xe70_noMu) trigger=true;
     }
     else{
       if(buffer.EF_xe60_verytight_noMu) trigger=true;
     }
   }else{
     trigger=false;
     if(buffer.RunNumber<=187815 && buffer.EF_xe70_noMu) trigger=true;
     if(buffer.RunNumber>187815 && buffer.RunNumber<=191933&& buffer.EF_xe60_verytight_noMu) trigger=true;
   }
   
   
   // event preselection 
   bool lar_error = buffer.larError; 
   bool ishforveto = false;


   const int n_jets = buffer.jet_AntiKt4TopoNewEM_n; 
   const int n_el = buffer.el_n;
   const int n_mu = buffer.mu_staco_n; // I'm pretty sure we're using staco
 
   TVector2 met(buffer.MET_Simplified20_RefFinal_etx, 
		buffer.MET_Simplified20_RefFinal_ety); 

  
   
   
   std::vector<BaselineJet> baseline_jets; 
   
   bool badjet_loose = false;

   for (int jet_n = 0; jet_n < n_jets; jet_n++){ 
     // jet preselection 
     // bool lar_hole_veto = check_lar_hole_veto(jet_n, buffer, def, info); 
     bool is_jet = check_if_jet(jet_n, buffer, def, info); 
     // ... fill jets here 

     if(!is_jet) badjet_loose = true;

     //this is where the jet is built 
     baseline_jets.push_back(BaselineJet(buffer, jet_n)); 
   }

   
   // unless we start doing something with them we can just count the good el
   int n_good_electrons = 0; 
   for (int el_n = 0; el_n < n_el; el_n++){
     bool isElectron = check_if_electron(el_n, buffer, def, info);
     if (isElectron) n_good_electrons++; 
   }

   int n_good_muons = 0; 
   for (int mu_n = 0; mu_n < n_mu; mu_n++){

     /*part of the badz0wrtPVmuon cut - complete later if needed
     muon.isCosmic = def.IsCosmicMuon(mu_staco_z0_exPV->at(mu_n),
				      mu_staco_d0_exPV->at(mu_n),
				      5.,
				      2.); */

     bool isMuon = check_if_muon(mu_n, buffer, def, info);
     if (isMuon) n_good_muons++; 
   }



  
    
   //ishforveto cut setup 
     if(!info.is_data){
       if (buffer.top_hfor_type==4)  ishforveto = true;

     }
     
     
  
     

     if(lar_error)
     continue;
     cut_counters["11_lar_error"]++; 
     
     
     if(IsSmartLArHoleVeto( met,
			    fakeMetEst,
			    buffer, 
			    def, 
			    baseline_jets))
     continue;     
     cut_counters["12_lar_hole_veto"]++;
  
    

     if(badjet_loose) 
     continue;
     cut_counters["13_badjet"]++;
  


    // if(badz0wrtPVmuon)  to be completed
    // continue;
       
     if(!def.IsGoodVertex(buffer.vx_nTracks))
     continue;
     cut_counters["14_GoodPV"]++;
  
	 
     if(ishforveto) 
     continue;
     cut_counters["15_ishfor"]++;
  
     
    
     
     
     if(!trigger)
     continue;
     cut_counters["16_trigger"]++;
  
     
     if(baseline_jets.size()<3)
     continue;
     cut_counters["17_UsedJetsSize"]++;
 


     bool jetAbove150 = false;
     //could move this for loop up with the others. cleaner?
     for (int i=0;i<baseline_jets.size();i++){
       if(baseline_jets.at(i).Pt() > 150) jetAbove150 = true;
     }
     
     if(!jetAbove150)
     continue;
     cut_counters["18_UsedJetsPt"]++;
		


     //correct implementation? 
     if(met.Mod()<=150)
     continue;
     cut_counters["19_MET"]++;

     //no electrons
     if(n_good_electrons>=1)
     continue;
     cut_counters["20_goodEl"]++;

     //no muons
     if(n_good_muons>=1)
     continue;
     cut_counters["21_goodMu"]++;

    
     //For DeltaPhi cut
     TLorentzVector sumjets; 
     for(int i=0;i<baseline_jets.size();i++){
     
     sumjets+= baseline_jets.at(i);


     }

     double sumPhi = sumjets.Phi();
     double metPhi = met.Phi();

     double delta = fabs(met.Phi() - sumjets.Phi());
     if(delta > M_PI)    delta = fabs(delta - 2*M_PI);

     if(delta < 0.4)
     continue;
     cut_counters["22_deltaPhi"]++;
     
    

     //ctag > 2 cut
     int ctagJets = 0;
     for (int i=0;i<baseline_jets.size();i++){
     
     if(baseline_jets.at(i).combNN_btag() > -2 
     && baseline_jets.at(i).combNN_btag() < 4)
     ctagJets++;
     
     } 

     if(ctagJets<2)
     continue;
     cut_counters["23_ctag"]++;

     
     
     
   
   

	   

     /*   std::cout << buffer.el_n << " electrons in event " << evt_n
		     << std::endl; 
	   std::cout << "in crack? " << (def.IsInCrack(0.4) ? "yes":"no" )
	   << std::endl; 
*/
 
  }

  for (std::map<std::string, int>::const_iterator 
	 cut_itr = cut_counters.begin(); 
       cut_itr != cut_counters.end(); 
       cut_itr++) { 
    std::cout << cut_itr->second << " events pass cut " 
	      << cut_itr->first << std::endl; 
  }
       

   def.finalize(); 
	 
	 return 0;
	 
}
  
bool IsSmartLArHoleVeto(TVector2 met,FakeMetEstimator& fakeMetEst,const susy& buffer, SUSYObjDef& def, std::vector<BaselineJet> baseline_jets ) {

  bool isVeto=false; 
  
  for (unsigned int j = 0; j<baseline_jets.size(); j++)
    {

      if(baseline_jets.at(j).Pt()<= 20.) continue;
      if(def.IsLArHole(baseline_jets.at(j).Eta(),
		       baseline_jets.at(j).Phi()))
	{

	  int d3pd_index = baseline_jets.at(j).jet_index();
	  //use jet pT after JES/JER
	  if(fakeMetEst.isBad(buffer.jet_AntiKt4TopoNewEM_pt            ->at(d3pd_index),
			      buffer.jet_AntiKt4TopoNewEM_BCH_CORR_JET  ->at(d3pd_index),
			      buffer.jet_AntiKt4TopoNewEM_BCH_CORR_CELL ->at(d3pd_index),
			      buffer.jet_AntiKt4TopoNewEM_BCH_CORR_DOTX ->at(d3pd_index),
			      buffer.jet_AntiKt4TopoNewEM_phi           ->at(d3pd_index),
			      met.Px()*1e3,
			      met.Py()*1e3,
			      10000.,
			      -1.,
			      -1.)


			      /* baseline_jets.at(j).Pt()*1e3,
			      baseline_jets.at(j).BCH_CORR_JET,
			      baseline_jets.at(j).BCH_CORR_CELL,
			      baseline_jets.at(j).BCH_CORR_DOTX,
			      baseline_jets.at(j).Phi(),
			      MET.Px()*1e3,
			      MET.Py()*1e3,
			      10000.,
			      10.,
			      -1.,
			      -1.)
			      */
	     ) {

	    isVeto = true;
	    break;

	  }

	}

    }

  return isVeto;

}
	  




/*bool check_lar_hole_veto(int jet_n, const susy& buffer, SUSYObjDef& def, 
			 const RunInfo& info ) { 
  int n = jet_n; 
  bool veto = def.IsLArHoleVeto
    (buffer.jet_AntiKt4TopoNewEM_pt->at(n), 
     buffer.jet_AntiKt4TopoNewEM_eta->at(n), 
     buffer.jet_AntiKt4TopoNewEM_phi->at(n),
     buffer.jet_AntiKt4TopoNewEM_BCH_CORR_JET->at(n), 
     buffer.jet_AntiKt4TopoNewEM_BCH_CORR_CELL->at(n), 
     info.is_data, 
     40000., // pt cut, not sure why needed...
     info.run_number);
  if (veto) return true; 
  return false; 
}

*/


/*TVector2 get_MET(const susy& buffer, SUSYObjDef& def, const RunInfo& info){
  std::vector<int> dummy; 
  return def.GetMET
    (buffer.jet_AntiKt4TopoNewEM_pt, // jacknife says this works... 
     buffer.jet_AntiKt4TopoNewEM_MET_Simplified20_wet,
     buffer.jet_AntiKt4TopoNewEM_MET_Simplified20_wpx,
     buffer.jet_AntiKt4TopoNewEM_MET_Simplified20_wpy,
     buffer.jet_AntiKt4TopoNewEM_MET_Simplified20_statusWord,
     dummy, 		// hack
     buffer.el_MET_Simplified20_wet,
     buffer.el_MET_Simplified20_wpx,
     buffer.el_MET_Simplified20_wpy,
     buffer.el_MET_Simplified20_statusWord,
     buffer.MET_SoftJets_etx,
     buffer.MET_SoftJets_ety,
     buffer.MET_SoftJets_sumet,
     buffer.MET_CellOut_etx,
     buffer.MET_CellOut_ety,
     buffer.MET_CellOut_sumet,
     buffer.MET_CellOut_Eflow_etx,
     buffer.MET_CellOut_Eflow_ety,
     buffer.MET_CellOut_Eflow_sumet,
     buffer.MET_RefGamma_em_etx,
     buffer.MET_RefGamma_em_ety,
     buffer.MET_RefGamma_em_sumet,
     dummy, 		// hack
     buffer.mu_staco_ms_qoverp,
     buffer.mu_staco_ms_theta,
     buffer.mu_staco_ms_phi,
     buffer.mu_staco_charge,
     SystErr::NONE);
}

*/

bool check_if_electron(int iEl,
		       const susy& buffer,
		       SUSYObjDef& def,
		       const RunInfo& info){
  return def.IsElectron
    (iEl,
     info.run_number,
     buffer.el_cl_eta                ->at(iEl),
     buffer.el_cl_phi                ->at(iEl),
     buffer.el_cl_E                  ->at(iEl),
     buffer.el_tracketa              ->at(iEl),
     buffer.el_trackphi              ->at(iEl),
     buffer.el_author                ->at(iEl),
     buffer.el_mediumPP              ->at(iEl),
     buffer.el_OQ                    ->at(iEl),
     buffer.el_nPixHits              ->at(iEl),
     buffer.el_nSCTHits              ->at(iEl),
     // buffer.el_weta2                 ->at(iEl),
     0,
     info.is_data,
     20000.,                         //et cut
     2.47,                          //eta cut
     SystErr::NONE);
     
  //info.isAF2
  //NOTE: el_wet set to zero, couldn't match variable in D3PD
}

bool check_if_muon(int iMu,
		   const susy& buffer,
		   SUSYObjDef& def,
		   const RunInfo& info){

  return def.IsMuon
    (iMu,
     buffer.mu_staco_pt                           ->at(iMu),
     buffer.mu_staco_eta                          ->at(iMu),
     buffer.mu_staco_phi                          ->at(iMu),
     buffer.mu_staco_E                            ->at(iMu),
     buffer.mu_staco_me_qoverp_exPV               ->at(iMu),
     buffer.mu_staco_id_qoverp_exPV               ->at(iMu),
     buffer.mu_staco_me_theta_exPV                ->at(iMu),
     buffer.mu_staco_id_theta_exPV                ->at(iMu),
     buffer.mu_staco_id_theta                     ->at(iMu),
     buffer.mu_staco_isCombinedMuon               ->at(iMu),
     buffer.mu_staco_isSegmentTaggedMuon          ->at(iMu),
     buffer.mu_staco_loose                        ->at(iMu),
     buffer.mu_staco_expectBLayerHit              ->at(iMu),
     buffer.mu_staco_nBLHits                      ->at(iMu),
     buffer.mu_staco_nPixHits                     ->at(iMu),
     buffer.mu_staco_nPixelDeadSensors            ->at(iMu),
     buffer.mu_staco_nPixHoles                    ->at(iMu),
     buffer.mu_staco_nSCTHits                     ->at(iMu),
     buffer.mu_staco_nSCTDeadSensors              ->at(iMu),
     buffer.mu_staco_nSCTHoles                    ->at(iMu),
     buffer.mu_staco_nTRTHits                     ->at(iMu),
     buffer.mu_staco_nTRTOutliers                 ->at(iMu),
     info.is_data,
     10000.,                      //pt cut
     2.4,                         //eta cut
     SystErr::NONE);


}

  
bool check_if_jet(int iJet, 
		  const susy& buffer, 
		  SUSYObjDef& def, 
		  const RunInfo& info){ 
    
  return def.IsJet
    (iJet, 
     buffer.jet_AntiKt4TopoNewEM_pt                 ->at(iJet), 
     buffer.jet_AntiKt4TopoNewEM_eta                ->at(iJet), 
     buffer.jet_AntiKt4TopoNewEM_phi                ->at(iJet),
     buffer.jet_AntiKt4TopoNewEM_E                  ->at(iJet), 
     buffer.jet_AntiKt4TopoNewEM_emscale_eta        ->at(iJet), 
     buffer.jet_AntiKt4TopoNewEM_emfrac             ->at(iJet), 
     buffer.jet_AntiKt4TopoNewEM_hecf               ->at(iJet),
     buffer.jet_AntiKt4TopoNewEM_LArQuality         ->at(iJet), 
     buffer.jet_AntiKt4TopoNewEM_HECQuality         ->at(iJet), 
     buffer.jet_AntiKt4TopoNewEM_AverageLArQF       ->at(iJet), 
     buffer.jet_AntiKt4TopoNewEM_Timing             ->at(iJet), 
     buffer.jet_AntiKt4TopoNewEM_sumPtTrk           ->at(iJet), 
     buffer.jet_AntiKt4TopoNewEM_fracSamplingMax    ->at(iJet),
     buffer.jet_AntiKt4TopoNewEM_NegativeE          ->at(iJet), 
     buffer.jet_AntiKt4TopoNewEM_flavor_truth_label ->at(iJet), 
     buffer.jet_AntiKt4TopoNewEM_emscale_E          ->at(iJet),
     buffer.jet_AntiKt4TopoNewEM_emscale_eta        ->at(iJet), 
     buffer.jet_AntiKt4TopoNewEM_EtaOrigin          ->at(iJet), 
     buffer.jet_AntiKt4TopoNewEM_PhiOrigin          ->at(iJet), 
     buffer.jet_AntiKt4TopoNewEM_MOrigin            ->at(iJet), 
     buffer.averageIntPerXing,
     buffer.vx_nTracks,             
     info.is_data, 
     20000., 			// pt cut
     2.8,			// eta cut
     JetID::LooseBad);
    
}
  
  
  
BaselineJet::BaselineJet(const susy& buffer, int jet_index) { 
  double pt = buffer.jet_AntiKt4TopoNewEM_pt  ->at(jet_index); 
  double eta = buffer.jet_AntiKt4TopoNewEM_eta ->at(jet_index); 
  double phi = buffer.jet_AntiKt4TopoNewEM_phi ->at(jet_index); 
  double e = buffer.jet_AntiKt4TopoNewEM_E   ->at(jet_index); 
  SetPtEtaPhiE(pt,eta,phi,e); 
  m_jet_index = jet_index;
    
  m_combNN_btag_wt = buffer.jet_AntiKt4TopoNewEM_flavor_weight_JetFitterCOMBNN ->at(jet_index); 	// TODO: find this branch and set it
}
  
double BaselineJet::combNN_btag(){ 
  //std::cerr << "ERROR: this isn't defined yet\n"; 
  //assert(false); 

  return m_combNN_btag_wt;
}
  
  
int BaselineJet::jet_index(){

  return m_jet_index;

}
