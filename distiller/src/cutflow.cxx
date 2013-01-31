#include "cutflow.hh"
#include "SusyBuffer.h"
#include "OutTree.hh"
#include "Jets.hh"
#include "Leptons.hh"
#include "constants.hh"
#include "RunInfo.hh"
#include "BitmapCutflow.hh"
#include "SmartChain.hh"
#include "CollectionTreeReport.hh"
#include "EventPreselector.hh"

#include "RunBits.hh"
#include "EventBits.hh"
#include "EventConstants.hh"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <string> 
#include <streambuf>
#include <cassert>
#include <cstdlib> // getenv, others
#include <cstdio>
#include <map>
#include "TChain.h"
#include "TVector2.h"
#include "TError.h"
#include <math.h>
#include <stdexcept> 
#include "SUSYTools/SUSYObjDef.h"
#include "SUSYTools/FakeMetEstimator.h"

#include <boost/format.hpp>
#include <boost/scoped_ptr.hpp>

//_________________________________________________________________
// forward declare utility 

void copy_jet_info(const SelectedJet* , OutTree::Jet&); 

//_________________________________________________________________
// run the cutflow
 
std::vector<std::pair<std::string, int> >
run_cutflow(std::vector<std::string> files, 
	    RunInfo info, unsigned flags, 
	    std::string out_ntuple_name) {  
  gErrorIgnoreLevel = kWarning;
  typedef std::vector<SelectedJet*> Jets; 
  std::ofstream nullstream("/dev/null"); 
  std::ofstream dbg_file("cutflow-dbg.txt"); 
  std::streambuf* out_buffer = nullstream.rdbuf(); 
  if (flags & cutflag::debug_cutflow) { 
    out_buffer = dbg_file.rdbuf(); 
  }
  std::ostream dbg_stream(out_buffer); 

  boost::scoped_ptr<SmartChain> input_chain(new SmartChain("susy")); 
  CollectionTreeReport ct_report("CollectionTree"); 

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
  ct_report.add_files(files); 


  SUSYObjDef def; 

  if (! (flags & cutflag::is_data) ) { 
    flags |= (cutflag::truth | cutflag::spartid); 
  }

  BranchNames branch_names; 
  branch_names.trigger = "EF_xe80_tclcw_loose"; 

  SusyBuffer buffer(input_chain.get(), flags, branch_names); 
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

  // --- redirect stdout for the initialization of annoying root tools ---

  std::string out_file = "/dev/null"; 
  if (flags & cutflag::debug_susy) { 
    out_file = "susy-dbg.txt"; 
  }
  else { 
    gErrorIgnoreLevel = kError;
  }
  int output_dup = dup(fileno(stdout)); 
  freopen(out_file.c_str(), "w", stdout); 

  EventPreselector event_preselector(flags, info.grl); 
  def.initialize(flags & cutflag::is_data, flags & cutflag::is_atlfast); 
  if (flags & cutflag::no_jet_recal) { 
    def.SetJetCalib(false); 
  }

  // Restore old cout.
  if (flags & cutflag::verbose) { 
    dup2(output_dup, fileno(stdout)); 
    close(output_dup); 
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

    // --- preselection 

    pass_bits |= event_preselector.get_preselection_flags(buffer); 

    // --- object selection 

    std::sort(all_jets.begin(),all_jets.end(),has_lower_pt); 
    std::reverse(all_jets.begin(), all_jets.end()); 

    if (all_jets.size()) { 
      copy_jet_info(all_jets.at(0), out_tree.leading_jet_uncensored); 
    }
    
    Jets preselection_jets; 
    for (EventJets::const_iterator jet_itr = all_jets.begin(); 
	 jet_itr != all_jets.end(); 
	 jet_itr++) {
      const SelectedJet& jet = **jet_itr; 
      bool is_low_pt = (jet.bits() & jetbit::low_pt); 
      bool is_high_eta = (jet.bits() & jetbit::high_eta); 
      if (!is_low_pt && !is_high_eta) { 
	preselection_jets.push_back(*jet_itr); 
      }
    }

    // need to get susy muon indices before overlap
    std::vector<int> susy_muon_idx = get_indices(susy_muons); 
    
    std::vector<double> jet_dr = remove_overlaping(susy_electrons, 
						   preselection_jets, 
						   REMOVE_JET_CONE); 
    remove_overlaping(preselection_jets, susy_electrons, REMOVE_EL_CONE); 
    remove_overlaping(preselection_jets, susy_muons, REMOVE_MU_CONE); 

    for (std::vector<double>::const_iterator dr_itr = jet_dr.begin(); 
	 dr_itr != jet_dr.end(); dr_itr++) { 
      dbg_stream << "evt " << buffer.EventNumber << ", removed jet -- dR = " 
		 << *dr_itr << std::endl; 
    }

    Jets signal_jets; 
    for (Jets::const_iterator jet_itr = preselection_jets.begin(); 
	 jet_itr != preselection_jets.end(); jet_itr++) { 

      const SelectedJet& jet = **jet_itr; 
      bool signal_pt = (jet.bits() & jetbit::signal_pt); 
      bool tag_eta = (jet.bits() & jetbit::taggable_eta);
      bool leading_jet = (*jet_itr == *preselection_jets.begin()); 
      if (signal_pt && (tag_eta || leading_jet)) { 
	signal_jets.push_back(*jet_itr); 
	bool good_isr_pt = jet.Pt() > 120*GeV;
	if (good_isr_pt) { 
	  pass_bits |= pass::leading_jet; 
	}
      }
    }
    set_bit(signal_jets, jetbit::good); 

    TVector2 met = get_met(buffer, def, info, susy_muon_idx); 
    if (flags & cutflag::use_met_reffinal) { 
      met.Set(buffer.MET_RefFinal_etx, buffer.MET_RefFinal_ety); 
    }

    // ----- object selection is done now, from here is filling outputs ---

    out_tree.n_susy_jets = preselection_jets.size(); 
    pass_bits |= jet_cleaning_bit(preselection_jets); 

    const unsigned n_req_jets = 3; 
    if (signal_jets.size() >= n_req_jets) { 
      pass_bits |= pass::n_jet; 

      Jets leading_jets(signal_jets.begin(), 
			signal_jets.begin() + n_req_jets); 
      set_bit(leading_jets, jetbit::leading); 
      assert(leading_jets.size() == n_req_jets);

      out_tree.htx = get_htx(preselection_jets); 

      out_tree.min_jetmet_dphi = get_min_jetmet_dphi(leading_jets, met); 
      if (out_tree.min_jetmet_dphi > MIN_DPHI_JET_MET) { 
	pass_bits |= pass::dphi_jetmet_min; 
      }

    }

    copy_leading_jet_info(signal_jets, out_tree); 
    pass_bits |= get_ctag_bits(signal_jets); 

    if(def.IsGoodVertex(buffer.vx_nTracks)) {
      pass_bits |= pass::vxp_gt_4trk; 
    }
    if (susy_electrons.size() == 0 && susy_muons.size() == 0) { 
      pass_bits |= pass::lepton_veto; 
    }
    if (met.Mod() > 120*GeV) { 
      pass_bits |= pass::met; 
    }

    if ( flags & cutflag::truth ) { 
      fill_cjet_truth(out_tree, signal_jets); 
      fill_event_truth(out_tree, buffer, flags); 
    }

    cutflow.fill(pass_bits); 
    out_tree.pass_bits = pass_bits; 
    out_tree.met = met.Mod(); 
    out_tree.met_phi = met.Phi(); 

    out_tree.event_number = buffer.EventNumber; 

    out_tree.fill(); 
 
  }
  // restore cout if not already done
  if (flags & cutflag::verbose) { 
    std::cout << "\n";  
  } 
  else {  
    dup2(output_dup, fileno(stdout)); 
    close(output_dup); 
  }
  
  dbg_file.close(); 

  def.finalize(); 

  typedef std::pair<std::string, int> Cut; 
  Cut total_events(std::make_pair("total_events", ct_report.total_entries()));
  std::vector<Cut> cutflow_vec = cutflow.get(); 
  cutflow_vec.insert(cutflow_vec.begin(),total_events); 
  return cutflow_vec;
	 
}

void copy_leading_jet_info(const std::vector<SelectedJet*>& signal_jets, 
			   OutTree& out_tree)
{
  if (signal_jets.size() >= 1) { 
    copy_jet_info(signal_jets.at(0), out_tree.isr_jet); 
  }
  if (signal_jets.size() >= 2) { 
    copy_jet_info(signal_jets.at(1), out_tree.leading_jet);
  }
  if (signal_jets.size() >= 3) { 
    copy_jet_info(signal_jets.at(2), out_tree.subleading_jet); 
  }
  out_tree.n_good_jets = signal_jets.size(); 

}

unsigned jet_cleaning_bit(const std::vector<SelectedJet*>& preselection_jets)
{
  typedef std::vector<SelectedJet*> Jets; 
  unsigned pass_bits = pass::jet_clean; 
  for (Jets::const_iterator jet_itr = preselection_jets.begin(); 
       jet_itr != preselection_jets.end(); 
       jet_itr++) { 
    bool clean_jet = ((*jet_itr)->bits() & jetbit::susy); 
    assert( !( (*jet_itr)->bits() & jetbit::low_pt)); 
    assert( !( (*jet_itr)->bits() & jetbit::high_eta)); 
    if (!clean_jet) pass_bits &=~ pass::jet_clean; 
  }
  return pass_bits; 
}

void fill_event_truth(OutTree& out_tree, const SusyBuffer& buffer, 
		      unsigned flags) { 
  out_tree.hfor_type = buffer.hfor_type; 
  if (flags & cutflag::spartid) { 
    out_tree.spart1_pdgid = buffer.spart1_pdgid; 
    out_tree.spart2_pdgid = buffer.spart2_pdgid; 
  }

  if (fabs(buffer.mc_event_weight) < 2.0) { 
    out_tree.mc_event_weight = buffer.mc_event_weight; 
  }
  else { 
    out_tree.mc_event_weight = 1.0; 
  }
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

//___________________________________________________________
// ctag bits 

bool pass_mainz_ctag(const SelectedJet* jet){ 
  bool pass = (jet->combNN_btag() > -2.55 && 
	       jet->combNN_btag() < 1.0); 
  return pass; 
}
bool pass_anti_b_ctag(const SelectedJet* jet) { 
  return log(jet->pc() / jet->pb() ) > COMBNN_LOG_CB_CUT; 
}
bool pass_anti_u_ctag(const SelectedJet* jet, float cut) { 
  return log(jet->pc() / jet->pu() ) > cut; 
}
unsigned get_ctag_bits(const std::vector<SelectedJet*>& signal_jets) 
{ 
  const float MED = COMBNN_MED_LOG_CU_CUT; 
  const float TGT = COMBNN_TIGHT_LOG_CU_CUT; 
  unsigned pass_bits = 0; 
  if (signal_jets.size() >= 1) { 
    const SelectedJet* jet = signal_jets.at(0); 
    if (pass_anti_b_ctag(jet))      pass_bits |= pass::jet1_anti_b; 
    if (pass_anti_u_ctag(jet, MED)) pass_bits |= pass::jet1_anti_u_medium; 
    if (pass_anti_u_ctag(jet, TGT)) pass_bits |= pass::jet1_anti_u_tight; 
  }
  if (signal_jets.size() >= 2) { 
    const SelectedJet* jet = signal_jets.at(1); 
    if (pass_anti_b_ctag(jet))      pass_bits |= pass::jet2_anti_b; 
    if (pass_anti_u_ctag(jet, MED)) pass_bits |= pass::jet2_anti_u_medium; 
    if (pass_anti_u_ctag(jet, TGT)) pass_bits |= pass::jet2_anti_u_tight; 
    if (pass_mainz_ctag(jet))       pass_bits |= pass::ctag_mainz; 
  }
  if (signal_jets.size() >= 3) { 
    const SelectedJet* jet = signal_jets.at(2); 
    if (pass_anti_b_ctag(jet))      pass_bits |= pass::jet3_anti_b; 
    if (pass_anti_u_ctag(jet, MED)) pass_bits |= pass::jet3_anti_u_medium; 
    if (pass_anti_u_ctag(jet, TGT)) pass_bits |= pass::jet3_anti_u_tight; 
    if (pass_mainz_ctag(jet))       pass_bits |= pass::ctag_mainz; 
  }
  return pass_bits; 
}


//____________________________________________________________
// utility functions 



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


double get_htx(const std::vector<SelectedJet*>& jets){ 
  double htx = 0; 
  const unsigned required_bits = jetbit::susy; 
  const unsigned veto_bits = jetbit::leading; 
  for (std::vector<SelectedJet*>::const_iterator itr = jets.begin(); 
       itr != jets.end(); itr++) { 
    unsigned jet_bits = (*itr)->bits(); 
    bool has_required = ( (jet_bits & required_bits) == required_bits); 
    bool has_veto = ( (jet_bits & veto_bits) != 0); 
    if (!has_veto && has_required) { 
      htx += (*itr)->Pt(); 
    }
  }
  return htx; 
}

bool has_lower_pt(const TLorentzVector* v1, const TLorentzVector* v2) { 
  return v1->Pt() < v2->Pt(); 
}

void set_bit(std::vector<SelectedJet*>& jets, unsigned bit) { 
  for (std::vector<SelectedJet*>::iterator itr = jets.begin(); 
       itr != jets.end(); itr++) { 
    (*itr)->set_bit(bit); 
  }
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
  if (in->has_truth()) { 
    jet.flavor_truth_label = in->flavor_truth_label(); 
  }
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
  info.grl = "grl.xml"; 
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

  flags |= is_data; 

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



