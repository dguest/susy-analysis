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
    par.set_branches(m_tree); 

    met_nom.set_branches(m_tree, ""); 
    met_mu.set_branches(m_tree, "mu_"); 

    MAKE_BRANCH(m_tree, min_jetmet_dphi); 
    MAKE_BRANCH(m_tree, event_number); 
    MAKE_BRANCH(m_tree, htx); 

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
    }

    for (int i = 0; i < n_jets; i++) { 
      jets.push_back(new Jet); 
      std::string jetname = "jet" + std::to_string(i) + "_";
      jets.at(i)->set_branches(m_tree, jetname, flags); 
    }

    electron_jet.set_branches(m_tree, "electron_jet_", flags); 

    MAKE_BRANCH(m_tree, boson_child_pt); 
    MAKE_BRANCH(m_tree, boson_child_phi); 
    m_evt_bools.set_branches(m_tree); 

  }

  void OutTree::clear_buffer() { 
    pass_bits = 0; 
    par.clear(); 

    met_nom.clear(); 
    met_mu.clear(); 

    hfor_type = -2; 
    event_number = 0; 

    htx = 0; 
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
    if (! (flags & cutflag::disable_sf_err)) { 
      tree->Branch((prefix + "scale_factor_err").c_str(), &scale_factor_err);
    }
  }
  void ScaleFactor::clear() { 
    scale_factor = -1; 
    scale_factor_err = -1; 
  }

  // ----------------- EvtBools -----------------------

#define MAKE_PASS_BRANCH(branch) \
  tree->Branch("pass_" #branch, &branch)

  void EvtBools::set_branches(TTree* tree) { 
    MAKE_PASS_BRANCH(grl); 
    MAKE_PASS_BRANCH(met_trigger); 
    MAKE_PASS_BRANCH(mu_trigger); 
    MAKE_PASS_BRANCH(el_trigger); 
    MAKE_PASS_BRANCH(bad_tile_stmet); 
    MAKE_PASS_BRANCH(bad_tile_mumet); 
  }

#define SET_FROM_BIT(BIT)	  \
  BIT = pass::BIT & bits

  void EvtBools::set_from_bits(ull_t bits) { 
    SET_FROM_BIT(grl); 
    SET_FROM_BIT(met_trigger); 
    SET_FROM_BIT(mu_trigger); 
    SET_FROM_BIT(el_trigger); 
    SET_FROM_BIT(bad_tile_stmet); 
    SET_FROM_BIT(bad_tile_mumet); 
  }
#undef SET_FROM_BIT

  // ---- object counts ----

  void EvtParameters::set_branches(TTree* tree) { 
    MAKE_BRANCH(tree, n_preselected_jets); 
    MAKE_BRANCH(tree, n_signal_jets); 
    MAKE_BRANCH(tree, n_veto_electrons); 
    MAKE_BRANCH(tree, n_veto_muons); 
    MAKE_BRANCH(tree, n_control_electrons); 
    MAKE_BRANCH(tree, n_control_muons); 
  }

  void EvtParameters::clear() { 
    n_preselected_jets = -1; 
    n_signal_jets = -1; 
    n_veto_electrons = -1; 
    n_veto_muons = -1; 
    n_control_electrons = -1; 
    n_control_muons = -1; 
  }


#undef MAKE_BRANCH

}; 
