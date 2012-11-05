#include "cutflow.hh"
#include "SusyBuffer.h"
#include "OutTree.hh"
#include "Jets.hh"
#include "Leptons.hh"
#include "constants.hh"
#include "RunInfo.hh"
#include "BitmapCutflow.hh"
#include "SmartChain.hh"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <string> 
#include <streambuf>
#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <map>
#include "TChain.h"
#include "TVector2.h"
#include "TError.h"
#include <math.h>
#include <stdexcept> 
#include "SUSYTools/SUSYObjDef.h"
#include "SUSYTools/FakeMetEstimator.h"
#include "CutBits.hh"
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
// forward declare utility 

void copy_jet_info(const SelectedJet* , OutTree::Jet&); 

//_________________________________________________________________
// run the cutflow
 
std::vector<std::pair<std::string, int> >
run_cutflow(std::vector<std::string> files, 
	    RunInfo info, const unsigned flags, 
	    std::string out_ntuple_name) {  

  std::ofstream nullstream("/dev/null"); 
  std::ofstream dbg_file("cutflow-dbg.txt"); 
  std::streambuf* out_buffer = nullstream.rdbuf(); 
  if (flags & cutflag::debug_cutflow) { 
    out_buffer = dbg_file.rdbuf(); 
  }
  std::ostream dbg_stream(out_buffer); 

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
  std::string out_file = "/dev/null"; 
  if (flags & cutflag::debug_susy) { 
    out_file = "susy-dbg.txt"; 
  }
  else { 
    gErrorIgnoreLevel = kError;
  }
  freopen(out_file.c_str(), "w", stdout); 

  def.initialize(flags & cutflag::is_data, flags & cutflag::is_atlfast); 
  if (flags & cutflag::no_jet_recal) { 
    def.SetJetCalib(false); 
  }

  // Restore old cout.
  if (flags & cutflag::verbose) { 
    fclose(stdout); 
    freopen("/dev/tty","w",stdout); 
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
  cutflow.add("dphi_jetmet_min"       , pass::dphi_jetmet_min);
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
    
    std::vector<SelectedJet*> susy_jets; 
    for (EventJets::const_iterator jet_itr = all_jets.begin(); 
	 jet_itr != all_jets.end(); 
	 jet_itr++) {
      const SelectedJet& jet = **jet_itr; 
      bool is_low_pt = (jet.bits() & jetbit::low_pt); 
      if (!is_low_pt) { 
	susy_jets.push_back(*jet_itr); 
      }
    }
    
    std::vector<double> jet_dr = remove_overlaping(susy_electrons, 
						   susy_jets, 0.2); 
    remove_overlaping(susy_jets, susy_electrons, 0.4); 
    remove_overlaping(susy_jets, susy_muons, 0.4); 

    for (std::vector<double>::const_iterator dr_itr = jet_dr.begin(); 
	 dr_itr != jet_dr.end(); dr_itr++) { 
      dbg_stream << "evt " << buffer.EventNumber << ", removed jet -- dR = " 
		 << *dr_itr << std::endl; 
    }

    pass_bits |= pass::jet_clean; 
    for (EventJets::const_iterator jet_itr = susy_jets.begin(); 
	 jet_itr != susy_jets.end(); 
	 jet_itr++) { 
      bool is_jet = ((*jet_itr)->bits() & jetbit::good); 
      assert( !( (*jet_itr)->bits() & jetbit::low_pt)); 
      bool is_veto = !is_jet;
      if (is_veto) pass_bits &=~ pass::jet_clean; 
    }
    out_tree.n_susy_jets = susy_jets.size(); 

    if(def.IsGoodVertex(buffer.vx_nTracks)) {
      pass_bits |= pass::vxp_gt_4trk; 
    }
      
    std::vector<int> muon_idx = get_indices(susy_muons); 
    TVector2 met = get_met(buffer, def, info, muon_idx); 
    if (flags & cutflag::use_met_reffinal) { 
      met.Set(buffer.MET_RefFinal_etx, buffer.MET_RefFinal_ety); 
    }
    if (met.Mod() > 120*GeV) { 
      pass_bits |= pass::met; 
    }

    std::vector<SelectedJet*> good_jets; 
    for (EventJets::const_iterator jet_itr = susy_jets.begin(); 
	 jet_itr != susy_jets.end(); jet_itr++) { 

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

    out_tree.n_good_jets = good_jets.size(); 

    if (flags & cutflag::save_truth) { 
      fill_cjet_truth(out_tree, good_jets); 
    }

    if (susy_electrons.size() == 0 && susy_muons.size() == 0) { 
      pass_bits |= pass::lepton_veto; 
    }

    const unsigned n_req_jets = 3; 
    if (good_jets.size() >= n_req_jets) { 
      pass_bits |= pass::n_jet; 

      std::vector<SelectedJet*> leading_jets(good_jets.begin(), 
					     good_jets.begin() + n_req_jets); 

      assert(leading_jets.size() == n_req_jets);

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
    out_tree.met_phi = met.Phi(); 

    out_tree.hfor_type = buffer.hfor_type; 
    out_tree.event_number = buffer.EventNumber; 

    out_tree.fill(); 
 
  }
  std::cout << "\n"; 
  
  // restore cout if not already done
  if (!flags & cutflag::verbose) { 
    fclose(stdout); 
    freopen("/dev/stdout","w",stdout); 
  }
  
  dbg_file.close(); 

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

void fill_cjet_truth(OutTree& out_tree, 
		     const std::vector<SelectedJet*>& jets)
{
  int leading_pos = -1; 
  int subleading_pos = -1; 
  int n_cjet = 0; 
  for (unsigned jet_pos = 0; jet_pos < jets.size(); jet_pos++) { 
    const SelectedJet& jet = *jets.at(jet_pos); 
    if (jet.flavor_truth_label() == 4) { 
      if (leading_pos == -1) { 
	leading_pos = jet_pos; 
      }
      else if (subleading_pos == -1){ 
	subleading_pos = jet_pos; 
      }
      n_cjet++; 
    } // end charm check
  } // end jet loop
  out_tree.leading_cjet_pos = leading_pos; 
  out_tree.subleading_cjet_pos = subleading_pos; 
  out_tree.n_cjet = n_cjet; 
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
    float deltaphi = fabs(mector.DeltaPhi(**itr)); 
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

  assert(in->Pt()); 
  jet.pt = in->Pt(); 
  jet.eta = in->Eta(); 
  jet.phi = in->Phi(); 
  jet.cnn_b = in->pb(); 
  jet.cnn_c = in->pc(); 
  jet.cnn_u = in->pu(); 
  jet.flavor_truth_label = in->flavor_truth_label(); 
  jet.cnn_log_cu = log( in->pc() / in->pu() ) ; 
  jet.cnn_log_cb = log( in->pc() / in->pb() ) ; 
}



//______________________________________________________________________
// object check functions 
  
  
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
  flags |= save_ratios; 
  // flags |= raw_evt_info; 
  // flags |= use_met_reffinal; 
  flags |= verbose; 
  flags |= is_atlfast; 
  flags |= debug_cutflow; 
  flags |= save_truth; 

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


//_____________________________________________________________________
//            cut counter



/////////////////////////////////////////////////////////////////////
//////////////           obsolete?         /////////////////////////
/////////////////////////////////////////////////////////////////////



