#include "OutTree.hh"
#include "cutflag.hh"
#include "EventBits.hh"

#include "TFile.h"
#include "TTree.h"
#include "TVector2.h"
#include "TParameter.h"

#include <string>
#include <stdexcept>

//_______________________________________________________________
// OutTree

#define MAKE_BRANCH(tree, branch) \
  tree->Branch(#branch, &branch)

namespace outtree {

  // -------------- SFBox -----------------

  SFBox::SFBox(unsigned flags):
    m_variations(true)
  {
    if (flags & cutflag::disable_sf_err) {
      m_variations = false;
    }
  }

  void SFBox::set_branches(TTree* tree, std::string prefix) {
    tree->Branch((prefix + "sf").c_str(), &nominal);
    if (m_variations) {
      tree->Branch((prefix + "sf_up").c_str(), &up);
      tree->Branch((prefix + "sf_down").c_str(), &down);
    }
  }
  void SFBox::clear() {
    nominal = -1;
    up = -1;
    down = -1;
  }

  // ---------------------- MetBlock ---------------

  void MetBlock::set_branches(TTree* tree, const std::string& prefix) {
    tree->Branch((prefix + "met").c_str(), &m_met);
    tree->Branch((prefix + "met_phi").c_str(), &m_met_phi);
  }
  void MetBlock::set_met(const TVector2& met) {
    m_met = met.Mod();
    m_met_phi = met.Phi();
  }
  void MetBlock::clear() {
    m_met = -1;
    m_met_phi = -10;
  }

  // ----------------- OutTree ---------------------

  OutTree::OutTree(std::string file, std::string tree, const unsigned flags,
		   int n_jets):
    el_sf(flags),
    mu_sf(flags),
    lepton_trig_sf(flags),
    m_evt_bools(flags),
    m_file(0),
    m_tree(0)
  {
    if (file.size() > 0) {
      m_file = new TFile(file.c_str(), "recreate");
      if (!m_file->IsOpen()) {
	throw std::runtime_error("can't create output file: " + file);
      }
      if (flags & cutflag::maximum_compression) {
	m_file->SetCompressionLevel(9);
      }
      m_tree = new TTree(tree.c_str(), tree.c_str());
      init(flags, n_jets);
    }
  }


  void OutTree::init(const unsigned flags, int n_jets)
  {
    par.set_branches(m_tree);
    met.set_branches(m_tree, "");

    MAKE_BRANCH(m_tree, event_number);

    if ( flags & cutflag::truth) {
      MAKE_BRANCH(m_tree, pileup_weight);
      MAKE_BRANCH(m_tree, hfor_type);
      MAKE_BRANCH(m_tree, truth_leading_cjet_pos);
      MAKE_BRANCH(m_tree, truth_subleading_cjet_pos);
      MAKE_BRANCH(m_tree, truth_n_cjet);
      MAKE_BRANCH(m_tree, mc_event_weight);
      if (flags & cutflag::spartid) {
	MAKE_BRANCH(m_tree, spart1_pdgid);
	MAKE_BRANCH(m_tree, spart2_pdgid);
      }
      if (flags & cutflag::boson_pt_reweight) {
	MAKE_BRANCH(m_tree, truth_boson_pt_weight);
	MAKE_BRANCH(m_tree, truth_boson_pt);
      }
      el_sf.set_branches(m_tree, "el_");
      mu_sf.set_branches(m_tree, "mu_");
      lepton_trig_sf.set_branches(m_tree, "lepton_trig_");
    }

    for (int i = 0; i < n_jets; i++) {
      jets.push_back(new Jet);
      std::string jetname = "jet" + std::to_string(i) + "_";
      jets.at(i)->set_branches(m_tree, jetname, flags);
    }
    first_lepton.set_branches(m_tree, "first_lepton_");
    second_lepton.set_branches(m_tree, "second_lepton_");

    MAKE_BRANCH(m_tree, boson_child_pt);
    MAKE_BRANCH(m_tree, boson_child_phi);
    m_evt_bools.set_branches(m_tree);

  }

  void OutTree::clear_buffer() {
    pass_bits = 0;
    par.clear();

    met.clear();

    hfor_type = -2;
    event_number = 0;

    pileup_weight = -1;

    truth_leading_cjet_pos = -1;
    truth_subleading_cjet_pos = -1;
    truth_n_cjet = 0;
    mc_event_weight = -1;
    truth_boson_pt_weight = -1;
    truth_boson_pt = -1;
    spart1_pdgid = 0;
    spart2_pdgid = 0;

    for (auto itr = jets.begin(); itr != jets.end(); itr++) {
      (*itr)->clear();
    }
    first_lepton.clear();
    second_lepton.clear();
    el_sf.clear();
    mu_sf.clear();
    boson_child_pt = -1;
    boson_child_phi = -10;
  }


  OutTree::~OutTree()
  {
    if (m_file){
      m_file->WriteTObject(m_tree);
      delete m_file;
    }
    for (auto itr: jets) {
      delete itr;
      itr = 0;
    }
    m_file = 0;
  }

  void OutTree::fill()
  {
    m_evt_bools.set_from_bits(pass_bits);
    if (m_tree) {
      m_tree->Fill();
    }
  }

  void OutTree::add_double_parameter(const std::string& name, double val) {
    TParameter<double> par(name.c_str(), val);
    if (m_file) m_file->WriteTObject(&par);
  }
  void OutTree::add_ll_parameter(const std::string& name, long long val) {
    TParameter<long long> par(name.c_str(), val);
    if (m_file) m_file->WriteTObject(&par);
  }

  bool OutTree::is_open() const {
    return m_file;
  }

  // ----------------- Jet -------------------

  Jet::Jet() {

  }

  void Jet::set_branches(TTree* tree, std::string prefix,
			 unsigned flags)
  {
    tree->Branch((prefix + "pt").c_str(), &pt);
    tree->Branch((prefix + "eta").c_str(), &eta);
    tree->Branch((prefix + "phi").c_str(), &phi);
    if ( flags & cutflag::truth) {
      tree->Branch((prefix + "flavor_truth_label").c_str(),
		   &flavor_truth_label);
      jfc_medium.set_branches(tree, prefix + "jfc_medium_", flags);
      jfc_loose.set_branches(tree,  prefix + "jfc_loose_", flags);
    }

    tree->Branch((prefix + "jfc_b").c_str(), &jfc_b);
    tree->Branch((prefix + "jfc_c").c_str(), &jfc_c);
    tree->Branch((prefix + "jfc_u").c_str(), &jfc_u);
  }

  void Jet::clear()
  {
    pt = -1;
    eta = -10;
    phi = -10;
    flavor_truth_label = -1;
    jfc_b = -1;
    jfc_c = -1;
    jfc_u = -1;
    jet_bits = 0;

    jfc_medium.clear();
    jfc_loose.clear();
  }

  // ----------------------- Lepton ----------------------
  Lepton::Lepton() {
  }
  void Lepton::set_branches(TTree* tree, std::string prefix) {
    tree->Branch((prefix + "pt").c_str(), &pt);
    tree->Branch((prefix + "eta").c_str(), &eta);
    tree->Branch((prefix + "phi").c_str(), &phi);
    tree->Branch((prefix + "pdgid").c_str(), &pdgid);
  }
  void Lepton::clear(){
    pt = -1;
    eta = -10;
    phi = -10;
    pdgid = 0;
  }

  // ----------------------- ScaleFactor -----------------

  ScaleFactor::ScaleFactor() {}

  void ScaleFactor::set_branches(TTree* tree, std::string prefix,
				 unsigned flags)
  {
    tree->Branch((prefix + "scale_factor").c_str(), &scale_factor);
    if (! (flags & cutflag::disable_sf_err)) {
      tree->Branch((prefix + "scale_factor_err").c_str(), &scale_factor_err);
    }
  }
  void ScaleFactor::clear() {
    scale_factor = -1;
    scale_factor_err = -1;
  }

  // ----------------- EvtBools -----------------------

  EvtBools::EvtBools(unsigned flags) {
    m_dump_triggers = flags & cutflag::all_trig_branches;
  }

#define MAKE_PASS_BRANCH(branch) \
  tree->Branch("pass_" #branch, &branch)

  void EvtBools::set_branches(TTree* tree) {
    if (m_dump_triggers) {
      // met triggers
      MAKE_PASS_BRANCH(EF_xe80_tclcw_loose);
      MAKE_PASS_BRANCH(EF_xe80T_tclcw_loose);
      MAKE_PASS_BRANCH(EF_xe80_tclcw_tight);
      // dilep triggers
      MAKE_PASS_BRANCH(EF_mu18_tight_mu8_EFFS);
      MAKE_PASS_BRANCH(EF_2e12Tvh_loose1);
      // single lepton triggers
      MAKE_PASS_BRANCH(EF_mu24i_tight);
      MAKE_PASS_BRANCH(EF_e24vhi_medium1);
      MAKE_PASS_BRANCH(EF_mu36_tight);
      MAKE_PASS_BRANCH(EF_e60_medium1);
    } else {
      // compound trigger
      MAKE_PASS_BRANCH(met_trigger);
      MAKE_PASS_BRANCH(single_mu_trigger);
      MAKE_PASS_BRANCH(single_el_trigger);
      MAKE_PASS_BRANCH(two_mu_trigger);
      MAKE_PASS_BRANCH(two_el_trigger);
    }
    // computed internally
    MAKE_PASS_BRANCH(event_quality);
    MAKE_PASS_BRANCH(ossf);
    MAKE_PASS_BRANCH(osdf);
  }

#define SET_FROM_BIT(BIT)	  \
  BIT = pass::BIT & bits

  void EvtBools::set_from_bits(ull_t bits) {
    // met triggers
    SET_FROM_BIT(EF_xe80_tclcw_loose);
    SET_FROM_BIT(EF_xe80T_tclcw_loose);
    SET_FROM_BIT(EF_xe80_tclcw_tight);
    // dilep triggers
    SET_FROM_BIT(EF_mu18_tight_mu8_EFFS);
    SET_FROM_BIT(EF_2e12Tvh_loose1);
    // single lepton triggers
    SET_FROM_BIT(EF_mu24i_tight);
    SET_FROM_BIT(EF_e24vhi_medium1);
    SET_FROM_BIT(EF_mu36_tight);
    SET_FROM_BIT(EF_e60_medium1);

    SET_FROM_BIT(met_trigger);
    SET_FROM_BIT(two_el_trigger);
    SET_FROM_BIT(two_mu_trigger);
    SET_FROM_BIT(single_el_trigger);
    SET_FROM_BIT(single_mu_trigger);

    // computed internally
    SET_FROM_BIT(event_quality);
    SET_FROM_BIT(ossf);
    SET_FROM_BIT(osdf);
  }
#undef SET_FROM_BIT

  // ---- object counts ----

  void EvtParameters::set_branches(TTree* tree) {
    MAKE_BRANCH(tree, n_preselected_jets);
    MAKE_BRANCH(tree, n_signal_jets);
    MAKE_BRANCH(tree, n_baseline_electrons);
    MAKE_BRANCH(tree, n_baseline_muons);
    MAKE_BRANCH(tree, n_signal_electrons);
    MAKE_BRANCH(tree, n_signal_muons);

    MAKE_BRANCH(tree, min_jetmet_dphi);
    MAKE_BRANCH(tree, lepmet_dphi);
    MAKE_BRANCH(tree, met_eff);
    MAKE_BRANCH(tree, mct);
    MAKE_BRANCH(tree, mct_uncorr);
    MAKE_BRANCH(tree, mcc);
    MAKE_BRANCH(tree, mt);
    MAKE_BRANCH(tree, mll);
    MAKE_BRANCH(tree, htx);

    // MAKE_BRANCH(tree, first_lepton_pt);
    // MAKE_BRANCH(tree, second_lepton_pt);
    // MAKE_BRANCH(tree, first_lepton_pdgid);
    // MAKE_BRANCH(tree, second_lepton_pdgid);
  }

  void EvtParameters::clear() {
    n_preselected_jets = -1;
    n_signal_jets = -1;
    n_baseline_electrons = -1;
    n_baseline_muons = -1;
    n_signal_electrons = -1;
    n_signal_muons = -1;

    min_jetmet_dphi = -1;
    lepmet_dphi = -1;
    met_eff = -1;
    mct = -1;
    mct_uncorr = -1;
    mcc = -1;
    mt = -1;
    mll = -1;
    htx = -1;

    // first_lepton_pt = -1;
    // second_lepton_pt = -1;
    // first_lepton_pdgid = 0;
    // second_lepton_pdgid = 0;
  }


#undef MAKE_BRANCH

};
