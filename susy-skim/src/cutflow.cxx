#include "cutflow.hh"
#include "SusyBuffer.h"
#include <iostream>
#include <fstream>
#include <algorithm>
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
#include "EventBits.hh"

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
  info.run_number = 180614; 
  srand(0); 
  using namespace cutflag; 
  unsigned flags = verbose | is_signal; 


  // run the main routine 
  typedef std::vector<std::pair<std::string, int> > CCOut; 
  CCOut cut_counters = run_cutflow(input_files, 
				   info, 
				   flags, 
				   "output_skim.root"); 

  float firstcut = cut_counters.at(0).second; 
  for (CCOut::const_iterator cut_itr = cut_counters.begin(); 
       cut_itr != cut_counters.end(); 
       cut_itr++) { 
    std::cout << boost::format("%i events pass %s (%.2f%%)\n") % 
      cut_itr->second % cut_itr->first % 
      ( float(cut_itr->second) * 100 / firstcut) ; 

  }

  return 0; 

}
 
std::vector<std::pair<std::string, int> >
run_cutflow(std::vector<std::string> files, 
	    RunInfo info, const unsigned flags, 
	    std::string out_ntuple_name) {  

  SmartChain* input_chain = new SmartChain("susy"); 

  if (files.size() == 0) { 
    throw std::runtime_error("I need files to run!"); 
  }

  for (std::vector<std::string>::const_iterator file_itr = files.begin(); 
       file_itr != files.end(); 
       file_itr++) { 
    int ret_code = input_chain->Add(file_itr->c_str(),-1); 
    if (ret_code != 1) { 
      std::string err = (boost::format("error connecting file %s to chain") % 
			 *file_itr).str(); 
      throw std::runtime_error(err); 
    }
  }

  typedef std::vector<SelectedJet>::const_iterator JetItr; 

  FakeMetEstimator fakeMetEst;

  TrigSimulator trig_simulator; 

  SUSYObjDef def; 

  // unsigned branches = branches::run_num | branches::trigger; 
  unsigned branches = branches::all; 

  SusyBuffer buffer(input_chain, branches); 
  int n_entries = input_chain->GetEntries(); 


  // dump stdout from susytools init to /dev/null 
  std::streambuf* old_out_stream = std::cout.rdbuf();
  std::streambuf* old_err_stream = std::cerr.rdbuf();
  ofstream strCout("/dev/null");
  std::cout.rdbuf( strCout.rdbuf() );
  std::cerr.rdbuf( strCout.rdbuf() );
    
  def.initialize(flags & cutflag::is_data); 

  // Restore old cout.
  if (flags & cutflag::verbose) { 
    std::cerr.rdbuf( old_err_stream ); 
    std::cout.rdbuf( old_out_stream );
  }


  // --- output things ---

  // tree with no name means no tree
  OutTree out_tree(out_ntuple_name); 

  CutCounter cut_counters; 
  
  // looping through events

  std::cout << n_entries << " entries in chain" << std::endl; 
  int one_percent = n_entries /  100; 

  for (int evt_n = 0; evt_n < n_entries; evt_n++) { 
    if (evt_n % one_percent == 0 || evt_n == n_entries - 1 ) { 
      std::cout << boost::format("\r%i of %i (%.1f%%) processed") % 
	(evt_n + 1) % n_entries % ( (evt_n + 1) / one_percent); 
      std::cout.flush(); 
    }

    cut_counters["n_events"]++;

    input_chain->GetEntry(evt_n); 

    def.Reset(); 
    
    bool trigger = false;

    if(flags & cutflag::is_signal){
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

      bool low_pt_jet = buffer.jet_AntiKt4TopoNewEM_pt->at(jet_n) < 20*GeV;
      
      //this is where the jet is built 
      if (!low_pt_jet || flags & cutflag::use_low_pt_jets) { 
	selected_jets.push_back(SelectedJet(buffer, jet_n)); 
      }
    }

    double leading_jet_pt = 0; 
    for(JetItr itr = selected_jets.begin(); 
	itr != selected_jets.end(); itr++){
      const SelectedJet& jet = *itr;
 
      bool is_jet = check_if_jet(jet.jet_index(), buffer, def, flags); 
      // ... fill jets here 

      if (is_jet) { 
	leading_jet_pt = std::max(jet.Pt(), leading_jet_pt); 
      }
      else { 
	badjet_loose = true; 
      }
    }
   
    // unless we start doing something with them we can just count the good el
    int n_good_electrons = 0; 
    for (int el_n = 0; el_n < n_el; el_n++){
      bool isElectron = check_if_electron(el_n, buffer, def, flags, info);
      if (isElectron) n_good_electrons++; 
    }

    int n_good_muons = 0; 
    for (int mu_n = 0; mu_n < n_mu; mu_n++){

      /*part of the badz0wrtPVmuon cut - complete later if needed
	muon.isCosmic = def.IsCosmicMuon(mu_staco_z0_exPV->at(mu_n),
	mu_staco_d0_exPV->at(mu_n),
	5.,
	2.); */

      bool isMuon = check_if_muon(mu_n, buffer, def, flags);
      if (isMuon) n_good_muons++; 
    }

    bool ishforveto = false;
    
    //ishforveto cut setup 
    if(! (flags & cutflag::is_data)){
      if (buffer.top_hfor_type == 4)  ishforveto = true;
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

    if(selected_jets.size() < 3)
      continue;
    cut_counters["n_jets_gt_3"]++;

    // --- this is the region we're studying ----

    float min_jetmet_dphi = get_jetmet_dphi(selected_jets, met); 

    int n_mainz_ctags = 0;
    for(std::vector<SelectedJet>::const_iterator itr = selected_jets.begin();
	itr != selected_jets.end(); itr++) { 
      if(itr->combNN_btag() > -2 &&
	 itr->combNN_btag() < 4)
	n_mainz_ctags++;
    } 
 
    unsigned pass_bits = 0; 
    using namespace evt; 
    if (leading_jet_pt > 150*GeV) pass_bits |= pass_leading_pt; 
    if (met.Mod() > 150*GeV)      pass_bits |= pass_met; 
    if (n_good_electrons == 0)    pass_bits |= pass_el_veto; 
    if (n_good_muons == 0)        pass_bits |= pass_mu_veto; 
    if (min_jetmet_dphi > 0.4)    pass_bits |= pass_jetmet_dphi; 
    if (n_mainz_ctags >= 2)       pass_bits |= pass_mainz_ctag; 

    fill_cutflow_from_bits(cut_counters, pass_bits); 

    out_tree.pass_bits = pass_bits; 
    out_tree.met = met.Mod(); 
    out_tree.min_jetmet_dphi = min_jetmet_dphi; 

    std::sort(selected_jets.begin(), selected_jets.end(), has_lower_pt); 
    std::reverse(selected_jets.begin(), selected_jets.end()); 
    const SelectedJet& leading_jet = selected_jets.at(1); 
    const SelectedJet& subleading_jet = selected_jets.at(2); 
    copy_jet_info(leading_jet, buffer, out_tree.leading_jet); 
    copy_jet_info(subleading_jet, buffer, out_tree.subleading_jet); 
    out_tree.fill(); 
 
  }
  std::cout << "\n"; 
  
  // restore cout if not already done
  if (!flags & cutflag::verbose) { 
    std::cerr.rdbuf( old_err_stream ); 
    std::cout.rdbuf( old_out_stream );
  }


  def.finalize(); 
  return cut_counters.get_ordered_cuts(); 
	 
}

void copy_jet_info(const SelectedJet& in, const SusyBuffer& buffer, 
		   OutTree::Jet& jet)
{
  int jet_index = in.jet_index(); 
  jet.pt = in.Pt(); 
  jet.eta = in.Eta(); 
  jet.phi = in.Phi(); 
  jet.cnn_b = buffer.jet_AntiKt4TopoNewEM_flavor_component_jfitcomb_pb->at
    (jet_index); 
  jet.cnn_c = buffer.jet_AntiKt4TopoNewEM_flavor_component_jfitcomb_pc->at
    (jet_index); 
  jet.cnn_u = buffer.jet_AntiKt4TopoNewEM_flavor_component_jfitcomb_pu->at
    (jet_index); 
  jet.flavor_truth_label = -1; 
}


int fill_cutflow_from_bits(CutCounter& cut_counters, const unsigned bits) { 
  using namespace evt; 
  if (! (bits & pass_leading_pt) ) { 
    return 0; 
  }
  cut_counters["one_jet_pt_gt_150"]++;
  
  if (! (bits & pass_met)        ) { 
    return 0; 
  }
  cut_counters["MET_gt_150"]++;

  if (! (bits & pass_el_veto)    ) { 
    return 0; 
  }
  cut_counters["el_veto"]++;
  
  if (! (bits & pass_mu_veto)    ) { 
    return 0; 
  }
  cut_counters["mu_veto"]++;

  if (! (bits & pass_jetmet_dphi)) { 
    return 0; 
  }
  cut_counters["min_jetmet_deltaPhi"]++;

  if (! (bits & pass_mainz_ctag) ) { 
    return 0; 
  }
  cut_counters["2_ctag"]++;
  return 1; 

}

float get_jetmet_dphi(const std::vector<SelectedJet>& jets, 
		      const TVector2& met){ 
  float min_dphi = M_PI; 
  for(std::vector<SelectedJet>::const_iterator itr = jets.begin();
      itr != jets.end(); itr++) { 
    float deltaphi = fabs(met.Phi() - itr->Phi()); 
    if(deltaphi > M_PI) deltaphi = fabs(deltaphi - 2*M_PI);
    min_dphi = std::min(deltaphi, min_dphi); 
  }
  return min_dphi; 
}


TrigSimulator::TrigSimulator(float fraction_preswap): 
  m_frac_preswap(fraction_preswap)
{ 
}

bool TrigSimulator::get_decision(const SusyBuffer& buffer){ 
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


bool has_lower_pt(const TLorentzVector& v1, const TLorentzVector& v2) { 
  return v1.Pt() < v2.Pt(); 
}

bool IsSmartLArHoleVeto(TVector2 met,
			FakeMetEstimator& fakeMetEst,
			const SusyBuffer& buffer, 
			SUSYObjDef& def, 
			std::vector<SelectedJet> selected_jets ) {
  
  for (unsigned int j = 0; j<selected_jets.size(); j++){
    
    if(selected_jets.at(j).Pt() <= 20.) continue;
    if(!def.IsLArHole(selected_jets.at(j).Eta(),
		     selected_jets.at(j).Phi())) {
      continue; 
    }

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
      
    int d3pd_index = selected_jets.at(j).jet_index();
    //use jet pT after JES/JER
    if(fakeMetEst.isBad
       (buffer.jet_AntiKt4TopoNewEM_pt            ->at(d3pd_index),
	buffer.jet_AntiKt4TopoNewEM_BCH_CORR_JET  ->at(d3pd_index),
	buffer.jet_AntiKt4TopoNewEM_BCH_CORR_CELL ->at(d3pd_index),
	buffer.jet_AntiKt4TopoNewEM_BCH_CORR_DOTX ->at(d3pd_index),
	buffer.jet_AntiKt4TopoNewEM_phi           ->at(d3pd_index),
	met.Px()*GeV,
	met.Py()*GeV,
	10000.,
	-1.,
	-1.)
       ) {
      return true; 
    }
  } // end jet loop

  return false;

}

bool check_if_electron(int iEl,
		       const SusyBuffer& buffer,
		       SUSYObjDef& def,
		       const unsigned flags, 
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
     flags & cutflag::is_data,
     20000.,                         //et cut
     2.47,                          //eta cut
     SystErr::NONE);
     
  //info.isAF2
  //NOTE: el_wet set to zero, couldn't match variable in D3PD
}

bool check_if_muon(int iMu,
		   const SusyBuffer& buffer,
		   SUSYObjDef& def,
		   const unsigned flags){

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
     flags & cutflag::is_data,
     10000.,                      //pt cut
     2.4,                         //eta cut
     SystErr::NONE);


}

  
bool check_if_jet(int iJet, 
		  const SusyBuffer& buffer, 
		  SUSYObjDef& def, 
		  const unsigned flags){ 
    
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
     flags & cutflag::is_data, 
     20000., 			// pt cut
     10,			// eta cut - changed from 2.8 for testing
     JetID::VeryLooseBad,
     SystErr::NONE);
    
}
  
  
  
SelectedJet::SelectedJet(const SusyBuffer& buffer, int jet_index) { 
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
  
double SelectedJet::jfitcomb_cu(const SusyBuffer& buffer, 
				int jet_index) const {

  double cu; 
  cu = buffer.jet_AntiKt4TopoNewEM_flavor_component_jfitcomb_pc->at(jet_index)
    / buffer.jet_AntiKt4TopoNewEM_flavor_component_jfitcomb_pu->at(jet_index);

  return log(cu);
}

double SelectedJet::jfitcomb_cb(const SusyBuffer& buffer, 
				int jet_index) const {

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

void SmartChain::SetBranchAddress(std::string name, void* branch) { 
  int return_code = TChain::SetBranchAddress(name.c_str(), branch); 
  if (return_code != 0 && return_code != 5 ){ 
    std::string issue = (boost::format("can not set %s , return code %i") % 
			 name % return_code).str(); 
    throw std::runtime_error(issue); 
  }
  if (!GetBranch(name.c_str())) { 
    std::string issue = (boost::format("can't find branch %s") % name).str(); 
    throw std::runtime_error(issue); 
  }
}

int& CutCounter::operator[](std::string key) 
{ 
  bool new_cut = (find(key) == end()); 
  if (new_cut) { 
    m_cuts.push_back(key); 
  }
  return std::map<std::string, int>::operator[](key); 
}

std::vector< std::pair<std::string, int> > CutCounter::get_ordered_cuts() 
  const 
{
  typedef std::vector<std::string>::const_iterator IdxItr; 
  std::vector< std::pair<std::string, int> > ordered_cuts; 
  for (IdxItr itr = m_cuts.begin(); itr != m_cuts.end(); itr++) { 
    ordered_cuts.push_back(*find(*itr));
  }
  return ordered_cuts;
}

OutTree::OutTree(std::string file, std::string tree): 
  m_file(0), 
  m_tree(0)
{ 
  if (file.size() > 0) { 
    m_file = new TFile(file.c_str(), "recreate"); 
    m_tree = new TTree(tree.c_str(), tree.c_str()); 
    init(); 
  }
}

void OutTree::Jet::set_branches(TTree* tree, std::string prefix) 
{
  tree->Branch((prefix + "pt").c_str(), &pt); 
  tree->Branch((prefix + "eta").c_str(), &eta); 
  tree->Branch((prefix + "phi").c_str(), &phi); 
  tree->Branch((prefix + "flavor_truth_label").c_str(), 
		 &flavor_truth_label); 
  tree->Branch((prefix + "cnn_b").c_str(), &cnn_b); 
  tree->Branch((prefix + "cnn_c").c_str(), &cnn_c); 
  tree->Branch((prefix + "cnn_u").c_str(), &cnn_u); 
}

void OutTree::init() 
{ 
  m_tree->Branch("pass_bits", &pass_bits ); 
  m_tree->Branch("met", &met); 
  m_tree->Branch("min_jetmet_dphi" , &min_jetmet_dphi); 

  leading_jet.set_branches(m_tree, "leading_jet_"); 
  subleading_jet.set_branches(m_tree, "subleading_jet_"); 

}

OutTree::~OutTree() 
{
  if (m_file){ 
    m_file->WriteTObject(m_tree); 
    delete m_file; 
  }
  m_file = 0; 
}

void OutTree::fill() 
{
  if (m_tree) { 
    m_tree->Fill(); 
  }
}
