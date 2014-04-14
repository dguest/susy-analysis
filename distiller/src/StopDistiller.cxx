#include "StopDistiller.hh"
#include "distill_tools.hh"
#include "copy_functions.hh"
#include "bitset_functions.hh"

#include "SusyBuffer.h"
#include "OutTree.hh"
#include "OutputFilter.hh"
#include "Jets.hh"
#include "Leptons.hh"
#include "Met.hh"
#include "EventObjects.hh"
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
#include "CutCounter.hh"

#include "cutflag.hh"
#include "EventBits.hh"
#include "ObjectComposites.hh"
#include "constants_distiller.hh"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
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

namespace {
  void add_skim_report(const SkimReport& report, outtree::OutTree&);
  void copy_event_truth(outtree::OutTree& out_tree,
			const SusyBuffer& buffer,
			unsigned branches);

}

StopDistiller::StopDistiller(const std::vector<std::string>& in,
			     const RunInfo& info, unsigned flags):
  m_info(info),
  m_flags(flags),
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
  m_mumet_out_tree(0),
  m_leptmet_out_tree(0),
  m_cutflow(0),
  m_btag_calibration(0),
  m_boson_truth_filter(0),
  m_truth_met_filter(0),
  m_boson_pt_reweighter(0)
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
  delete m_mumet_out_tree;
  delete m_leptmet_out_tree;
  delete m_object_counter;
  delete m_btag_calibration;
  delete m_boson_truth_filter;
  delete m_truth_met_filter;
  delete m_boson_pt_reweighter;
}

StopDistiller::Cutflow StopDistiller::run_cutflow() {
  m_n_entries = m_chain->GetEntries();
  if (m_flags & cutflag::cutflow) m_n_entries = std::min(
    m_n_entries, 100000);
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
    } catch (EventReadError& e) {
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
  add_skim_report(*m_skim_report, *m_mumet_out_tree);
  add_skim_report(*m_skim_report, *m_leptmet_out_tree);
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

  if (m_boson_truth_filter) {
    if (m_boson_truth_filter->is_over_threshold(
	  m_susy_buffer->mc_particles)) {
      return;
    }
  }
  if (m_truth_met_filter) {
    if (m_truth_met_filter->is_over_threshold(m_susy_buffer)) {
      return;
    }
  }

  // the sequence of the next few lines is important:
  // SUSYObjDef requires that leptons are filled before met
  m_def->Reset();

  EventObjects obj(*m_susy_buffer, *m_def, m_flags, m_info,
		   *m_event_preselector);
  obj.do_overlap_removal(*m_object_counter);
  // ---- must calibrate signal jets for b-tagging ----
  calibrate_jets(obj.signal_jets, m_btag_calibration);

  const Mets mets(*m_susy_buffer, *m_def, obj.susy_muon_idx,
		  sum_muon_pt(obj.control_muons),
		  sum_el_pt(obj.control_electrons),
		  m_info.systematic);

  BitmapCutflow* nom_cutflow = 0;
  BitmapCutflow* lept_cutflow = 0;
  if (m_info.cutflow_type == CutflowType::CRZ) {
    lept_cutflow = m_cutflow;
  } else {
    nom_cutflow = m_cutflow;
  }
  fill_event_output(obj, mets.nominal, mets.nominal, *m_out_tree,
		    nom_cutflow);
  fill_event_output(obj, mets.nominal, mets.muon, *m_mumet_out_tree);
  fill_event_output(obj, mets.nominal, mets.lepton, *m_leptmet_out_tree,
		    lept_cutflow);
}

void StopDistiller::fill_event_output(const EventObjects& obj,
				      const TVector2& met,
				      const TVector2& alt_met,
				      outtree::OutTree& out_tree,
				      BitmapCutflow* cutflow) const {

  // ACHTUNG: this is saved as the pileup weight, should fix that...
  const float combined_mc_wt = obj.prec.pileup_weight * obj.prec.trigger_sf;

  const ObjectComposites par(obj, met, alt_met);
  // ----- object selection is done now, from here is filling outputs ---

  // --- fill bits ---
  ull_t pass_bits = obj.prec.bits;
  pass_bits |= bits::object_composit_bits(par);
  pass_bits |= bits::event_object_bits(obj);
  pass_bits |= bits::met_bits(alt_met);
  // ACHTUNG: the "bad tile" veto is using standard met for qcd rejection
  // (Will seems to agree that we should do this)
  pass_bits |= bits::bad_tile_bits(met, obj.preselected_jets);
  pass_bits |= bits::compound_bits(pass_bits);

  // save bools to cutflow and out tree
  if (cutflow) cutflow->fill(pass_bits);

  // start filling out_tree here
  out_tree.clear_buffer();

  out_tree.pass_bits = pass_bits;
  out_tree.event_number = m_susy_buffer->EventNumber;
  out_tree.pileup_weight = combined_mc_wt;

  // main event copy function
  copy_event(obj, par, alt_met, out_tree);

  if ( m_flags & cutflag::truth ) {
    copy_cjet_truth(out_tree, obj.signal_jets);
    copy_event_truth(out_tree, *m_susy_buffer, m_flags);
  }

  if (m_boson_pt_reweighter) {
    out_tree.truth_boson_pt_weight = m_boson_pt_reweighter->
      get_boson_weight(m_susy_buffer->mc_particles);
    out_tree.truth_boson_pt = m_boson_pt_reweighter->
      get_boson_pt(m_susy_buffer->mc_particles);
  }

  // save boson decay products
  TVector2 boson_decay = get_boson_child_pt(
    obj.control_electrons, obj.control_muons);
  if (boson_decay.Mod() > 0.001) {
    out_tree.boson_child_pt = boson_decay.Mod();
    out_tree.boson_child_phi = boson_decay.Phi();
  }

  if (m_output_filter->should_save_event(pass_bits)) {
    out_tree.fill();
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

  // don't try to setup the buffer if the chain is empty.
  if (m_chain->GetEntries() == 0) return;

  m_susy_buffer = new SusyBuffer(m_chain, m_flags);

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

  m_def->initialize(
    m_flags & cutflag::is_data,
    m_flags & cutflag::is_atlfast,
    false, // mc12b
    true  // lepton trigger
    );

  m_event_preselector = new EventPreselector(m_flags, m_info);
  m_btag_calibration = new BtagCalibration(m_info.btag_cal_file,
					   m_info.btag_cal_dir);

  dup2(output_dup, fileno(stdout));
  close(output_dup);
}

void StopDistiller::setup_outputs() {
  m_out_tree = new outtree::OutTree(
    m_info.out_ntuple, "evt_tree", m_flags, N_JETS_TO_SAVE);
  m_mumet_out_tree = new outtree::OutTree(
    m_info.mumet_out_ntuple, "evt_tree", m_flags, N_JETS_TO_SAVE);
  m_leptmet_out_tree = new outtree::OutTree(
    m_info.leptmet_out_ntuple, "evt_tree", m_flags, N_JETS_TO_SAVE);

  m_object_counter = new CutCounter;
}

namespace {
  template<typename T>
  std::string cat(const std::string& str, T other) {
    return str + std::to_string(other);
  }
  template<>
  std::string cat<float>(const std::string& str, float val) {
    if (val > 1e3) {
      return str + std::to_string(static_cast<int>(val / GeV));
    } else {
      std::stringstream strm;
      strm << str;
      strm << std::setprecision(2) << val;
      return strm.str();
    }
  }
}

void StopDistiller::setup_cutflow(CutflowType cutflow) {

  const ull_t lept_trigger = (
    pass::EF_mu24i_tight | pass::EF_e24vhi_medium1 |
    pass::EF_mu36_tight | pass::EF_e60_medium1);
  const ull_t match_trigger = pass::single_el_trigger |
    pass::single_mu_trigger;
  m_cutflow = new BitmapCutflow;
  switch (cutflow) {
  case CutflowType::NONE: return;
  case CutflowType::SIGNAL: {
    m_cutflow->add("GRL"                   , pass::grl            );
    m_cutflow->add("trigger"          , pass::met_trigger        );
    m_cutflow->add("primary_vertex"        , pass::vxp_gt_4trk    );
    m_cutflow->add("bad_jet_veto"          , pass::jet_clean      );
    m_cutflow->add("tile_trip"        , pass::tile_trip          );
    m_cutflow->add("tile_error"        , pass::tile_error          );
    m_cutflow->add("bad_tile"          , pass::bad_tile      );
    m_cutflow->add("lar_error"        , pass::lar_error          );
    m_cutflow->add("energy_wt_time", pass::energy_wt_time);
    m_cutflow->add("core"        , pass::core          );
    m_cutflow->add("cosmic_muon" , pass::cosmic_muon);
    m_cutflow->add("bad_muon"    , pass::bad_muon);
    m_cutflow->add("zero_lepton"           , pass::zero_lepton    );
    m_cutflow->add("lepton_veto"           , pass::lepton_veto    );
    m_cutflow->add("chf_cut"     , pass::jet_chf);
    m_cutflow->add("met_150" , pass::met150    );
    m_cutflow->add(cat("n_jet_",N_SR_JETS) , pass::n_jet          );
    m_cutflow->add(cat("j1_", CUTFLOW_JET1_PT), pass::cutflow_leading);
    m_cutflow->add("j2_50", pass::j2_50);
    m_cutflow->add(cat("j3_veto_", CUTFLOW_JET3_PT_VETO), pass::cutflow_jet3);
    m_cutflow->add("dphi_jetmet_min"       , pass::dphi_jetmet_min);
    m_cutflow->add("met_eff"               , pass::met_eff);
    m_cutflow->add("one_ctag"              , pass::tagged  );
    m_cutflow->add("two_ctag"              , pass::double_tagged  );
    m_cutflow->add(cat("m_ct_",SR_MCT_MIN) , pass::sr_mct            );
    m_cutflow->add("m_cc"                  , pass::m_cc           );
    return;
  }
  case CutflowType::CRW: {
    m_cutflow->add("GRL"                   , pass::grl            );
    m_cutflow->add("lepton_trigger" , lept_trigger);
    m_cutflow->add("matched_trigger" , match_trigger);
    m_cutflow->add("primary_vertex"        , pass::vxp_gt_4trk    );
    m_cutflow->add("bad_jet_veto"          , pass::jet_clean      );
    m_cutflow->add("tile_trip"        , pass::tile_trip          );
    m_cutflow->add("tile_error"        , pass::tile_error          );
    m_cutflow->add("bad_tile"          , pass::bad_tile      );
    m_cutflow->add("lar_error"        , pass::lar_error          );
    m_cutflow->add("energy_wt_time", pass::energy_wt_time);
    m_cutflow->add("core"        , pass::core          );
    m_cutflow->add("cosmic_muon" , pass::cosmic_muon);
    m_cutflow->add("bad_muon"    , pass::bad_muon);
    m_cutflow->add("pass_1l", pass::one_lepton);
    m_cutflow->add("lepton_veto"        , pass::lepton_veto);
    m_cutflow->add("chf_cut"     , pass::jet_chf);
    m_cutflow->add("met_100" , pass::met100    );
    m_cutflow->add(cat("n_jet_",N_SR_JETS) , pass::n_jet          );
    m_cutflow->add(cat("j1_", CUTFLOW_JET1_PT), pass::cutflow_leading);
    m_cutflow->add("j2_50", pass::j2_50);
    m_cutflow->add("one_ctag"              , pass::tagged  );
    m_cutflow->add("two_ctag"              , pass::double_tagged  );
    m_cutflow->add(cat("m_ct_", SR_MCT_MIN), pass::sr_mct);
    m_cutflow->add("mt"                    , pass::mass_t);
    return;
  }
  case CutflowType::CRZ: {
    m_cutflow->add("GRL"                   , pass::grl            );
    m_cutflow->add("trigger", pass::two_el_trigger | pass::two_mu_trigger);
    m_cutflow->add("primary_vertex"        , pass::vxp_gt_4trk    );
    m_cutflow->add("bad_jet_veto"          , pass::jet_clean      );
    m_cutflow->add("tile_trip"        , pass::tile_trip          );
    m_cutflow->add("tile_error"        , pass::tile_error          );
    m_cutflow->add("bad_tile"          , pass::bad_tile      );
    m_cutflow->add("lar_error"        , pass::lar_error          );
    m_cutflow->add("energy_wt_time", pass::energy_wt_time);
    m_cutflow->add("core"        , pass::core          );
    m_cutflow->add("cosmic_muon" , pass::cosmic_muon);
    m_cutflow->add("bad_muon"    , pass::bad_muon);
    m_cutflow->add("ossf"           , pass::ossf    );
    m_cutflow->add("lepton_veto"        , pass::lepton_veto);
    m_cutflow->add("chf_cut"     , pass::jet_chf);
    m_cutflow->add("met_100" , pass::met100    );
    m_cutflow->add(cat("n_jet_",N_SR_JETS) , pass::n_jet          );
    m_cutflow->add("j1_50", pass::j1_50);
    m_cutflow->add("j2_50", pass::j2_50);
    m_cutflow->add("one_ctag"              , pass::tagged  );
    m_cutflow->add("two_ctag"              , pass::double_tagged  );
    m_cutflow->add("mll_zpeak"             , pass::mass_ll_z);
    m_cutflow->add("lepton_70"             , pass::lepton_70);
    return;
  }
  case CutflowType::CRT: {
    m_cutflow->add("GRL"                   , pass::grl            );
    m_cutflow->add("lepton_trigger" , lept_trigger);
    m_cutflow->add("matched_trigger" , match_trigger);
    m_cutflow->add("primary_vertex"        , pass::vxp_gt_4trk    );
    m_cutflow->add("bad_jet_veto"          , pass::jet_clean      );
    m_cutflow->add("tile_trip"        , pass::tile_trip          );
    m_cutflow->add("tile_error"        , pass::tile_error          );
    m_cutflow->add("bad_tile"          , pass::bad_tile      );
    m_cutflow->add("lar_error"        , pass::lar_error          );
    m_cutflow->add("energy_wt_time", pass::energy_wt_time);
    m_cutflow->add("core"        , pass::core          );
    m_cutflow->add("cosmic_muon" , pass::cosmic_muon);
    m_cutflow->add("bad_muon"    , pass::bad_muon);
    m_cutflow->add("two_lepton"        , pass::two_lepton);
    m_cutflow->add("lepton_veto"        , pass::lepton_veto);
    m_cutflow->add("osdf", pass::osdf);
    m_cutflow->add("chf_cut"     , pass::jet_chf);
    m_cutflow->add("met_50" , pass::met50 );
    m_cutflow->add(cat("n_jet_",N_SR_JETS) , pass::n_jet          );
    m_cutflow->add("j1_50", pass::j1_50);
    m_cutflow->add("j2_50", pass::j2_50);
    m_cutflow->add("one_ctag"              , pass::tagged  );
    m_cutflow->add("two_ctag"              , pass::double_tagged  );
    m_cutflow->add("mll_t"                 , pass::mass_ll_t);
    return;
  }
  }
}

namespace {
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
    out_tree.mc_event_weight = buffer.mc_event_weight;
  }
}
