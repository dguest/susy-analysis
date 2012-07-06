#include "main.hh"
#include "susy.h"
#include <iostream>
#include <string> 

#include "SUSYTools/SUSYObjDef.h"

/* NOTES:

need to get proper condition for first trigger statement (not isSignal)
remove isSignal = true


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
   if(isSignal)
     trigger=false;
     gRandom= new TRandom3(0);
  
     //We have no runnumbers in our Stop-samples, so we create a random number to determine what trigger is being used for which event (depending on the luminosities of the periods and the used triggers) 
     // EF_xe70_noMu: perdiod B-K
     // EF_xe60_verytight_noMu: period L-M
  
     float rndnr=gRandom->Rndm(0)*4689.68;
     //cout << rndnr << endl;
     if(rndnr>2281.26){
       if(EF_xe70_noMu) trigger=true;
     }
     else{
       if(EF_xe60_verytight_noMu) trigger=true;
     }
   }else{
     trigger=false;
     if(RunNumber<=187815 && EF_xe70_noMu) trigger=true;
     if(RunNumber>187815 && RunNumber<=191933&& EF_xe60_verytight_noMu) trigger=true;
   }
   
   // event preselection 
   bool lar_error = buffer.larError; 
   
   const int n_jets = buffer.jet_AntiKt4TopoNewEM_n; 
   
   for (int jet_n = 0; jet_n < n_jets; jet_n++){ 
     // jet preselection 
     bool lar_hole_veto = check_lar_hole_veto(jet_n, buffer, def, info); 
     
     bool is_jet = check_if_jet(jet_n, buffer, def, info); 
     
   }
   

    std::cout << buffer.el_n << " electrons in event " << evt_n
	      << std::endl; 
    std::cout << "in crack? " << (def.IsInCrack(0.4) ? "yes":"no" )
	      << std::endl;
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


