#include "StopDistiller.hh"
#include "distill_tools.hh"
#include "copy_functions.hh"
#include "object_selection.hh"

#include "SusyBuffer.h"
#include "OutTree.hh"
#include "OutputFilter.hh"
#include "Jets.hh"
#include "Leptons.hh"
#include "Met.hh"
#include "constants.hh"
#include "RunInfo.hh"
#include "BitmapCutflow.hh"
#include "SmartChain.hh"
#include "SkimReport.hh"
#include "EventPreselector.hh"
#include "BtagCalibration.hh"
#include "BosonTruthFilter.hh"
#include "TruthMetFilter.hh"
#include "BosonPtReweighter.hh"
#include "CheckSize.hh"

#include "cutflag.hh"
#include "EventBits.hh"
#include "constants_distiller.hh"
#include "constants_cutflow.hh"

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
#include "PileupReweighting/TPileupReweighting.h"

namespace { 
  void add_skim_report(const SkimReport& report, outtree::OutTree&); 
  void copy_event_truth(outtree::OutTree& out_tree, 
			const SusyBuffer& buffer, 
			unsigned branches); 

}

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
  m_skim_report(0), 
  m_susy_buffer(0), 
  m_def(0), 
  m_event_preselector(0), 
  m_out_tree(0), 
  m_cutflow(0), 
  m_btag_calibration(0), 
  m_boson_truth_filter(0), 
  m_truth_met_filter(0), 
  m_boson_pt_reweighter(0), 
  m_prw(0)
{ 
  gErrorIgnoreLevel = kError;
  check_flags(); 
  setup_streams(); 
  setup_chain(in); 
  setup_susytools(); 
  setup_outputs(); 
  setup_cutflow(info.cutflow_type); 
  if (info.boson_pt_max_mev > 0) { 
    m_boson_truth_filter = new BosonTruthFilter(info.boson_pt_max_mev); 
  }
  if (info.truth_met_max_mev > 0 ){
    if (flags & cutflag::old_skim) { 
      throw std::logic_error("truth met filter disabled for old skims"); 
    }
    m_truth_met_filter = new TruthMetFilter(info.truth_met_max_mev); 
  }
  if (flags & cutflag::boson_pt_reweight) { 
    m_boson_pt_reweighter = new BosonPtReweighter(); 
  }
}

StopDistiller::~StopDistiller() { 
  if (m_norm_dbg_file) { 
    m_norm_dbg_file->close(); 
    delete m_norm_dbg_file; 
  }
  delete m_null_file; 
  delete m_chain; 
  delete m_skim_report; 
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
  delete m_boson_truth_filter; 
  delete m_truth_met_filter; 
  delete m_boson_pt_reweighter; 
  if (m_flags & cutflag::generate_pileup && m_prw) { 
    m_prw->WriteToFile(m_info.pu_config); 
  }
  delete m_prw; 
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
      std::string err = "logic error in " + m_chain->get_current_file() + 
	", event " + std::to_string(evt_n) + ": " + e.what(); 
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

  add_skim_report(*m_skim_report, *m_out_tree); 
  return get_cutflow_vec(n_error); 
}

std::vector<std::pair<std::string, int> > StopDistiller::get_cutflow_vec(
  int n_error) const {

  typedef std::pair<std::string, int> Cut; 
  Cut total_events(std::make_pair("total_events", 
				  m_skim_report->total_entries()));
  std::vector<Cut> cutflow_vec = m_cutflow->get(); 
  cutflow_vec.insert(cutflow_vec.begin(),total_events); 
  if (n_error) { 
    cutflow_vec.push_back(std::make_pair("read_errors",n_error)); 
  }
  int n_empty_files = m_skim_report->empty_files(); 
  if (n_empty_files) { 
    cutflow_vec.push_back(
      std::make_pair("files_no_summary", n_empty_files)); 
  }
  auto obj_counts = m_object_counter->get_ordered_cuts(); 
  for (auto cut: obj_counts) {
    cutflow_vec.push_back(cut); 
  }
  if (m_boson_truth_filter) { 
    int n_lt_2_lepton = m_boson_truth_filter->get_n_lt_2_lepton_events(); 
    cutflow_vec.push_back(std::make_pair("lt_2_lepton", n_lt_2_lepton)); 
  }
  return cutflow_vec;

}

void StopDistiller::process_event(int evt_n, std::ostream& dbg_stream) { 
  assert(m_susy_buffer); 
  if (m_flags & cutflag::verbose) { 
    print_progress(evt_n, std::cout); 
  }
  m_chain->GetEntry(evt_n); 

  const float pileup_weight = get_pileup_weight(); 

  if (m_boson_truth_filter) { 
    if (m_boson_truth_filter->is_over_threshold(m_susy_buffer)) { 
      return; 
    }
  }
  if (m_truth_met_filter) { 
    if (m_truth_met_filter->is_over_threshold(m_susy_buffer)) { 
      return; 
    }
  }

  m_def->Reset(); 
  m_out_tree->clear_buffer(); 
  ull_t pass_bits = 0; 
    
  EventJets all_jets(*m_susy_buffer, *m_def, m_flags, m_info); 
  EventElectrons all_electrons(*m_susy_buffer, *m_def, m_flags, m_info); 
  EventMuons all_muons(*m_susy_buffer, *m_def, m_flags, m_info); 

  auto& ob_counts = *m_object_counter; 

  // --- preselection 

  pass_bits |= m_event_preselector->get_preselection_flags(
    *m_susy_buffer, *m_def); 

  // --- object selection 

  std::sort(all_jets.begin(),all_jets.end(),has_higher_pt); 

  auto preselected_jets = object::preselection_jets(all_jets); 
  auto preselected_electrons = filter_susy(all_electrons); 
  auto preselected_muons = filter_susy(all_muons); 

  ob_counts["preselected_jets"] += preselected_jets.size(); 
  ob_counts["preselected_el"] += preselected_electrons.size(); 
  ob_counts["preselected_mu"] += preselected_muons.size(); 

  // need to get susy muon indices before overlap
  std::vector<int> susy_muon_idx = get_indices(preselected_muons); 

  // --- overlap removal ---
  preselected_jets = remove_overlaping(
    preselected_electrons, preselected_jets, REMOVE_JET_CONE); 
  preselected_electrons = remove_overlaping(
    preselected_jets, preselected_electrons, REMOVE_EL_CONE); 
  preselected_muons = remove_overlaping(
    preselected_jets, preselected_muons, REMOVE_MU_CONE); 

  ob_counts["after_overlap_jets"] += preselected_jets.size(); 
  ob_counts["after_overlap_el"] += preselected_electrons.size(); 
  ob_counts["after_overlap_mu"] += preselected_muons.size(); 

  const auto veto_jets = object::veto_jets(preselected_jets); 
  const auto veto_electrons = object::veto_electrons(preselected_electrons); 
  const auto veto_muons = object::veto_muons(preselected_muons); 

  ob_counts["veto_jets"] += veto_jets.size(); 
  ob_counts["veto_el"] += veto_electrons.size(); 
  ob_counts["veto_mu"] += veto_muons.size(); 

  const auto good_jets = object::remove_bad_jets(preselected_jets); 
  auto signal_jets = object::signal_jets(good_jets); 
  const auto control_electrons = object::control_electrons(veto_electrons); 
  const auto control_muons = object::control_muons(veto_muons); 

  ob_counts["signal_jets"] += signal_jets.size(); 
  ob_counts["control_el"] += control_electrons.size(); 
  ob_counts["control_mu"] += control_muons.size(); 


  const int n_leading = std::min(signal_jets.size(), N_SR_JETS); 
  Jets leading_jets(signal_jets.begin(), signal_jets.begin() + n_leading); 

  const Mets mets(*m_susy_buffer, *m_def, susy_muon_idx, 
		  sum_muon_pt(control_muons));

  // ---- must calibrate signal jets for b-tagging ----
  calibrate_jets(signal_jets, m_btag_calibration); 
  // ----- object selection is done now, from here is filling outputs ---

  if (veto_jets.size() == 0) pass_bits |= pass::jet_clean; 
  m_out_tree->counts.n_preselected_jets = preselected_jets.size(); 
  m_out_tree->counts.n_signal_jets = signal_jets.size(); 
  m_out_tree->counts.n_veto_electrons = veto_electrons.size(); 
  m_out_tree->counts.n_veto_muons = veto_muons.size(); 
  m_out_tree->counts.n_control_electrons = control_electrons.size(); 
  m_out_tree->counts.n_control_muons = control_muons.size(); 

  pass_bits |= signal_jet_bits(signal_jets); 

  m_out_tree->htx = get_htx(signal_jets, N_SR_JETS); 
  m_out_tree->min_jetmet_dphi = get_min_jetmet_dphi(
    leading_jets, mets.nominal); 
  if (m_out_tree->min_jetmet_dphi > MIN_DPHI_JET_MET) { 
    pass_bits |= pass::dphi_jetmet_min; 
  }

  copy_leading_jet_info(signal_jets, *m_out_tree); 

  if(m_def->IsGoodVertex(m_susy_buffer->vx_nTracks)) {
    pass_bits |= pass::vxp_gt_4trk; 
  }

  pass_bits |= control_lepton_bits(control_electrons, control_muons); 
  // get zmass pair bits (true if _any_ leptons are in the z window)
  pass_bits |= z_control_bits(preselected_electrons, preselected_muons); 

  if (veto_electrons.size() == 0) pass_bits |= pass::electron_veto; 
  if (veto_muons.size() == 0) pass_bits |= pass::muon_veto; 

  pass_bits |= met_bits(mets); 

  if ( m_flags & cutflag::truth ) { 
    copy_cjet_truth(*m_out_tree, signal_jets); 
    copy_event_truth(*m_out_tree, *m_susy_buffer, m_flags); 
  }

  m_cutflow->fill(pass_bits); 
  m_out_tree->pass_bits = pass_bits; 

  copy_met(*m_out_tree, mets); 

  m_out_tree->pileup_weight = pileup_weight; 
  if (m_boson_pt_reweighter) {
    m_out_tree->truth_boson_pt_weight = m_boson_pt_reweighter->
      get_boson_weight(m_susy_buffer); 
    m_out_tree->truth_boson_pt = m_boson_pt_reweighter->
      get_boson_pt(m_susy_buffer); 
  }

  m_out_tree->event_number = m_susy_buffer->EventNumber; 

  copy_id_vec_to_box(control_electrons, m_out_tree->el_sf); 
  copy_id_vec_to_box(control_muons, m_out_tree->mu_sf); 

  // save electron jet
  if (control_electrons.size() == 1) { 
    auto control_el = control_electrons.at(0); 
    auto el_jet = object::get_leptojet(all_jets, *control_el); 
    el_jet->set_flavor_tag(m_btag_calibration); 
    copy_jet_info(el_jet, m_out_tree->electron_jet); 
  }

  // save boson decay products
  TVector2 boson_decay = get_boson_child_pt(
    control_electrons, control_muons); 
  if (boson_decay.Mod() > 0.001) { 
    m_out_tree->boson_child_pt = boson_decay.Mod(); 
    m_out_tree->boson_child_phi = boson_decay.Phi(); 
  }


  if (m_output_filter->should_save_event(pass_bits)) { 
    m_out_tree->fill(); 
  }

}


void StopDistiller::check_flags() { 
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
  m_skim_report = new SkimReport(); 

  if (in.size() == 0) { 
    throw std::runtime_error("I need files to run!"); 
  }

  for (auto file_itr: in){
    m_chain->add(file_itr.c_str(),-1); 
  }
  m_skim_report->add_files(in); 

  BranchSettings branch_settings; 
  branch_settings.trigger = m_info.trigger;

  // don't try to setup the buffer if the chain is empty. 
  if (m_chain->GetEntries() == 0) return; 

  m_susy_buffer = new SusyBuffer(m_chain, m_flags, branch_settings); 

  if (m_flags & cutflag::get_branches) { 
    std::vector<std::string> br_names = m_chain->get_all_branch_names();
    ofstream branch_save("all-set-branches.txt"); 
    for (auto itr: br_names){
      branch_save << itr << std::endl;
    }
    branch_save.close(); 
  }

}

void StopDistiller::setup_susytools() { 
  m_def = new SUSYObjDef; 

  std::string out_file = "/dev/null"; 
  if (m_flags & cutflag::debug_susy) { 
    out_file = m_susy_dbg_file; 
    gErrorIgnoreLevel = kWarning;
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

  if (m_flags & cutflag::truth && m_info.pu_config.size() > 0) { 
    m_prw = new Root::TPileupReweighting("PileupReweighting"); 
    m_prw->DisableWarnings( !(m_flags & cutflag::debug_susy)); 
    if (m_flags & cutflag::generate_pileup) { 
      m_prw->UsePeriodConfig("MC12a"); 
    } else { 
      m_prw->SetDefaultChannel(0); // this is what Brett does
      m_prw->AddConfigFile(m_info.pu_config); 
      m_prw->SetDataScaleFactors(1/1.11); // What Brett, Jan do
      m_prw->AddLumiCalcFile(m_info.pu_lumicalc); 
      m_prw->MergeMCRunNumbers(195847,195848); 
      m_prw->SetUnrepresentedDataAction(2); 
    }
    m_prw->Initialize(); 
  }
  dup2(output_dup, fileno(stdout)); 
  close(output_dup); 
}

void StopDistiller::setup_outputs() { 
  m_out_tree = new outtree::OutTree(m_out_ntuple_name, 
				    "evt_tree", m_flags, N_JETS_TO_SAVE); 
  m_object_counter = new CutCounter; 

}

void StopDistiller::setup_cutflow(CutflowType cutflow) { 
  const ull_t event_clean = pass::lar_error | pass::tile_error | 
    pass::core | pass::tile_trip; 
  const ull_t lepton_veto = pass::electron_veto | pass::muon_veto; 

  m_cutflow = new BitmapCutflow;
  switch (cutflow) { 
  case CutflowType::NOMINAL: { 
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
    m_cutflow->add("jtag_2"                , pass::cutflow_tag_2  ); 
    m_cutflow->add("jtag_1"                , pass::cutflow_tag_1  ); 
    return; 
  }
  case CutflowType::NONE: return; 
  case CutflowType::ELECTRON_CR: { 
    m_cutflow->add("GRL"                   , pass::grl            );  
    m_cutflow->add(m_info.trigger          , pass::trigger        );
    m_cutflow->add("primary_vertex"        , pass::vxp_gt_4trk    );
    m_cutflow->add("lar_error"        , pass::lar_error          );
    m_cutflow->add("tile_error"        , pass::tile_error          );
    m_cutflow->add("core"        , pass::core          );
    m_cutflow->add("tile_trip"        , pass::tile_trip          );
    m_cutflow->add("event_cleaning"        , event_clean          );
    m_cutflow->add("control_electron"      , pass::control_electron);
    m_cutflow->add("bad_jet_veto"          , pass::jet_clean      );
    m_cutflow->add("n_jet_geq_3"           , pass::n_jet          );
    m_cutflow->add("dphi_jetmet_min"       , pass::dphi_jetmet_min);
    m_cutflow->add("met_280"               , pass::cutflow_met    );
    m_cutflow->add("leading_jet_280"       , pass::cutflow_leading);
    m_cutflow->add("jtag_2"                , pass::cutflow_tag_2  ); 
    m_cutflow->add("jtag_1"                , pass::cutflow_tag_1  ); 
    return; 
  }
  case CutflowType::MUON_CR: { 
    m_cutflow->add("GRL"                   , pass::grl            );  
    m_cutflow->add(m_info.trigger          , pass::trigger        );
    m_cutflow->add("primary_vertex"        , pass::vxp_gt_4trk    );
    m_cutflow->add("lar_error"        , pass::lar_error          );
    m_cutflow->add("tile_error"        , pass::tile_error          );
    m_cutflow->add("core"        , pass::core          );
    m_cutflow->add("tile_trip"        , pass::tile_trip          );
    m_cutflow->add("event_cleaning"        , event_clean          );
    m_cutflow->add("control_muon"      , pass::control_muon    );
    m_cutflow->add("bad_jet_veto"          , pass::jet_clean      );
    m_cutflow->add("n_jet_geq_3"           , pass::n_jet          );
    m_cutflow->add("dphi_jetmet_min"       , pass::dphi_jetmet_min);
    m_cutflow->add("met_280"               , pass::cutflow_met    );
    m_cutflow->add("leading_jet_280"       , pass::cutflow_leading);
    m_cutflow->add("jtag_2"                , pass::cutflow_tag_2  ); 
    m_cutflow->add("jtag_1"                , pass::cutflow_tag_1  ); 
    return; 
  }
  }
}

float StopDistiller::get_pileup_weight() { 
  if (!m_prw) return 1.0; 
  const SusyBuffer* b = m_susy_buffer; 

  // apply hack from Will Butt's twiki: 
  // https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/ExtendedPileupReweighting#Recipe_A_MC12a_Pileup_Reweightin
  // sets some values from 1 to 0
  bool need_hack = b->lbn==1 && int(b->averageIntPerXing+0.5)==1; 
  float avx = need_hack ? 0.0 : b->averageIntPerXing;

  if (m_flags & cutflag::generate_pileup) { 
    m_prw->Fill(
      b->RunNumber, b->mc_channel_number, 
      b->mcevt_weight->at(0).at(0), avx); 
    return 1.0; 
  } else { 
    return m_prw->GetCombinedWeight(
      b->RunNumber,b->mc_channel_number,avx);
  }

}


void StopDistiller::print_progress(int entry_n, std::ostream& stream) { 
  if (m_one_percent) { 
    if (( entry_n % m_one_percent == 0) || entry_n == m_n_entries - 1 ) { 
      std::string msg = "\r" + std::to_string(entry_n + 1) + " of " + 
	std::to_string(m_n_entries) + 
	" (" + std::to_string((entry_n + 1) / m_one_percent) + "%) processed";
      stream.flush(); 
    }
  }

}

namespace { 
  void add_skim_report(const SkimReport& report, outtree::OutTree& tree) { 
    tree.add_ll_parameter("total_events", report.total_entries()); 
    if (!report.is_data()) {
      tree.add_double_parameter(
	"total_event_weight", report.sum_evt_weight()); 
    }
  }

  void copy_event_truth(outtree::OutTree& out_tree, 
			const SusyBuffer& buffer, 
			unsigned flags) { 
    out_tree.hfor_type = buffer.hfor_type; 
    if (flags & cutflag::spartid) { 
      out_tree.spart1_pdgid = buffer.spart1_pdgid; 
      out_tree.spart2_pdgid = buffer.spart2_pdgid; 
    } 
    // currently there is a bug in mc_event_weight, we have to use 
    // mcevt_weight[0][0] instead
    //out_tree.mc_event_weight = buffer.mc_event_weight; 
    out_tree.mc_event_weight = buffer.mcevt_weight->at(0).at(0); 
  }
}
