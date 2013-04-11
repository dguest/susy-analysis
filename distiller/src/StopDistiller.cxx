#include "StopDistiller.hh"
#include "distill_tools.hh"
#include "object_selection.hh"

#include "SusyBuffer.h"
#include "OutTree.hh"
#include "OutputFilter.hh"
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
#include <algorithm> // min
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
  m_output_filter(new OutputFilter(info, flags)), 
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
  delete m_object_counter; 
  delete m_btag_calibration; 
}

StopDistiller::Cutflow StopDistiller::run_cutflow() { 
  m_n_entries = m_chain->GetEntries(); 
  m_one_percent = m_n_entries / 100; 

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
  auto obj_counts = m_object_counter->get_ordered_cuts(); 
  for (auto cut = obj_counts.begin(); cut != obj_counts.end(); cut++) { 
    cutflow_vec.push_back(*cut); 
  }
  return cutflow_vec;

}

void StopDistiller::process_event(int evt_n, std::ostream& dbg_stream) { 
  assert(m_susy_buffer); 
  if (m_flags & cutflag::verbose) { 
    print_progress(evt_n, std::cout); 
  }
  m_chain->GetEntry(evt_n); 

  m_def->Reset(); 
  m_out_tree->clear_buffer(); 
  ull_t pass_bits = 0; 
    
  EventJets all_jets(*m_susy_buffer, *m_def, m_flags, m_info); 
  EventElectrons all_electrons(*m_susy_buffer, *m_def, m_flags, m_info); 
  EventMuons all_muons(*m_susy_buffer, *m_def, m_flags, m_info); 

  auto preselected_electrons = filter_susy(all_electrons); 
  auto preselected_muons = filter_susy(all_muons); 
  
  auto& ob_counts = *m_object_counter; 

  ob_counts["preselected_el"] += preselected_electrons.size(); 
  ob_counts["preselected_mu"] += preselected_muons.size(); 

  // --- preselection 

  pass_bits |= m_event_preselector->get_preselection_flags(*m_susy_buffer, 
							   *m_def); 

  // --- object selection 

  std::sort(all_jets.begin(),all_jets.end(),has_lower_pt); 
  std::reverse(all_jets.begin(), all_jets.end()); 

  if (all_jets.size()) { 
    all_jets.at(0)->set_flavor_tag(m_btag_calibration); 
    copy_jet_info(all_jets.at(0), m_out_tree->leading_jet_uncensored); 
  }
    
  PreselectionJets preselection_jets(all_jets); 
  ob_counts["preselected_jets"] += preselection_jets.size(); 

  SignalJets signal_jets(preselection_jets); 
  set_bit(signal_jets, jetbit::signal); 

  // need to get susy muon indices before overlap
  std::vector<int> susy_muon_idx = get_indices(preselected_muons); 

  signal_jets = remove_overlaping(preselected_electrons, 
					signal_jets, 
					REMOVE_JET_CONE); 
  preselected_electrons = remove_overlaping(signal_jets, 
					    preselected_electrons, 
					    REMOVE_EL_CONE); 
  preselected_muons = remove_overlaping(signal_jets, 
					preselected_muons, 
					REMOVE_MU_CONE); 

  ob_counts["after_overlap_jets"] += signal_jets.size(); 
  ob_counts["after_overlap_el"] += preselected_electrons.size(); 
  ob_counts["after_overlap_mu"] += preselected_muons.size(); 

  const auto veto_electrons = object::veto_electrons(preselected_electrons); 
  const auto veto_muons = object::veto_muons(preselected_muons); 

  ob_counts["veto_el"] += veto_electrons.size(); 

  const auto control_electrons = object::control_electrons(veto_electrons); 
  const auto control_muons = object::control_muons(veto_muons); 

  ob_counts["veto_mu"] += veto_muons.size(); 


  const int n_leading = std::min(signal_jets.size(), N_SR_JETS); 
  Jets leading_jets(signal_jets.begin(), signal_jets.begin() + n_leading); 
  set_bit(leading_jets, jetbit::leading); 

  const auto met = get_met(*m_susy_buffer, *m_def, m_info, susy_muon_idx); 
  const auto mu_met = get_mumet(met, control_muons); 

  // ---- must calibrate signal jets for b-tagging ----
  calibrate_jets(signal_jets, m_btag_calibration); 
  // ----- object selection is done now, from here is filling outputs ---

  pass_bits |= jet_cleaning_bit(all_jets); 
  pass_bits |= signal_jet_bits(signal_jets); 
  m_out_tree->n_susy_jets = preselection_jets.size(); 


  if (signal_jets.size() == 2) pass_bits |= pass::dopplejet; 
  if (signal_jets.size() >= N_SR_JETS) pass_bits |= pass::n_jet; 

  m_out_tree->htx = get_htx(signal_jets); 
  m_out_tree->min_jetmet_dphi = get_min_jetmet_dphi(leading_jets, met); 
  if (m_out_tree->min_jetmet_dphi > MIN_DPHI_JET_MET) { 
    pass_bits |= pass::dphi_jetmet_min; 
  }

  copy_leading_jet_info(signal_jets, *m_out_tree); 

  if(m_def->IsGoodVertex(m_susy_buffer->vx_nTracks)) {
    pass_bits |= pass::vxp_gt_4trk; 
  }

  pass_bits |= control_lepton_bits(control_electrons, control_muons); 

  if (veto_electrons.size() == 0) pass_bits |= pass::electron_veto; 
  if (veto_muons.size() == 0) pass_bits |= pass::muon_veto; 

  pass_bits |= met_bits(met); 

  if ( m_flags & cutflag::truth ) { 
    fill_cjet_truth(*m_out_tree, signal_jets); 
    fill_event_truth(*m_out_tree, *m_susy_buffer, m_flags); 
  }

  m_cutflow->fill(pass_bits); 
  m_out_tree->pass_bits = pass_bits; 
  m_out_tree->met = met.Mod(); 
  m_out_tree->met_phi = met.Phi(); 
  m_out_tree->mu_met = mu_met.Mod(); 
  m_out_tree->mu_met_phi = mu_met.Phi(); 

  m_out_tree->event_number = m_susy_buffer->EventNumber; 

  if (m_output_filter->should_save_event(pass_bits)) { 
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

  for (auto file_itr = in.begin(); file_itr != in.end(); file_itr++) { 
     m_chain->add(file_itr->c_str(),-1); 
  }
  m_ct_report->add_files(in); 

  BranchNames branch_names; 
  branch_names.trigger = m_info.trigger;

  // don't try to setup the buffer if the chain is empty. 
  if (m_chain->GetEntries() == 0) return; 

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
  const ull_t event_clean = pass::lar_error | pass::tile_error | 
    pass::core | pass::tile_trip; 
  const ull_t lepton_veto = pass::electron_veto | pass::muon_veto; 
  m_cutflow = new BitmapCutflow;
  m_cutflow->add("GRL"                   , pass::grl            );  
  m_cutflow->add(m_info.trigger          , pass::trigger        );
  m_cutflow->add("primary_vertex"        , pass::vxp_gt_4trk    );
  m_cutflow->add("lar_error"        , pass::lar_error          );
  m_cutflow->add("tile_error"        , pass::tile_error          );
  m_cutflow->add("core"        , pass::core          );
  m_cutflow->add("tile_trip"        , pass::tile_trip          );
  m_cutflow->add("event_cleaning"        , event_clean          );
  m_cutflow->add("electron_veto"           , pass::electron_veto    );
  m_cutflow->add("muon_veto"           , pass::muon_veto    );
  m_cutflow->add("lepton_veto"           ,       lepton_veto    );
  m_cutflow->add("bad_jet_veto"          , pass::jet_clean      );
  m_cutflow->add("n_jet_geq_3"           , pass::n_jet          );
  m_cutflow->add("dphi_jetmet_min"       , pass::dphi_jetmet_min);
  m_cutflow->add("met_280"               , pass::cutflow_met    );
  m_cutflow->add("leading_jet_280"       , pass::cutflow_leading);
  m_cutflow->add("jtag_1"                , pass::cutflow_tag_1  ); 
  m_cutflow->add("jtag_2"                , pass::cutflow_tag_2  ); 

  m_object_counter = new CutCounter; 

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


