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

  void SFBox::set_branches(TTree* tree, std::string prefix) { 
    tree->Branch((prefix + "sf").c_str(), &nominal); 
    tree->Branch((prefix + "sf_up").c_str(), &up); 
    tree->Branch((prefix + "sf_down").c_str(), &down); 
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
    m_file(0), 
    m_tree(0)
  { 
    if (file.size() > 0) { 
      m_file = new TFile(file.c_str(), "recreate"); 
      if (flags & cutflag::maximum_compression) { 
	m_file->SetCompressionLevel(9); 
      }
      m_tree = new TTree(tree.c_str(), tree.c_str()); 
      init(flags, n_jets); 
    }
  }


  void OutTree::init(const unsigned flags, int n_jets) 
  { 
    MAKE_BRANCH(m_tree, pass_bits); 
    counts.set_branches(m_tree); 

    met_nom.set_branches(m_tree, ""); 
    met_mu.set_branches(m_tree, "mu_"); 

    MAKE_BRANCH(m_tree, min_jetmet_dphi); 
    MAKE_BRANCH(m_tree, event_number); 
    MAKE_BRANCH(m_tree, htx); 

    if ( flags & cutflag::truth) { 
      met_nom_up.set_branches(m_tree, "stup_"); 
      met_mu_up.set_branches(m_tree, "stup_mu_"); 
      met_nom_down.set_branches(m_tree, "stdown_"); 
      met_mu_down.set_branches(m_tree, "stdown_mu_"); 
      met_nom_res.set_branches(m_tree, "stres_"); 
      met_mu_res.set_branches(m_tree, "stres_mu_"); 
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
    }

    for (int i = 0; i < n_jets; i++) { 
      jets.push_back(new Jet); 
      std::string jetname = "jet" + std::to_string(i) + "_";
      if (! (flags & cutflag::vector_output)) { 
	(*jets.rbegin())->set_branches(m_tree, jetname, flags); 
      }
    }
    if (flags & cutflag::vector_output) { 
      m_jet_vector.set_branches(m_tree, "jet_", flags); 
    }

    electron_jet.set_branches(m_tree, "electron_jet_", flags); 

    if (flags & cutflag::raw_evt_info) {
      leading_jet_uncensored.set_branches(m_tree, "leading_jet_uncensored_", 
					  flags); 
    }
    m_tree->Branch("boson_child_pt", &boson_child_pt); 
    m_tree->Branch("boson_child_phi", &boson_child_phi); 
    m_evt_bools.set_branches(m_tree); 

  }

  void OutTree::clear_buffer() { 
    pass_bits = 0; 
    counts.clear(); 

    met_nom.clear(); 
    met_mu.clear(); 
    met_nom_up.clear(); 
    met_mu_up.clear(); 
    met_nom_down.clear(); 
    met_mu_down.clear(); 
    min_jetmet_dphi = -1; 
    sum_jetmet_dphi = -1; 

    hfor_type = -2; 
    event_number = 0; 

    htx = 0; 
    pileup_weight = -1; 

    truth_leading_cjet_pos = -1; 
    truth_subleading_cjet_pos = -1; 
    truth_n_cjet = 0; 
    mc_event_weight = 0; 
    truth_boson_pt_weight = -1; 
    truth_boson_pt = -1; 
    spart1_pdgid = 0; 
    spart2_pdgid = 0; 

    for (auto itr = jets.begin(); itr != jets.end(); itr++) { 
      (*itr)->clear(); 
    }
    leading_jet_uncensored.clear(); 
    electron_jet.clear(); 
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
    m_jet_vector.fill(jets); 
    if (m_tree) { 
      m_tree->Fill(); 
    }
  }

  void OutTree::add_double_parameter(const std::string& name, double val) { 
    TParameter<double> par(name.c_str(), val); 
    m_file->WriteTObject(&par); 
  }
  void OutTree::add_ll_parameter(const std::string& name, long long val) { 
    TParameter<long long> par(name.c_str(), val); 
    m_file->WriteTObject(&par); 
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

    // tree->Branch((prefix + "bits").c_str(), &jet_bits); 
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

  // ----------------------- ScaleFactor -----------------

  ScaleFactor::ScaleFactor() {}

  void ScaleFactor::set_branches(TTree* tree, std::string prefix, 
				 unsigned flags)
  {
    tree->Branch((prefix + "scale_factor").c_str(), &scale_factor);
    tree->Branch((prefix + "scale_factor_err").c_str(), &scale_factor_err);
  }
  void ScaleFactor::clear() { 
    scale_factor = -1; 
    scale_factor_err = -1; 
  }

  // ------------- SFVector --------------------------

  SFVector::SFVector() {}

  void SFVector::set_branches(TTree* tree, std::string prefix, 
			      unsigned flags) { 
    tree->Branch((prefix + "scale_factor").c_str(), &m_scale_factor);
    tree->Branch((prefix + "scale_factor_err").c_str(), &m_scale_factor_err);
  }
  void SFVector::fill(const ScaleFactor& sf) { 
    m_scale_factor.push_back(sf.scale_factor); 
    m_scale_factor_err.push_back(sf.scale_factor_err); 
  }
  void SFVector::clear() { 
    m_scale_factor.clear(); 
    m_scale_factor_err.clear(); 
  }

  // ------------ jet vector ----------------
  // remove? 

  JetVector::JetVector() { }

  void JetVector::set_branches(TTree* tree, std::string prefix, 
			       unsigned flags) { 
    tree->Branch((prefix + "pt").c_str(), &m_pt); 
    tree->Branch((prefix + "eta").c_str(), &m_eta); 
    tree->Branch((prefix + "phi").c_str(), &m_phi); 
    if ( flags & cutflag::truth) { 
      tree->Branch((prefix + "flavor_truth_label").c_str(), 
		   &m_flavor_truth_label); 
      m_cnn_tight.set_branches(tree, prefix + "cnn_tight_", flags); 
      m_cnn_medium.set_branches(tree, prefix + "cnn_medium_", flags); 
      m_cnn_loose.set_branches(tree, prefix + "cnn_loose_", flags); 
    }

    tree->Branch((prefix + "cnn_b").c_str(), &m_cnn_b); 
    tree->Branch((prefix + "cnn_c").c_str(), &m_cnn_c); 
    tree->Branch((prefix + "cnn_u").c_str(), &m_cnn_u); 
    tree->Branch((prefix + "bits").c_str(), &m_jet_bits); 
  }
  void JetVector::fill(const std::vector<outtree::Jet*>& jets) { 
    m_pt.clear(); 
    m_eta.clear(); 
    m_phi.clear(); 
    m_flavor_truth_label.clear(); 
    m_cnn_b.clear(); 
    m_cnn_c.clear(); 
    m_cnn_u.clear(); 
    m_jet_bits.clear(); 
    m_cnn_tight.clear(); 
    m_cnn_medium.clear(); 
    m_cnn_loose.clear(); 
    for (auto itr = jets.cbegin(); itr != jets.cend(); itr++) { 
      const auto jet = *itr; 
      if (jet->pt < 0.0) break; // pt is -1 for non-existent jets
      m_pt.push_back(jet->pt); 
      m_eta.push_back(jet->eta); 
      m_phi.push_back(jet->phi); 
      m_flavor_truth_label.push_back(jet->flavor_truth_label); 
      // m_cnn_u.push_back(jet->cnn_u); 
      // m_cnn_c.push_back(jet->cnn_c); 
      // m_cnn_b.push_back(jet->cnn_b); 
      m_jet_bits.push_back(jet->jet_bits); 
      // m_cnn_tight.fill(jet->cnn_tight); 
      // m_cnn_medium.fill(jet->cnn_medium); 
      // m_cnn_loose.fill(jet->cnn_loose); 
    }
  }

  // ----------------- EvtBools -----------------------

#define MAKE_PASS_BRANCH(branch) \
  tree->Branch("pass_" #branch, &branch)

  void EvtBools::set_branches(TTree* tree) { 
    MAKE_PASS_BRANCH(grl); 
    MAKE_PASS_BRANCH(met_trigger); 
    MAKE_PASS_BRANCH(mu_trigger); 
    MAKE_PASS_BRANCH(el_trigger); 
    // MAKE_PASS_BRANCH(electron_veto); 
    // MAKE_PASS_BRANCH(muon_veto); 
    // MAKE_PASS_BRANCH(control_muon); 
    // MAKE_PASS_BRANCH(control_electron); 
    MAKE_PASS_BRANCH(os_zmass_el_pair); 
    MAKE_PASS_BRANCH(os_zmass_mu_pair); 
  }
#undef MAKE_PASS_BRANCH

#define SET_FROM_BIT(BIT)	  \
  BIT = pass::BIT & bits

  void EvtBools::set_from_bits(ull_t bits) { 
    SET_FROM_BIT(grl); 
    SET_FROM_BIT(met_trigger); 
    SET_FROM_BIT(mu_trigger); 
    SET_FROM_BIT(el_trigger); 
    // SET_FROM_BIT(electron_veto); 
    // SET_FROM_BIT(muon_veto); 
    // SET_FROM_BIT(control_muon); 
    // SET_FROM_BIT(control_electron); 
    SET_FROM_BIT(os_zmass_el_pair); 
    SET_FROM_BIT(os_zmass_mu_pair); 
  }
#undef SET_FROM_BIT

  // ---- object counts ----

  void ObjectCounts::set_branches(TTree* tree) { 
    MAKE_BRANCH(tree, n_preselected_jets); 
    MAKE_BRANCH(tree, n_signal_jets); 
    MAKE_BRANCH(tree, n_veto_electrons); 
    MAKE_BRANCH(tree, n_veto_muons); 
    MAKE_BRANCH(tree, n_control_electrons); 
    MAKE_BRANCH(tree, n_control_muons); 
  }

  void ObjectCounts::clear() { 
    n_preselected_jets = -1; 
    n_signal_jets = -1; 
    n_veto_electrons = -1; 
    n_veto_muons = -1; 
    n_control_electrons = -1; 
    n_control_muons = -1; 
  }


#undef MAKE_BRANCH

}; 
