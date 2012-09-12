#include "cutflow.hh"
#include "susy.h"
#include <iostream>
#include <fstream>
#include <string> 
#include <streambuf>
#include <cassert>
#include <cstdlib>
#include <map>
#include "TChain.h"
#include "TVector2.h"
#include <math.h>
#include <stdexcept> 
#include "SUSYTools/SUSYObjDef.h"
#include "SUSYTools/FakeMetEstimator.h"

#include <boost/format.hpp>

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

  std::vector<std::string> input_files; 
  for (int n = 1; n < narg; n++) { 
    input_files.push_back(argv[n]); 
  }

  RunInfo info; 
  info.is_data = false; 
  info.run_number = 180614; 
  info.is_signal = true; 
  srand(0); 

  // run the main routine 
  std::map<std::string, int> cut_counters = run_cutflow(input_files, info); 

  // output cutflow to file
  std::ofstream cutflow_textfile("cutflow.txt"); 

  float firstcut = cut_counters["00_events"]; 
  
  for (std::map<std::string, int>::const_iterator 
	 cut_itr = cut_counters.begin(); 
       cut_itr != cut_counters.end(); 
       cut_itr++) { 
    std::cout << boost::format("%i events pass %s (%.2f%%)\n") % 
      cut_itr->second % cut_itr->first % 
      ( float(cut_itr->second) * 100 / firstcut) ; 
    cutflow_textfile << cut_itr->second << " events pass cut " 
		     << cut_itr->first << std::endl; 

  }

  cutflow_textfile.close(); 

  return 0; 

}
 
std::map<std::string, int> run_cutflow(std::vector<std::string> files, 
				       RunInfo info, const unsigned flags) {  

  TChain* input_chain = new TChain("susy"); 

  if (files.size() == 0) { 
    throw std::runtime_error("I need files to run!"); 
  }

  for (std::vector<std::string>::const_iterator file_itr = files.begin(); 
       file_itr != files.end(); 
       file_itr++) { 
    input_chain->Add(file_itr->c_str()); 
    // TFile f(file_itr->c_str()); 
    // f.ShowStreamerInfo(); 
  }


  FakeMetEstimator fakeMetEst;

  TrigSimulator trig_simulator; 

  SUSYObjDef def; 

  // unsigned branches = branches::run_num | branches::trigger; 
  unsigned branches = branches::all; 

  susy buffer(input_chain, branches); 
  int n_entries = input_chain->GetEntries(); 


  // create a textfile to for annoying susytools output
  std::streambuf* old_out_stream = std::cout.rdbuf();
  std::streambuf* old_err_stream = std::cerr.rdbuf();
  ofstream strCout("susy_noise.txt", ios_base::out | ios_base::trunc);
  std::cout.rdbuf( strCout.rdbuf() );
  std::cerr.rdbuf( strCout.rdbuf() );
    
  def.initialize(info.is_data); 

  // Restore old cout.
  if (flags & cutflag::verbose) { 
    std::cerr.rdbuf( old_err_stream ); 
    std::cout.rdbuf( old_out_stream );
  }

  std::map<std::string, int> cut_counters; 
  

  // looping through events

  std::cout << n_entries << " entries in chain" << std::endl; 
  int one_percent = n_entries /  100; 

  for (int evt_n = 0; evt_n < n_entries; evt_n++) { 
    if (evt_n % one_percent == 0 || evt_n == n_entries - 1 ) { 
      std::cout << "\r" << evt_n << " of " << n_entries << 
	" (" << evt_n  / (one_percent ) << "%) processed "; 
      std::cout.flush(); 
    }

    cut_counters["n_events"]++;

    input_chain->GetEntry(evt_n); 

    def.Reset(); 
    
    bool trigger = false;

    if(info.is_signal){
      trigger = trig_simulator.get_decision(buffer); 
      
    }else{
      trigger=false;
      if(buffer.RunNumber <= 187815 && buffer.EF_xe70_noMu) trigger=true;
      if(buffer.RunNumber >  187815 && buffer.EF_xe60_verytight_noMu &&
	 buffer.RunNumber <= 191933) trigger=true;
    }

    // event preselection preparation
   
    const int n_jets = buffer.jet_AntiKt4TopoNewEM_n; 
    const int n_el = buffer.el_n;
    const int n_mu = buffer.mu_staco_n; // I'm pretty sure we're using staco
 
    TVector2 met(buffer.MET_Simplified20_RefFinal_etx, 
		 buffer.MET_Simplified20_RefFinal_ety); 
   
    std::vector<SelectedJet> selected_jets; 
   
    bool badjet_loose = false;

    for (int jet_n = 0; jet_n < n_jets; jet_n++){ 
    
      //add the "standard quality" cuts here ************************
      //JVF>0.75, pt>20GeV, isGoodJet (from SUSYTools), ...)

      if( buffer.jet_AntiKt4TopoNewEM_pt->at(jet_n) < 20*GeV)
	continue;

      //this is where the jet is built 
      selected_jets.push_back(SelectedJet(buffer, jet_n)); 
    }

    for( size_t i = 0; i < selected_jets.size(); i++){
      SelectedJet jet = selected_jets.at(i);
 
      bool is_jet = check_if_jet(jet.jet_index(), buffer, def, info); 
      // ... fill jets here 

      if(!is_jet) badjet_loose = true; }
   
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

    bool ishforveto = false;
    
    //ishforveto cut setup 
    if(!info.is_data){
      if (buffer.top_hfor_type==4)  ishforveto = true;

    }
     
    //---------------------------------------------------
    //Cleanup Cuts:
  
    bool lar_error = buffer.larError;
    if(lar_error)
      continue;
    cut_counters["lar_error"]++; 
     
    if(IsSmartLArHoleVeto( met,
			   fakeMetEst,
			   buffer, 
			   def, 
			   selected_jets))
      continue;     
    cut_counters["lar_hole_veto"]++;

    if(badjet_loose) 
      continue;
    cut_counters["badjet_loose"]++;
  
    // if(badz0wrtPVmuon)  to be completed
    // continue;
       
    if(!def.IsGoodVertex(buffer.vx_nTracks))
      continue;
    cut_counters["GoodPV"]++;
  
    if(ishforveto) 
      continue;
    cut_counters["hfor_veto"]++;
     
    //------------------------------------------------
    //Preselection cuts
     
    if(!trigger)
      continue;
    cut_counters["trigger"]++;

     
    if(selected_jets.size()<3)
      continue;
    cut_counters["n_jets_gt_3"]++;
 

    bool jetAbove150 = false;
    //could move this for loop up with the others. cleaner?
    for (unsigned i=0;i<selected_jets.size();i++){
      if(selected_jets.at(i).Pt() > 150*GeV) jetAbove150 = true;
    }
     
    if(!jetAbove150)
      continue;
    cut_counters["one_jet_pt_gt_150"]++;
		
    //correct implementation? 
    if(met.Mod() <= 150*GeV)
      continue;
    cut_counters["MET_gt_150"]++;

    //no electrons
    if(n_good_electrons >= 1)
      continue;
    cut_counters["el_veto"]++;

    //no muons
    if(n_good_muons >= 1)
      continue;
    cut_counters["mu_veto"]++;

    
    //For DeltaPhi cut
    bool pass_dphi_veto = true; 
    for(std::vector<SelectedJet>::const_iterator itr = selected_jets.begin();
	itr != selected_jets.end(); itr++) { 
      double deltaphi = fabs(met.Phi() - itr->Phi()); 
      if(deltaphi > M_PI) deltaphi = fabs(deltaphi - 2*M_PI);
      if(deltaphi < 0.4){ 
	pass_dphi_veto = false; 
	break; 
      }
    }
    
    if (!pass_dphi_veto) { 
      continue;
    }
    cut_counters["min_jetmet_deltaPhi"]++;
     

    //ctag > 2 cut
    int ctagJets = 0;
    for(std::vector<SelectedJet>::const_iterator itr = selected_jets.begin();
	itr != selected_jets.end(); itr++) { 
      if(itr->combNN_btag() > -2 &&
	 itr->combNN_btag() < 4)
	ctagJets++;
    } 

    if(ctagJets < 2)
      continue;
    cut_counters["2_ctag"]++;
 
  }
  std::cout << "\n"; 
  
  // restore cout if not already done
  if (!flags & cutflag::verbose) { 
    std::cerr.rdbuf( old_err_stream ); 
    std::cout.rdbuf( old_out_stream );
  }


  def.finalize(); 
  return cut_counters; 
	 
}

TrigSimulator::TrigSimulator(float fraction_preswap): 
  m_frac_preswap(fraction_preswap)
{ 
}

bool TrigSimulator::get_decision(const susy& buffer){ 
  // We have no runnumbers in our Stop-samples, so we create a
  // random number to determine what trigger is being used for
  // which event (depending on the luminosities of the periods
  // and the used triggers) 
  // EF_xe70_noMu: perdiod B-K
  // EF_xe60_verytight_noMu: period L-M
  float rand_float = float(rand() ) / float(RAND_MAX); 

  //cout << rndnr << endl;
  if(rand_float < m_frac_preswap){
    if(buffer.EF_xe70_noMu) return true;
  }
  else{
    if(buffer.EF_xe60_verytight_noMu) return true;
  }
  return false; 

}; 

  
bool IsSmartLArHoleVeto(TVector2 met,FakeMetEstimator& fakeMetEst,const susy& buffer, SUSYObjDef& def, std::vector<SelectedJet> selected_jets ) {

  bool isVeto=false; 
  
  for (unsigned int j = 0; j<selected_jets.size(); j++)
    {

      if(selected_jets.at(j).Pt()<= 20.) continue;
      if(def.IsLArHole(selected_jets.at(j).Eta(),
		       selected_jets.at(j).Phi()))
	{

	  int d3pd_index = selected_jets.at(j).jet_index();
	  //use jet pT after JES/JER
	  if(fakeMetEst.isBad
	     (buffer.jet_AntiKt4TopoNewEM_pt            ->at(d3pd_index),
	      buffer.jet_AntiKt4TopoNewEM_BCH_CORR_JET  ->at(d3pd_index),
	      buffer.jet_AntiKt4TopoNewEM_BCH_CORR_CELL ->at(d3pd_index),
	      buffer.jet_AntiKt4TopoNewEM_BCH_CORR_DOTX ->at(d3pd_index),
	      buffer.jet_AntiKt4TopoNewEM_phi           ->at(d3pd_index),
	      met.Px()*1e3,
	      met.Py()*1e3,
	      10000.,
	      -1.,
	      -1.)


	     /* selected_jets.at(j).Pt()*1e3,
		selected_jets.at(j).BCH_CORR_JET,
		selected_jets.at(j).BCH_CORR_CELL,
		selected_jets.at(j).BCH_CORR_DOTX,
		selected_jets.at(j).Phi(),
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
     10,			// eta cut - changed from 2.8 for testing
     JetID::VeryLooseBad,
     SystErr::NONE);
    
}
  
  
  
SelectedJet::SelectedJet(const susy& buffer, int jet_index) { 
  double pt = buffer.jet_AntiKt4TopoNewEM_pt  ->at(jet_index); 
  double eta = buffer.jet_AntiKt4TopoNewEM_eta ->at(jet_index); 
  double phi = buffer.jet_AntiKt4TopoNewEM_phi ->at(jet_index); 
  double e = buffer.jet_AntiKt4TopoNewEM_E   ->at(jet_index); 
  SetPtEtaPhiE(pt,eta,phi,e); 
  m_jet_index = jet_index;
    
  m_combNN_btag_wt = buffer.jet_AntiKt4TopoNewEM_flavor_weight_JetFitterCOMBNN ->at(jet_index); 	// TODO: find this branch and set it
}
  
double SelectedJet::combNN_btag() const { 
  //std::cerr << "ERROR: this isn't defined yet\n"; 
  //assert(false); 

  return m_combNN_btag_wt;
}
  
double SelectedJet::jfitcomb_cu(const susy& buffer, int jet_index) const {

  double cu; 
  cu = buffer.jet_AntiKt4TopoNewEM_flavor_component_jfitcomb_pc->at(jet_index)
    / buffer.jet_AntiKt4TopoNewEM_flavor_component_jfitcomb_pu->at(jet_index);

  return log(cu);
}

double SelectedJet::jfitcomb_cb(const susy& buffer, int jet_index) const {

  double cb; 
  cb =  buffer.jet_AntiKt4TopoNewEM_flavor_component_jfitcomb_pc->at(jet_index) / buffer.jet_AntiKt4TopoNewEM_flavor_component_jfitcomb_pb->at(jet_index);
   
  return log(cb); 
}
 

 
int SelectedJet::jet_index() const{

  return m_jet_index;

}

SmartChain::SmartChain(std::string tree_name): 
  TChain(tree_name.c_str())
{ 
}

void SmartChain::SetBranchAddress(std::string name, void** branch) { 
  int return_code = TChain::SetBranchAddress(name.c_str(), branch); 
  if (return_code) { 
    throw std::runtime_error(name); 
  }
}

