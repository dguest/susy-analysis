#include "main.hh"
#include "susy.h"
#include <iostream>
#include <string> 
#include <cassert>

#include "SUSYTools/SUSYObjDef.h"

/* NOTES:

need to get proper condition for first trigger statement (not isSignal)
remove isSignal = true

find applicable variable names for the trigger/random number portion


 */

int main (int narg, char* argv[])
{

  SUSYObjDef def; 
  RunInfo info; 
  info.is_data = false; 
  info.run_number = 180614; 

  susy buffer; 
  TTree* tree = buffer.fChain; 
  int n_entries = tree->GetEntriesFast(); 
  std::cout << n_entries << " in chain" << std::endl; 

  def.initialize(true); 

  // looping through events
  for (int evt_n = 0; evt_n < 10; evt_n++) { 
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
   
   const int n_jets = buffer.jet_AntiKt4TopoNewEM_n; 
   
   std::vector<BaselineJet> baseline_jets; 
   
   for (int jet_n = 0; jet_n < n_jets; jet_n++){ 
     // jet preselection 
     bool lar_hole_veto = check_lar_hole_veto(jet_n, buffer, def, info); 

     
     bool is_jet = check_if_jet(jet_n, buffer, def, info); 

     /* 

     if(lar_hole_veto){
       

       //do i need to define this, like the two other booleans?
       if(IsGoodVertex){
	 

       if(is_jet){
	 

	 if(trigger){
	   

	   if(baseline_jets.size()>=3){

	     for (int i=0;i<baseline_jets.size();i++){
	       if(baseline_jets.at(i).Pt() > 150){
		 
		 //fix the Etmiss one
		 if(Etmiss.Pt()>150){

		   //how do i get the size of goodmuons?
		   //don't have a container for muons as of yet...??
		   //   if (IsMuon
		       
		       
		 }
	       }
	     }
	   }
	 }
       }
       }
     }

     */ 
     
     //ACHTUNG Ariel! this is where the jet is built 
     baseline_jets.push_back(BaselineJet(buffer, jet_n)); 
     
   
   

	   

	   std::cout << buffer.el_n << " electrons in event " << evt_n
		     << std::endl; 
	   std::cout << "in crack? " << (def.IsInCrack(0.4) ? "yes":"no" )
		     << std::endl;
   }
  }
   def.finalize(); 
	 
	 return 0;
  
}
  
bool check_lar_hole_veto(int jet_n, const susy& buffer, SUSYObjDef& def, 
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
  
TVector2 get_MET(const susy& buffer, SUSYObjDef& def, const RunInfo& info){
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
    
  m_combNN_btag_wt = 0; 	// TODO: find this branch and set it
}
  
double BaselineJet::combNN_btag(){ 
  std::cerr << "ERROR: this isn't defined yet\n"; 
  assert(false); 
}
  
  
