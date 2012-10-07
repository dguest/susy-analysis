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
#include <boost/scoped_ptr.hpp>

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


//_________________________________________________________________
// run the cutflow
 
std::vector<std::pair<std::string, int> >
run_cutflow(std::vector<std::string> files, 
	    RunInfo info, const unsigned flags, 
	    std::string out_ntuple_name) {  

  boost::scoped_ptr<SmartChain> input_chain(new SmartChain("susy")); 

  if (files.size() == 0) { 
    throw std::runtime_error("I need files to run!"); 
  }

  for (std::vector<std::string>::const_iterator file_itr = files.begin(); 
       file_itr != files.end(); 
       file_itr++) { 
    int ret_code = input_chain->Add(file_itr->c_str(),-1); 
    if (ret_code != 1) { 
      std::string err = (boost::format("bad file: %s") % 
			 *file_itr).str(); 
      throw std::runtime_error(err); 
    }
  }


  SUSYObjDef def; 

  // unsigned branches = branches::run_num | branches::trigger; 
  unsigned branches = branches::all; 

  BranchNames branch_names; 
  branch_names.trigger = "EF_xe80_tclcw_loose"; 

  SusyBuffer buffer(input_chain.get(), branches, branch_names); 
  int n_entries = input_chain->GetEntries(); 

  if (flags & cutflag::get_branches) { 
    std::vector<std::string> br_names = input_chain->get_all_branch_names();
    ofstream branch_save("all-set-branches.txt"); 
    for (std::vector<std::string>::const_iterator itr = br_names.begin(); 
	 itr != br_names.end(); itr++) { 
      branch_save << *itr << std::endl;
    }
    branch_save.close(); 
  }


  // dump stdout from susytools init to /dev/null 
  std::streambuf* old_out_stream = std::cout.rdbuf();
  std::streambuf* old_err_stream = std::cerr.rdbuf();
  std::string out_file = "/dev/null"; 
  if (flags & cutflag::debug_susy) { 
    out_file = "susy-dbg.txt"; 
  }
  ofstream strCout(out_file.c_str());
  std::cout.rdbuf( strCout.rdbuf() );
  std::cerr.rdbuf( strCout.rdbuf() );
    
  def.initialize(flags & cutflag::is_data); 
  def.SetJetCalib(false); 


  // Restore old cout.
  if (flags & cutflag::verbose) { 
    std::cerr.rdbuf( old_err_stream ); 
    std::cout.rdbuf( old_out_stream );
  }

  // --- output things ---
  OutTree out_tree(out_ntuple_name,  "evt_tree", flags); 
  BitmapCutflow cutflow; 
  cutflow.add("GRL"                   , pass::grl            );  
  cutflow.add(branch_names.trigger    , pass::trigger        );
  cutflow.add("lar_error" 	      , pass::lar_error      );
  cutflow.add("core" 		      , pass::core           );
  cutflow.add("jet_clean" 	      , pass::jet_clean      );
  cutflow.add("vxp_good" 	      , pass::vxp_gt_4trk    );
  cutflow.add("leading_jet_120"	      , pass::leading_jet    );
  cutflow.add("met_120" 	      , pass::met            );
  cutflow.add("n_jet_geq_3" 	      , pass::n_jet          );
  cutflow.add("dphi_jetmet_sum"       , pass::dphi_jetmet_sum);
  cutflow.add("lepton_veto" 	      , pass::lepton_veto    );
  cutflow.add("ctag_mainz"            , pass::ctag_mainz     ); 
  
  // looping through events
  int one_percent = n_entries /  100; 

  for (int evt_n = 0; evt_n < n_entries; evt_n++) { 
    if (evt_n % one_percent == 0 || evt_n == n_entries - 1 ) { 
      std::cout << boost::format("\r%i of %i (%.1f%%) processed") % 
	(evt_n + 1) % n_entries % ( (evt_n + 1) / one_percent); 
      std::cout.flush(); 
    }

    input_chain->GetEntry(evt_n); 

    def.Reset(); 
    out_tree.clear_buffer(); 
    
    EventJets all_jets(buffer, def, flags, info); 
    EventElectrons all_electrons(buffer, def, flags, info); 
    EventMuons all_muons(buffer, def, flags, info); 

    std::vector<Electron*> susy_electrons = filter_susy(all_electrons); 
    std::vector<Muon*> susy_muons = filter_susy(all_muons); 

    unsigned pass_bits = 0; 
  
    pass_bits |= pass::grl; 

    if(buffer.trigger)    pass_bits |= pass::trigger; 
    if(!buffer.larError)  pass_bits |= pass::lar_error; 

    if ( !(flags & cutflag::is_data ) ||
	 ((buffer.coreFlags & 0x40000) == 0) ){ 
      pass_bits |= pass::core; 
    }

    std::sort(all_jets.begin(),all_jets.end(),has_lower_pt); 
    std::reverse(all_jets.begin(), all_jets.end()); 

    if (all_jets.size()) { 
      copy_jet_info(all_jets.at(0), out_tree.leading_jet_uncensored); 
    }
    
    std::vector<SelectedJet*> selected_jets; 
    for (EventJets::const_iterator jet_itr = all_jets.begin(); 
	 jet_itr != all_jets.end(); 
	 jet_itr++) {
      const SelectedJet& jet = **jet_itr; 
      bool is_low_pt = (jet.bits() & jetbit::low_pt); 
      if (!is_low_pt) { 
	selected_jets.push_back(*jet_itr); 
      }
    }
    remove_overlaping(susy_electrons, selected_jets, 0.2); 

    pass_bits |= pass::jet_clean; 
    for (EventJets::const_iterator jet_itr = selected_jets.begin(); 
	 jet_itr != selected_jets.end(); 
	 jet_itr++) { 
      bool is_jet = ((*jet_itr)->bits() & jetbit::good); 
      bool is_veto = !is_jet;
      if (is_veto) pass_bits &=~ pass::jet_clean; 
    }

    if(def.IsGoodVertex(buffer.vx_nTracks)) {
      pass_bits |= pass::vxp_gt_4trk; 
    }
      
    std::vector<int> muon_idx = get_indices(susy_muons); 
    TVector2 met = get_met(buffer, def, info, muon_idx); 
    if (met.Mod() > 120*GeV) { 
      pass_bits |= pass::met; 
    }

    std::vector<SelectedJet*> good_jets; 
    for (EventJets::const_iterator jet_itr = selected_jets.begin(); 
	 jet_itr != selected_jets.end(); jet_itr++) { 

      const SelectedJet& jet = **jet_itr; 
      bool good_pt = jet.Pt() > 30*GeV; 
      bool good_eta = fabs(jet.Eta()) < 2.5;
      bool good_jvf = jet.jvf() > 0.5; 
      if (good_pt && good_eta && good_jvf) { 
	good_jets.push_back(*jet_itr); 
	bool good_isr_pt = jet.Pt() > 120*GeV;
	if (good_isr_pt) { 
	  pass_bits |= pass::leading_jet; 
	}
      }
    }

    remove_overlaping(selected_jets, susy_electrons, 0.4); 
    remove_overlaping(selected_jets, susy_muons, 0.4); 

    if (susy_electrons.size() == 0 && susy_muons.size() == 0) { 
      pass_bits |= pass::lepton_veto; 
    }

    const unsigned n_req_jets = 3; 
    if (good_jets.size() >= n_req_jets) { 
      pass_bits |= pass::n_jet; 

      std::vector<SelectedJet*> leading_jets(good_jets.begin(), 
					     good_jets.begin() + n_req_jets); 

      //  --- multijet things ---
      do_multijet_calculations(leading_jets, pass_bits, out_tree, met); 
    }

    if (good_jets.size() >= 1) { 
      copy_jet_info(good_jets.at(0), out_tree.isr_jet); 
    }
    if (good_jets.size() >= 2) { 
      copy_jet_info(good_jets.at(1), out_tree.leading_jet);
      if (pass_mainz_ctag(good_jets.at(1)) ) { 
	pass_bits |= pass::ctag_mainz; 
      }
    }
    if (good_jets.size() >= 3) { 
      copy_jet_info(good_jets.at(2), out_tree.subleading_jet); 
      if (pass_mainz_ctag(good_jets.at(2)) ) { 
	pass_bits |= pass::ctag_mainz; 
      }
    }

    cutflow.fill(pass_bits); 
    out_tree.pass_bits = pass_bits; 
    out_tree.met = met.Mod(); 

    out_tree.fill(); 
 
  }
  std::cout << "\n"; 
  
  // restore cout if not already done
  if (!flags & cutflag::verbose) { 
    std::cerr.rdbuf( old_err_stream ); 
    std::cout.rdbuf( old_out_stream );
  }


  def.finalize(); 
  return cutflow.get(); 
	 
}

void do_multijet_calculations(const std::vector<SelectedJet*>& leading_jets, 
			      unsigned& pass_bits, 
			      OutTree& out_tree, 
			      const TVector2& met) 
{ 
  float dphi_sum = get_sum_jetmet_dphi(leading_jets, met); 
  if (dphi_sum > 0.4) { 
    pass_bits |= pass::dphi_jetmet_sum; 
  }
  out_tree.sum_jetmet_dphi = dphi_sum; 

  float dphi_min = get_min_jetmet_dphi(leading_jets, met); 
  if (dphi_min > 0.4) { 
    pass_bits |= pass::dphi_jetmet_min; 
  }
  out_tree.min_jetmet_dphi = dphi_min; 

}

//____________________________________________________________
// utility functions 

bool pass_mainz_ctag(const SelectedJet* jet){ 

  bool pass = (jet->combNN_btag() > -2.55 && 
	       jet->combNN_btag() < 1.0); 
  return pass; 
}


float get_min_jetmet_dphi(const std::vector<SelectedJet*>& jets, 
			  const TVector2& met){ 
  typedef std::vector<SelectedJet*> JVec; 
  typedef JVec::const_iterator JItr; 
  TLorentzVector mector; 
  mector.SetPtEtaPhiE(1,0,met.Phi(),1); 
  float min_dphi = M_PI; 
  for(JItr itr = jets.begin(); itr != jets.end(); itr++){
    float deltaphi = mector.DeltaPhi(**itr); 
    min_dphi = std::min(deltaphi, min_dphi); 
  }
  return min_dphi; 
}

float get_sum_jetmet_dphi(const std::vector<SelectedJet*>& jets, 
			  const TVector2& met){ 
  typedef std::vector<SelectedJet*> JVec; 
  typedef JVec::const_iterator JItr; 
  TLorentzVector sum; 
  for(JItr itr = jets.begin(); itr != jets.end(); itr++){
    sum += **itr; 
  }
  TLorentzVector mector; 
  mector.SetPtEtaPhiE(1,0,met.Phi(),1); 
  return mector.DeltaPhi(sum); 
}

bool has_lower_pt(const TLorentzVector* v1, const TLorentzVector* v2) { 
  return v1->Pt() < v2->Pt(); 
}


//____________________________________________________________________
// io functions 

void copy_jet_info(const SelectedJet* in, OutTree::Jet& jet)
{

  jet.pt = in->Pt(); 
  jet.eta = in->Eta(); 
  jet.phi = in->Phi(); 
  jet.cnn_b = in->pb(); 
  jet.cnn_c = in->pc(); 
  jet.cnn_u = in->pu(); 
  jet.flavor_truth_label = in->flavor_truth_label(); 

}



//______________________________________________________________________
// object check functions 

bool check_if_electron(int iEl,
		       const SusyBuffer& buffer,
		       SUSYObjDef& def,
		       const unsigned flags, 
		       const RunInfo& info){
  
  bool pass_el = def.FillElectron
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
     buffer.el_MET_Egamma10NoTau_wet->at(iEl).at(0), 
     // buffer.el_MET_Egamma10NoTau_wet ->at(iEl),
     flags & cutflag::is_data);

  return pass_el; 
  //info.isAF2
  //NOTE: el_wet set to zero, couldn't match variable in D3PD
}

bool check_if_muon(int iMu,
		   const SusyBuffer& buffer,
		   SUSYObjDef& def,
		   const unsigned flags){

  return def.FillMuon
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
     buffer.mu_staco_charge                       ->at(iMu), 
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
     flags & cutflag::is_data);


}

  
bool check_if_jet(int iJet, 
		  const SusyBuffer& buffer, 
		  SUSYObjDef& def, 
		  const unsigned flags, 
		  const RunInfo& info){ 

  assert(buffer.jet_pt                 );
  assert(buffer.jet_eta                );
  assert(buffer.jet_phi                );
  assert(buffer.jet_E                  );
  assert(buffer.jet_emscale_eta        );
  assert(buffer.jet_emfrac             );
  assert(buffer.jet_hecf               );
  assert(buffer.jet_LArQuality         );
  assert(buffer.jet_HECQuality         );
  assert(buffer.jet_AverageLArQF       );
  assert(buffer.jet_Timing             );
  assert(buffer.jet_sumPtTrk           );
  assert(buffer.jet_fracSamplingMax    );
  assert(buffer.jet_SamplingMax        );
  assert(buffer.jet_NegativeE          );
  assert(buffer.jet_flavor_truth_label );
  assert(buffer.jet_emscale_E          );
  assert(buffer.jet_emscale_eta        );
  assert(buffer.jet_EtaOrigin          );
  assert(buffer.jet_PhiOrigin          );
  assert(buffer.jet_MOrigin            );
    
  return def.FillJet
    (iJet, 
     buffer.jet_pt                 ->at(iJet), 
     buffer.jet_eta                ->at(iJet), 
     buffer.jet_phi                ->at(iJet),
     buffer.jet_E                  ->at(iJet), 
     buffer.jet_emscale_eta        ->at(iJet), 
     buffer.jet_emfrac             ->at(iJet), 
     buffer.jet_hecf               ->at(iJet),
     buffer.jet_LArQuality         ->at(iJet), 
     buffer.jet_HECQuality         ->at(iJet), 
     buffer.jet_AverageLArQF       ->at(iJet), 
     buffer.jet_Timing             ->at(iJet), 
     buffer.jet_sumPtTrk           ->at(iJet), 
     buffer.jet_fracSamplingMax    ->at(iJet),
     buffer.jet_SamplingMax        ->at(iJet), 
     buffer.jet_NegativeE          ->at(iJet), 
     buffer.jet_flavor_truth_label ->at(iJet), 
     buffer.jet_emscale_E          ->at(iJet),
     buffer.jet_emscale_eta        ->at(iJet), 
     buffer.jet_EtaOrigin          ->at(iJet), 
     buffer.jet_PhiOrigin          ->at(iJet), 
     buffer.jet_MOrigin            ->at(iJet), 
     buffer.averageIntPerXing,
     buffer.vx_nTracks,             
     info.run_number, 
     flags & cutflag::is_data, 
     20000.0, 	// pt cut
     2.8,	// eta cut - changed from 2.8 for testing
     JetID::VeryLooseBad,
     SystErr::NONE);
    
}
  
TVector2 get_met(const SusyBuffer& buffer, 
		 SUSYObjDef& def, 
		 const RunInfo&, 
		 const std::vector<int>& mu_mystery_index) 
{ 
  const int n_el = buffer.el_n; 
  std::vector<int> el_mystery_index; 
  for (int el_n = 0; el_n < n_el; el_n++) { 
    float mystery_float = buffer.el_MET_Egamma10NoTau_wet->at(el_n).at(0); 
    if (mystery_float != 0.0) { 
      el_mystery_index.push_back(el_n); 
    }
  }
  
  TVector2 met = def.GetMET
    (buffer.jet_pt, 
     buffer.jet_MET_Egamma10NoTau_wet,
     buffer.jet_MET_Egamma10NoTau_wpx,
     buffer.jet_MET_Egamma10NoTau_wpy,
     buffer.jet_MET_Egamma10NoTau_statusWord,
     el_mystery_index,
     buffer.el_MET_Egamma10NoTau_wet,
     buffer.el_MET_Egamma10NoTau_wpx,
     buffer.el_MET_Egamma10NoTau_wpy,
     buffer.el_MET_Egamma10NoTau_statusWord,
     buffer.MET_Egamma10NoTau_SoftJets_etx,
     buffer.MET_Egamma10NoTau_SoftJets_ety,
     buffer.MET_Egamma10NoTau_SoftJets_sumet,                                
     buffer.MET_Egamma10NoTau_CellOut_etx, //CellOut
     buffer.MET_Egamma10NoTau_CellOut_ety, //CellOut
     buffer.MET_Egamma10NoTau_CellOut_sumet, //CellOut
     buffer.MET_Egamma10NoTau_CellOut_etx, //CellOut Eflow
     buffer.MET_Egamma10NoTau_CellOut_ety, //CellOut Eflow
     buffer.MET_Egamma10NoTau_CellOut_sumet, //CellOut Eflow
     buffer.MET_Egamma10NoTau_RefGamma_etx,
     buffer.MET_Egamma10NoTau_RefGamma_ety,
     buffer.MET_Egamma10NoTau_RefGamma_sumet,
     mu_mystery_index, 
     buffer.mu_staco_ms_qoverp, 
     buffer.mu_staco_ms_theta, 
     buffer.mu_staco_ms_phi, 
     buffer.mu_staco_charge,
     buffer.averageIntPerXing); 
     
  return met; 
}

//___________________________________________________________________
// selected jet  
  
SelectedJet::SelectedJet(const EventJets* parent, int jet_index): 
  m_bits(0)
{ 
  const SusyBuffer& buffer = *parent->m_buffer; 
  double pt = buffer.jet_pt  ->at(jet_index); 
  double eta = buffer.jet_eta ->at(jet_index); 
  double phi = buffer.jet_phi ->at(jet_index); 
  double e = buffer.jet_E   ->at(jet_index); 
  SetPtEtaPhiE(pt,eta,phi,e); 
  m_jet_index = jet_index;

  m_jvf = buffer.jet_jvtxf->at(jet_index); 

  m_cnn_b = buffer.jet_flavor_component_jfitcomb_pb->at(jet_index); 
  m_cnn_c = buffer.jet_flavor_component_jfitcomb_pc->at(jet_index); 
  m_cnn_u = buffer.jet_flavor_component_jfitcomb_pu->at(jet_index); 
  m_flavor_truth_label = buffer.jet_flavor_truth_label->at(jet_index); 


}
  
double SelectedJet::combNN_btag() const { 
  return log(m_cnn_b / m_cnn_u); 
}
  
double SelectedJet::jfitcomb_cu(const SusyBuffer& buffer, 
				int jet_index) const {
  return m_cnn_u;
}

double SelectedJet::jfitcomb_cb(const SusyBuffer& buffer, 
				int jet_index) const {
  return m_cnn_b; 
}
 
int SelectedJet::index() const{
  return m_jet_index;
}

double SelectedJet::jvf() const { 
  return m_jvf; 
}
double SelectedJet::pb() const { 
  return m_cnn_b; 
}
double SelectedJet::pc() const { 
  return m_cnn_c; 
}
double SelectedJet::pu() const { 
  return m_cnn_u; 
}
int SelectedJet::flavor_truth_label() const { 
  return m_flavor_truth_label; 
}

unsigned SelectedJet::bits() const { 
  return m_bits; 
}

void SelectedJet::set_bit(unsigned bit) { 
  m_bits |= bit; 
}
void SelectedJet::unset_bit(unsigned bit) { 
  m_bits &=~ bit; 
}

EventJets::EventJets(const SusyBuffer& buffer, SUSYObjDef& def, 
		     unsigned flags, const RunInfo& info): 
  m_buffer(&buffer)
{ 

  assert(buffer.jet_n); 
  assert(buffer.jet_pt); 
  assert(buffer.el_cl_pt); 
  assert(buffer.el_cl_eta); 
  assert(buffer.el_cl_phi); 
  assert(buffer.el_cl_E); 

  const int n_jets = buffer.jet_n; 
  for (int jet_n = 0; jet_n < n_jets; jet_n++){ 
    
      //add the "standard quality" cuts here ************************
      //JVF>0.75, pt>20GeV, isGoodJet (from SUSYTools), ...)

    // must initalize all jets
    bool is_jet = check_if_jet(jet_n, buffer, def, flags, info); 

    SelectedJet* the_jet = new SelectedJet(this, jet_n); 
    push_back(the_jet); 

    // susytools corrected tlv
    TLorentzVector tlv = def.GetJetTLV(); 
    the_jet->SetPtEtaPhiE(tlv.Pt(), tlv.Eta(), tlv.Phi(), tlv.E()); 

    if (is_jet) { 
      the_jet->set_bit(jetbit::good); 
    }
    
    bool low_pt_jet = the_jet->Pt() < 20*GeV;
    if (low_pt_jet) { 
      the_jet->set_bit(jetbit::low_pt); 
    }
    

    // overlap removal 
    double min_delta_r = 10; 
    for (int el_n = 0; el_n < buffer.el_n; el_n++) { 
      TLorentzVector el; 
      el.SetPtEtaPhiE(buffer.el_cl_pt->at(el_n), 
		      buffer.el_cl_eta->at(el_n), 
		      buffer.el_cl_phi->at(el_n), 
		      buffer.el_cl_E->at(el_n)); 
      float delta_r = el.DeltaR(*the_jet); 
      if (delta_r < 0.2) { 
	min_delta_r = delta_r; 
	break; 
      }
    }
    if (min_delta_r < 0.2) { 
      the_jet->set_bit(jetbit::el_overlap); 
    }

  }
}

EventJets::~EventJets() { 
  for (iterator itr = begin(); itr != end(); itr++) { 
    delete *itr; 
  }
}

//________________________________________________________________
// main function (for quick checks)

int main(int narg, char* argv[])
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
  unsigned flags = verbose | is_signal | debug_susy; 

  flags |= get_branches; 

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

//______________________________________________________________
// lepton classes

// -- electron --
Electron::Electron(const EventElectrons* container, int index) { 
  const SusyBuffer* buffer = container->m_buffer; 
  
  // calls the susytools electron filler
  m_pass_susy = check_if_electron
    (index , 
     *buffer, 
     *container->m_def, 
     container->m_flags, 
     *container->m_info);

  SUSYObjDef* def = container->m_def; 
  const TLorentzVector& tlv = def->GetElecTLV(index); 
  SetPxPyPzE(tlv.Px(), tlv.Py(), tlv.Pz(), tlv.E()); 
  
}
bool Electron::pass_susy() const { 
  return m_pass_susy; 
}

EventElectrons::EventElectrons(const SusyBuffer& buffer, SUSYObjDef& def, 
			       unsigned flags, const RunInfo& info): 
  m_buffer(&buffer), 
  m_def(&def), 
  m_flags(flags), 
  m_info(&info)
{
  for (int iii = 0; iii < buffer.el_n; iii++) { 
    push_back(new Electron(this, iii)); 
  }
  
}

EventElectrons::~EventElectrons() { 
  for (iterator itr = begin(); itr != end(); itr++) { 
    delete *itr; 
  }
}

// -- muon --

Muon::Muon(const EventMuons* container, int index): 
  m_index(index)
{ 
  const SusyBuffer* buffer = container->m_buffer; 

  // calls the susytools muon filler
  m_pass_susy = check_if_muon(index, 
			      *buffer, 
			      *container->m_def, 
			      container->m_flags); 

  SUSYObjDef* def = container->m_def; 
  const TLorentzVector& tlv = def->GetMuonTLV(index); 
  assert(tlv.Pt() > 0); 
  SetPxPyPzE(tlv.Px(), tlv.Py(), tlv.Pz(), tlv.E()); 
}
bool Muon::pass_susy() const { 
  return m_pass_susy; 
}
int Muon::index() const { 
  return m_index; 
}

EventMuons::EventMuons(const SusyBuffer& buffer, SUSYObjDef& def, 
		       unsigned flags, const RunInfo& info):
  m_buffer(&buffer), 
  m_def(&def), 
  m_flags(flags), 
  m_info(&info)
{
  for (int iii = 0; iii < buffer.mu_staco_n; iii++) { 
    push_back(new Muon(this, iii)); 
  }
}
EventMuons::~EventMuons() { 
  for (iterator itr = begin(); itr != end() ; itr++) { 
    delete *itr; 
  }
}

//______________________________________________________________
// smart chain

SmartChain::SmartChain(std::string tree_name): 
  TChain(tree_name.c_str())
{ 
}

std::vector<std::string> SmartChain::get_all_branch_names() const { 
  return m_set_branches; 
}

void SmartChain::SetBranchAddressPrivate(std::string name, void* branch, 
					 bool turnon) { 

  if (turnon) { 
    SetBranchStatus(name.c_str(), 1); 
  }
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

  m_set_branches.push_back(name);

  if (m_set_branch_set.count(name)) { 
    std::string err = (boost::format("setting branch address: %s is already"
				     " set in %s") % name % GetName()).str(); 
    throw std::runtime_error(err); 
  }
  m_set_branch_set.insert(name); 
}

//_____________________________________________________________________
//            cut counter

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

BitmapCutflow::BitmapCutflow(std::string first_name): 
  m_first_name(first_name)
{ 
}

void BitmapCutflow::add(std::string name, unsigned mask) { 
  m_mask_name.push_back(std::make_pair(mask, name)); 
}

int BitmapCutflow::fill(const unsigned mask) { 
  int n_passed = 0; 
  m_counter[m_first_name]++; 
  for (MaskName::const_iterator itr = m_mask_name.begin(); 
       itr != m_mask_name.end(); itr++) { 
    if (mask & itr->first) { 
      m_counter[itr->second]++; 
      n_passed++; 
    }
    else { 
      break; 
    }
  }
  return n_passed; 
}

std::vector< std::pair<std::string, int> > BitmapCutflow::get() const { 
  return m_counter.get_ordered_cuts(); 
} 

//_______________________________________________________________
// OutTree

OutTree::OutTree(std::string file, std::string tree, const unsigned flags): 
  m_file(0), 
  m_tree(0)
{ 
  if (file.size() > 0) { 
    m_file = new TFile(file.c_str(), "recreate"); 
    m_tree = new TTree(tree.c_str(), tree.c_str()); 
    init(flags); 
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

void OutTree::Jet::clear() 
{
  pt = -1; 
  eta = -1; 
  phi = -1; 
  flavor_truth_label = -1; 
  cnn_b = -1; 
  cnn_c = -1; 
  cnn_u = -1; 
}

void OutTree::init(const unsigned flags) 
{ 
  m_tree->Branch("pass_bits", &pass_bits ); 
  m_tree->Branch("met", &met); 
  m_tree->Branch("min_jetmet_dphi" , &min_jetmet_dphi); 

  leading_jet.set_branches(m_tree, "leading_jet_"); 
  subleading_jet.set_branches(m_tree, "subleading_jet_"); 
  isr_jet.set_branches(m_tree, "isr_jet_"); 
  if (flags & cutflag::raw_evt_info) {
    leading_jet_uncensored.set_branches(m_tree, "leading_jet_uncensored_"); 
  }

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

void OutTree::clear_buffer() { 
  pass_bits = 0; 
  met = -1; 
  min_jetmet_dphi = -1; 
  leading_jet.clear(); 
  subleading_jet.clear(); 
  isr_jet.clear(); 
  leading_jet_uncensored.clear(); 
}

/////////////////////////////////////////////////////////////////////
//////////////           obsolete?         /////////////////////////
/////////////////////////////////////////////////////////////////////



