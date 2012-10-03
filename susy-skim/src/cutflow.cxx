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

  typedef std::vector<SelectedJet>::const_iterator JetItr; 

  SUSYObjDef def; 

  // unsigned branches = branches::run_num | branches::trigger; 
  unsigned branches = branches::all; 

  BranchNames branch_names; 
  branch_names.trigger = "EF_xe80_tclcw_loose"; 

  SusyBuffer buffer(input_chain.get(), branches, branch_names); 
  int n_entries = input_chain->GetEntries(); 


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

  // Restore old cout.
  if (flags & cutflag::verbose) { 
    std::cerr.rdbuf( old_err_stream ); 
    std::cout.rdbuf( old_out_stream );
  }


  // --- output things ---
  OutTree out_tree(out_ntuple_name); 
  CutCounter cut_counters; 
  
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
    
    SelectedJets selected_jets(buffer, def, flags, info); 

    unsigned pass_bits = 0; 
  
    cut_counters["n_events"]++;
    pass_bits |= pass::grl; 

    if(!buffer.trigger)   pass_bits |= pass::trigger; 
    if(!buffer.larError)  pass_bits |= pass::lar_error; 

    if ( !(flags & cutflag::is_data ) ||
	 ((buffer.coreFlags & 0x40000) == 0) ){ 
      pass_bits |= pass::core; 
    }
    
    pass_bits |= pass::jet_clean; 
    for (SelectedJets::const_iterator jet_itr = selected_jets.begin(); 
	 jet_itr != selected_jets.end(); 
	 jet_itr++) { 
      bool is_jet = (jet_itr->get_bits() & jetbit::good); 
      bool is_low_pt = (jet_itr->get_bits() & jetbit::low_pt); 
      bool is_overlap = (jet_itr->get_bits() & jetbit::el_overlap); 
      bool is_veto = !is_jet && !is_low_pt && !is_overlap; 
      if (is_veto) pass_bits &=~ pass::jet_clean; 
    }

    if(def.IsGoodVertex(buffer.vx_nTracks)) {
      pass_bits |= pass::vxp_gt_4trk; 
    }
        
    double max_jet_pt = 0; 
    for (SelectedJets::const_iterator jet_itr = selected_jets.begin(); 
	 jet_itr != selected_jets.end(); jet_itr++) { 
      bool in_eta = fabs(jet_itr->Eta()) < 2.5; 
      bool good_jvf = jet_itr->jvf() > 0.5; 

      if ( in_eta && good_jvf) { 
	max_jet_pt = std::max(jet_itr->Pt(), max_jet_pt); 
      }
    }
    if (max_jet_pt > 120*GeV) { 
      pass_bits |= pass::leading_jet; 
    }
      
    int n_electrons = fill_electrons(buffer, def, flags, info); 
    std::vector<int> muon_idx = fill_muons(buffer, def, flags, info); 

    TVector2 met = get_met(buffer, def, info, muon_idx); 

    if (met.Mod() > 120*GeV) { 
      pass_bits |= pass::met; 
    }

    SelectedJets good_jets; 
    for (SelectedJets::const_iterator jet_itr = selected_jets.begin(); 
	 jet_itr != selected_jets.end(); jet_itr++) { 

      bool good_pt = jet_itr->Pt() > 30*GeV; 
      bool good_eta = fabs(jet_itr->Eta()) < 2.5;
      bool good_jvf = jet_itr->jvf() > 0.5; 
      if (good_pt && good_eta && good_jvf) { 
	good_jets.push_back(*jet_itr); 
      }
    }

    if (n_electrons == 0 && muon_idx.size() == 0) { 
      pass_bits |= pass::lepton_veto; 
    }

    if (good_jets.size() >= 3) { 
      pass_bits |= pass::n_jet; 

      float dphi = get_min_jetmet_dphi(good_jets, met); 
      if (dphi > 0.4) { 
	pass_bits |= pass::dphi_jetmet; 
      }

      std::sort(good_jets.begin(), good_jets.end(), has_lower_pt); 
      std::reverse(good_jets.begin(), good_jets.end()); 

      const SelectedJet& leading_jet = good_jets.at(1); 
      const SelectedJet& subleading_jet = good_jets.at(2); 
      const SelectedJet& isr_jet = good_jets.at(0); 

      if (pass_mainz_ctag(leading_jet) || 
	  pass_mainz_ctag(subleading_jet)) { 
	pass_bits |= pass::ctag_mainz; 
      }


      copy_jet_info(leading_jet, buffer, out_tree.leading_jet); 
      copy_jet_info(subleading_jet, buffer, out_tree.subleading_jet); 
      copy_jet_info(isr_jet, buffer, out_tree.isr_jet); 
      out_tree.min_jetmet_dphi = dphi; 

    } // end if n_jets >= 3

    fill_cutflow_from_bits(cut_counters, pass_bits); 

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
  return cut_counters.get_ordered_cuts(); 
	 
}

//____________________________________________________________
// utility functions 

bool pass_mainz_ctag(const SelectedJet& jet){ 

  bool pass = (jet.combNN_btag() > -2.55 && 
	       jet.combNN_btag() < 1.0); 
  return pass; 
}


float get_min_jetmet_dphi(const std::vector<SelectedJet>& jets, 
			  const TVector2& met){ 
  TLorentzVector mector; 
  mector.SetPtEtaPhiE(1,0,met.Phi(),1); 
  float min_dphi = M_PI; 
  for(unsigned iii = 0; iii <3; iii++){
    float deltaphi = mector.DeltaPhi(jets.at(iii)); 
    min_dphi = std::min(deltaphi, min_dphi); 
  }
  return min_dphi; 
}

float get_sum_jetmet_dphi(const std::vector<SelectedJet>& jets, 
			  const TVector2& met){ 
  TLorentzVector sum; 
  for(unsigned iii = 0; iii <3 ; iii++) {
    sum += jets.at(iii); 
  }
  TLorentzVector mector; 
  mector.SetPtEtaPhiE(1,0,met.Phi(),1); 
  return mector.DeltaPhi(sum); 
}

bool has_lower_pt(const TLorentzVector& v1, const TLorentzVector& v2) { 
  return v1.Pt() < v2.Pt(); 
}



//___________________________________________________________
// object fillers 

int fill_electrons(const SusyBuffer& buffer, SUSYObjDef& def, 
		   unsigned flags, const RunInfo& info)
{ 
  int n_electrons = 0; 
  for (int iii = 0; iii < buffer.el_n; iii++) { 
    bool is_electron = check_if_electron(iii, buffer, def, flags, info); 
    if (is_electron) { 
      n_electrons++; 
    }
  }
  return n_electrons; 
}

std::vector<int> fill_muons(const SusyBuffer& buffer, SUSYObjDef& def, 
			    unsigned flags, const RunInfo& info)
{ 
  std::vector<int> muon_idx; 
  for (int iii = 0; iii < buffer.mu_staco_n; iii++) { 
    bool is_muon = check_if_muon(iii, buffer, def, flags); 
    if (is_muon) { 
      muon_idx.push_back(iii); 
    }
  }
  return muon_idx; 
}


//____________________________________________________________________
// io functions 

void copy_jet_info(const SelectedJet& in, const SusyBuffer& buffer, 
		   OutTree::Jet& jet)
{
  int jet_index = in.jet_index(); 
  jet.pt = in.Pt(); 
  jet.eta = in.Eta(); 
  jet.phi = in.Phi(); 
  jet.cnn_b = 
    buffer.jet_flavor_component_jfitcomb_pb->at(jet_index); 
  jet.cnn_c = 
    buffer.jet_flavor_component_jfitcomb_pc->at(jet_index); 
  jet.cnn_u = 
    buffer.jet_flavor_component_jfitcomb_pu->at(jet_index); 
  jet.flavor_truth_label = 
    buffer.jet_flavor_truth_label->at(jet_index); 
}



int fill_cutflow_from_bits(CutCounter& cut_counters, const unsigned bits) { 
  using namespace pass; 

  if (! (bits & grl        ) ) return 0; 
  cut_counters["GRL"]++;
  if (! (bits & trigger    ) ) return 0; 
  cut_counters["trigger"]++;
  if (! (bits & lar_error    ) ) return 0; 
  cut_counters["lar_error"]++;
  if (! (bits & core    ) ) return 0; 
  cut_counters["core"]++;
  if (! (bits & jet_clean    ) ) return 0; 
  cut_counters["jet_clean"]++;
  if (! (bits & vxp_gt_4trk    ) ) return 0; 
  cut_counters["vxp_gt_4trk"]++;
  if (! (bits & leading_jet    ) ) return 0; 
  cut_counters["leading_jet"]++;
  if (! (bits & met    ) ) return 0; 
  cut_counters["met"]++;
  if (! (bits & n_jet    ) ) return 0; 
  cut_counters["n_jet_gt_3"]++;
  if (! (bits & dphi_jetmet    ) ) return 0; 
  cut_counters["dphi_jetmet"]++;
  if (! (bits & lepton_veto    ) ) return 0; 
  cut_counters["lepton_veto"]++;
  if (! (bits & ctag_mainz    ) ) return 0; 
  cut_counters["ctag_mainz"]++;
  return 1; 

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
  
SelectedJet::SelectedJet(const SelectedJets* parent, int jet_index): 
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
    
  m_combNN_btag_wt = 
    buffer.jet_flavor_weight_JetFitterCOMBNN->at(jet_index); 
}
  
double SelectedJet::combNN_btag() const { 
  //std::cerr << "ERROR: this isn't defined yet\n"; 
  //assert(false); 

  return m_combNN_btag_wt;
}
  
double SelectedJet::jfitcomb_cu(const SusyBuffer& buffer, 
				int jet_index) const {

  double cu; 
  cu = buffer.jet_flavor_component_jfitcomb_pc->at(jet_index)
    / buffer.jet_flavor_component_jfitcomb_pu->at(jet_index);

  return log(cu);
}

double SelectedJet::jfitcomb_cb(const SusyBuffer& buffer, 
				int jet_index) const {

  double cb; 
  cb = buffer.jet_flavor_component_jfitcomb_pc->at(jet_index) / buffer.jet_flavor_component_jfitcomb_pb->at(jet_index);
  return log(cb); 
}
 
int SelectedJet::jet_index() const{
  return m_jet_index;
}

double SelectedJet::jvf() const { 
  return m_jvf; 
}

unsigned SelectedJet::get_bits() const { 
  return m_bits; 
}

void SelectedJet::set_bit(unsigned bit) { 
  m_bits |= bit; 
}

SelectedJets::SelectedJets() {}; 

SelectedJets::SelectedJets(const SusyBuffer& buffer, SUSYObjDef& def, 
			   const unsigned flags, const RunInfo& info): 
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

    SelectedJet the_jet(this, jet_n); 

    if (is_jet) { 
      the_jet.set_bit(jetbit::good); 
    }
    
    bool low_pt_jet = the_jet.Pt() < 20*GeV;
    if (low_pt_jet) { 
      the_jet.set_bit(jetbit::low_pt); 
    }
    

    // overlap removal 
    double min_delta_r = 10; 
    for (int el_n = 0; el_n < buffer.el_n; el_n++) { 
      TLorentzVector el; 
      el.SetPtEtaPhiE(buffer.el_cl_pt->at(el_n), 
		      buffer.el_cl_eta->at(el_n), 
		      buffer.el_cl_phi->at(el_n), 
		      buffer.el_cl_E->at(el_n)); 
      float delta_r = el.DeltaR(the_jet); 
      if (delta_r < 0.2) { 
	min_delta_r = delta_r; 
	break; 
      }
    }
    if (min_delta_r < 0.2) { 
      the_jet.set_bit(jetbit::el_overlap); 
    }

    push_back(the_jet); 
  }
}



//________________________________________________________________
// main function (for quick checks)

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
  unsigned flags = verbose | is_signal | debug_susy; 


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
// smart chain

SmartChain::SmartChain(std::string tree_name): 
  TChain(tree_name.c_str())
{ 
}

// void SmartChain::SetBranchAddress(std::string name, void* branch, 
// 				  bool turnon ) { 
//   SetBranchAddressPrivate(name, branch, turnon); 
// }


void SmartChain::SetBranchAddressPrivate(std::string name, void* branch, 
					 bool turnon) { 
  // printf("setting %s\n", name.c_str());  

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



//_______________________________________________________________
// OutTree

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

void OutTree::init() 
{ 
  m_tree->Branch("pass_bits", &pass_bits ); 
  m_tree->Branch("met", &met); 
  m_tree->Branch("min_jetmet_dphi" , &min_jetmet_dphi); 

  leading_jet.set_branches(m_tree, "leading_jet_"); 
  subleading_jet.set_branches(m_tree, "subleading_jet_"); 
  isr_jet.set_branches(m_tree, "isr_jet_"); 

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
}

/////////////////////////////////////////////////////////////////////
//////////////           obsolete?         /////////////////////////
/////////////////////////////////////////////////////////////////////


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

  // NOTE: Triggers have been removed from SusyBuffer

  if(rand_float < m_frac_preswap){
    // if(buffer.EF_xe70_noMu) return true;
  }
  else{
    // if(buffer.EF_xe60_verytight_noMu) return true;
  }
  return false; 

}; 

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
       (buffer.jet_pt            ->at(d3pd_index),
	buffer.jet_BCH_CORR_JET  ->at(d3pd_index),
	buffer.jet_BCH_CORR_CELL ->at(d3pd_index),
	buffer.jet_BCH_CORR_DOTX ->at(d3pd_index),
	buffer.jet_phi           ->at(d3pd_index),
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

