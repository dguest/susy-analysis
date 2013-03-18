#include "StopDistiller.hh"
#include "distill_tools.hh"

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
#include "BtagCalibration.hh"
#include "CheckSize.hh"

#include "RunBits.hh"
#include "EventBits.hh"
#include "DistillerConstants.hh"

#include <iostream>
#include <fstream>
#include <string> 
#include <streambuf>
#include <cassert>
#include <cstdlib> // getenv, others
#include <cstdio>
#include <map>

#include "TVector2.h"
#include "TError.h"

#include <stdexcept> 
#include "SUSYTools/SUSYObjDef.h"
#include "SUSYTools/FakeMetEstimator.h"

#include <boost/format.hpp>


StopDistiller::StopDistiller(const std::vector<std::string>& in, 
			     const RunInfo& info, unsigned flags, 
			     std::string out): 
  m_info(info), 
  m_flags(flags), 
  m_out_ntuple_name(out), 
  m_susy_dbg_file("susy-debug.txt"), 
  m_required_for_save(0), 
  m_norm_dbg_file(0), 
  m_null_file(new std::ofstream("/dev/null")), 
  m_chain(0), 
  m_ct_report(0), 
  m_susy_buffer(0), 
  m_def(0), 
  m_event_preselector(0), 
  m_out_tree(0), 
  m_cutflow(0), 
  m_btag_calibration(0)
{ 
  gErrorIgnoreLevel = kWarning;
  setup_flags(); 
  setup_streams(); 
  setup_chain(in); 
  setup_susytools(); 
  setup_outputs(); 
}

StopDistiller::~StopDistiller() { 
  if (m_norm_dbg_file) { 
    m_norm_dbg_file->close(); 
    delete m_norm_dbg_file; 
  }
  delete m_null_file; 
  delete m_chain; 
  delete m_ct_report; 
  delete m_susy_buffer; 
  if (m_def) { 
    m_def->finalize(); 
    delete m_def; 
  }

  delete m_event_preselector; 
  delete m_out_tree; 
  delete m_cutflow; 
  delete m_btag_calibration; 
}

StopDistiller::Cutflow StopDistiller::run_cutflow() { 
  m_n_entries = m_chain->GetEntries(); 
  m_one_percent = m_n_entries / 100; 

  // first setup my debug stream
  std::ostream debug_stream(m_debug_buffer); 

  // redirect the stdout stuff 
  int output_dup = dup(fileno(stdout)); 
  if ( ! (m_flags & cutflag::verbose)) { 
    freopen("/dev/null", "w", stdout); 
  }

  // ------- event loop -------
  int n_error = 0; 
  for (int evt_n = 0; evt_n < m_n_entries; evt_n++) { 
    try { 
      process_event(evt_n, debug_stream); 
    }
    catch (EventReadError& e) { 
      n_error++; 
    }
    catch (std::logic_error& e) { 
      std::string err_tpl = "logic error in %s, event %i: %s"; 
      std::string err = (boost::format(err_tpl) % 
			 m_chain->get_current_file() % 
			 evt_n % e.what()).str();
      throw std::logic_error(err); 
    }
  }
  if (m_flags & cutflag::verbose) { 
    std::cout << "\n";  
  } 
  else { 
    dup2(output_dup, fileno(stdout)); 
    close(output_dup); 
  }

  typedef std::pair<std::string, int> Cut; 
  Cut total_events(std::make_pair("total_events", 
				  m_ct_report->total_entries()));
  std::vector<Cut> cutflow_vec = m_cutflow->get(); 
  cutflow_vec.insert(cutflow_vec.begin(),total_events); 
  if (n_error) { 
    cutflow_vec.push_back(std::make_pair("read_errors",n_error)); 
  }
  return cutflow_vec;

}

void StopDistiller::process_event(int evt_n, std::ostream& dbg_stream) { 
  if (m_flags & cutflag::verbose) { 
    print_progress(evt_n, std::cout); 
  }
  m_chain->GetEntry(evt_n); 

  m_def->Reset(); 
  m_out_tree->clear_buffer(); 
    
  EventJets all_jets(*m_susy_buffer, *m_def, m_flags, m_info); 
  EventElectrons all_electrons(*m_susy_buffer, *m_def, m_flags, m_info); 
  EventMuons all_muons(*m_susy_buffer, *m_def, m_flags, m_info); 

  std::vector<Electron*> preselected_electrons = filter_susy(all_electrons); 
  std::vector<Muon*> preselected_muons = filter_susy(all_muons); 

  ull_t pass_bits = 0; 

  // --- preselection 

  pass_bits |= m_event_preselector->get_preselection_flags(*m_susy_buffer); 

  // --- object selection 

  std::sort(all_jets.begin(),all_jets.end(),has_lower_pt); 
  std::reverse(all_jets.begin(), all_jets.end()); 

  if (all_jets.size()) { 
    all_jets.at(0)->set_flavor_tag(m_btag_calibration); 
    copy_jet_info(all_jets.at(0), m_out_tree->leading_jet_uncensored); 
  }
    
  Jets preselection_jets; 
  for (auto jet_itr = all_jets.begin(); jet_itr != all_jets.end(); jet_itr++) {
    const SelectedJet& jet = **jet_itr; 
    bool is_low_pt = (jet.bits() & jetbit::low_pt); 
    bool is_high_eta = (jet.bits() & jetbit::high_eta); 
    if (!is_low_pt && !is_high_eta) { 
      preselection_jets.push_back(*jet_itr); 
    }
  }

  // need to get susy muon indices before overlap
  std::vector<int> susy_muon_idx = get_indices(preselected_muons); 
    
  std::vector<double> jet_dr = remove_overlaping(preselected_electrons, 
						 preselection_jets, 
						 REMOVE_JET_CONE); 
  remove_overlaping(preselection_jets, preselected_electrons, REMOVE_EL_CONE); 
  remove_overlaping(preselection_jets, preselected_muons, REMOVE_MU_CONE); 

  for (auto dr_itr = jet_dr.begin(); 
       dr_itr != jet_dr.end(); dr_itr++) { 
    dbg_stream << "evt " << m_susy_buffer->EventNumber 
	       << ", removed jet -- dR = " 
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

  TVector2 met = get_met(*m_susy_buffer, *m_def, m_info, susy_muon_idx); 
  if (m_flags & cutflag::use_met_reffinal) { 
    met.Set(m_susy_buffer->MET_RefFinal_etx, 
	    m_susy_buffer->MET_RefFinal_ety); 
  }

  // ----- object selection is done now, from here is filling outputs ---

  calibrate_jets(signal_jets, m_btag_calibration); 

  m_out_tree->n_susy_jets = preselection_jets.size(); 
  pass_bits |= jet_cleaning_bit(preselection_jets); 

  const unsigned n_req_jets = 3; 
  if (signal_jets.size() >= n_req_jets) { 
    pass_bits |= pass::n_jet; 

    Jets leading_jets(signal_jets.begin(), 
		      signal_jets.begin() + n_req_jets); 
    set_bit(leading_jets, jetbit::leading); 
    assert(leading_jets.size() == n_req_jets);

    m_out_tree->htx = get_htx(preselection_jets); 

    m_out_tree->min_jetmet_dphi = get_min_jetmet_dphi(leading_jets, met); 
    if (m_out_tree->min_jetmet_dphi > MIN_DPHI_JET_MET) { 
      pass_bits |= pass::dphi_jetmet_min; 
    }

  }

  copy_leading_jet_info(signal_jets, *m_out_tree); 
  pass_bits |= get_ctag_bits(signal_jets); 

  if(m_def->IsGoodVertex(m_susy_buffer->vx_nTracks)) {
    pass_bits |= pass::vxp_gt_4trk; 
  }
  if (preselected_electrons.size() == 0 && preselected_muons.size() == 0) { 
    pass_bits |= pass::lepton_veto; 
  }
  if (met.Mod() > 120*GeV) { 
    pass_bits |= pass::met; 
  }

  if ( m_flags & cutflag::truth ) { 
    fill_cjet_truth(*m_out_tree, signal_jets); 
    fill_event_truth(*m_out_tree, *m_susy_buffer, m_flags); 
  }

  m_cutflow->fill(pass_bits); 
  m_out_tree->pass_bits = pass_bits; 
  m_out_tree->met = met.Mod(); 
  m_out_tree->met_phi = met.Phi(); 

  m_out_tree->event_number = m_susy_buffer->EventNumber; 

  if (m_flags & cutflag::save_all_events) { 
    m_out_tree->fill(); 
  }
  else if ( (pass_bits & m_required_for_save) == m_required_for_save) { 
    m_out_tree->fill(); 
  }

}


void StopDistiller::setup_flags() { 
  if (m_flags & cutflag::is_data ) { 
    if (m_flags & cutflag::spartid) { 
      throw std::logic_error("sparticle ID and data flags cannot coexist"); 
    }
    if (m_flags & cutflag::truth) { 
      throw std::logic_error("truth and data flags cannot coexist"); 
    }
  }

}

void StopDistiller::setup_streams() { 
  if (m_flags & cutflag::debug_cutflow) { 
    m_norm_dbg_file = new std::ofstream("distiller-dbg.txt"); 
  }
  if (m_norm_dbg_file) { 
    m_debug_buffer = m_norm_dbg_file->rdbuf(); 
  }
  else { 
    m_debug_buffer = m_null_file->rdbuf(); 
  }
}

void StopDistiller::setup_chain(const std::vector<std::string>& in) { 
  m_chain = new SmartChain("susy"); 
  m_ct_report = new CollectionTreeReport("CollectionTree"); 

  if (in.size() == 0) { 
    throw std::runtime_error("I need files to run!"); 
  }

  for (std::vector<std::string>::const_iterator file_itr = in.begin(); 
       file_itr != in.end(); 
       file_itr++) { 
    int ret_code = m_chain->add(file_itr->c_str(),-1); 
    if (ret_code != 1) { 
      std::string err = (boost::format("bad file: %s") % 
			 *file_itr).str(); 
      throw std::runtime_error(err); 
    }
  }
  m_ct_report->add_files(in); 

  BranchNames branch_names; 
  branch_names.trigger = m_info.trigger;

  m_susy_buffer = new SusyBuffer(m_chain, m_flags, branch_names); 

  if (m_flags & cutflag::get_branches) { 
    std::vector<std::string> br_names = m_chain->get_all_branch_names();
    ofstream branch_save("all-set-branches.txt"); 
    for (std::vector<std::string>::const_iterator itr = br_names.begin(); 
	 itr != br_names.end(); itr++) { 
      branch_save << *itr << std::endl;
    }
    branch_save.close(); 
  }

}

void StopDistiller::setup_susytools() { 
  m_def = new SUSYObjDef; 

  std::string out_file = "/dev/null"; 
  if (m_flags & cutflag::debug_susy) { 
    out_file = m_susy_dbg_file; 
  }
  else { 
    gErrorIgnoreLevel = kError;
  }
  int output_dup = dup(fileno(stdout)); 
  freopen(out_file.c_str(), "w", stdout); 

  m_def->initialize(m_flags & cutflag::is_data, 
		    m_flags & cutflag::is_atlfast); 
  if (m_flags & cutflag::no_jet_recal) { 
    m_def->SetJetCalib(false); 
  }
  m_event_preselector = new EventPreselector(m_flags, m_info.grl); 
  m_btag_calibration = new BtagCalibration(m_info.btag_cal_file, 
					   m_info.btag_cal_dir); 


  dup2(output_dup, fileno(stdout)); 
  close(output_dup); 
}

void StopDistiller::setup_outputs() { 
  m_out_tree = new outtree::OutTree(m_out_ntuple_name, 
				    "evt_tree", m_flags, N_JETS_TO_SAVE); 
  m_cutflow = new BitmapCutflow;
  m_cutflow->add("GRL"                   , pass::grl            );  
  m_cutflow->add(m_info.trigger          , pass::trigger        );
  m_cutflow->add("lar_error"             , pass::lar_error      );
  m_cutflow->add("core"                  , pass::core           );
  m_cutflow->add("jet_clean"             , pass::jet_clean      );
  m_cutflow->add("vxp_good"              , pass::vxp_gt_4trk    );
  m_cutflow->add("leading_jet_120"       , pass::leading_jet    );
  m_cutflow->add("met_120"               , pass::met            );
  m_cutflow->add("n_jet_geq_3"           , pass::n_jet          );
  m_cutflow->add("dphi_jetmet_min"       , pass::dphi_jetmet_min);
  m_cutflow->add("lepton_veto"           , pass::lepton_veto    );
  m_cutflow->add("ctag_mainz"            , pass::ctag_mainz     ); 
  ull_t preselection = pass::grl | pass::trigger | pass::core | 
    pass::lar_error; 
  m_required_for_save = preselection; 
  m_required_for_save |= pass::jet_clean; 
  m_required_for_save |= pass::vxp_gt_4trk; 
  m_required_for_save |= pass::leading_jet | pass::met; 
  m_required_for_save |= pass::n_jet; 
}


void StopDistiller::print_progress(int entry_n, std::ostream& stream) { 
  if (m_one_percent) { 
    if (( entry_n % m_one_percent == 0) || entry_n == m_n_entries - 1 ) { 
      stream << boost::format("\r%i of %i (%.1f%%) processed") % 
	(entry_n + 1) % m_n_entries % ( (entry_n + 1) / m_one_percent); 
      stream.flush(); 
    }
  }

}


