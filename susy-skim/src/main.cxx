#include "main.hh"
#include "susy.h"
#include <iostream>
#include <string> 

#include "SUSYTools/SUSYObjDef.h"

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


  for (int evt_n = 0; evt_n < 10; evt_n++) { 
    tree->GetEntry(evt_n); 
    
    def.Reset(); 

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
  
  def.Finalize(); 

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


