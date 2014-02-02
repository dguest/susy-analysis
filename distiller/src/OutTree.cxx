#include "OutTree.hh"
#include "cutflag.hh"

#include "TFile.h"
#include "TTree.h"
#include "TVector2.h"
#include "TParameter.h"

#include <string> 

//_______________________________________________________________
// OutTree

namespace outtree { 

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
    m_tree->Branch("pass_bits", &pass_bits ); 
    met_nom.set_branches(m_tree, ""); 
    met_mu.set_branches(m_tree, "mu_"); 
    m_tree->Branch("min_jetmet_dphi" , &min_jetmet_dphi); 
    m_tree->Branch("n_susy_jets", &n_susy_jets); 
    m_tree->Branch("n_signal_jets", &n_good_jets); 

    m_tree->Branch("event_number", &event_number); 

    m_tree->Branch("htx", &htx); 

    if ( flags & cutflag::truth) { 
      met_nom_up.set_branches(m_tree, "stup_"); 
      met_mu_up.set_branches(m_tree, "stup_mu_"); 
      met_nom_down.set_branches(m_tree, "stdown_"); 
      met_mu_down.set_branches(m_tree, "stdown_mu_"); 
      met_nom_res.set_branches(m_tree, "stres_"); 
      met_mu_res.set_branches(m_tree, "stres_mu_"); 
      m_tree->Branch("pileup_weight", &pileup_weight); 
      m_tree->Branch("hfor_type", &hfor_type); 
      m_tree->Branch("leading_cjet_pos", &leading_cjet_pos); 
      m_tree->Branch("subleading_cjet_pos", &subleading_cjet_pos); 
      m_tree->Branch("n_cjet", &n_cjet); 
      m_tree->Branch("mc_event_weight", &mc_event_weight); 
      if (flags & cutflag::spartid) { 
	m_tree->Branch("spart1_pdgid", &spart1_pdgid); 
	m_tree->Branch("spart2_pdgid", &spart2_pdgid); 
      }
      if (flags & cutflag::boson_pt_reweight) { 
	m_tree->Branch("boson_pt_weight", &boson_pt_weight); 
	m_tree->Branch("boson_pt", &boson_pt); 
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

  }

  void OutTree::clear_buffer() { 
    pass_bits = 0; 
    met_nom.clear(); 
    met_mu.clear(); 
    met_nom_up.clear(); 
    met_mu_up.clear(); 
    met_nom_down.clear(); 
    met_mu_down.clear(); 
    min_jetmet_dphi = -1; 
    sum_jetmet_dphi = -1; 
    n_susy_jets = -1; 
    n_good_jets = -1; 

    hfor_type = -2; 
    event_number = 0; 

    htx = 0; 
    pileup_weight = -1; 

    leading_cjet_pos = -1; 
    subleading_cjet_pos = -1; 
    n_cjet = 0; 
    mc_event_weight = 0; 
    boson_pt_weight = -1; 
    boson_pt = -1; 
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
    m_jet_vector.fill(jets); 
    if (m_tree) { 
      m_tree->Fill(); 
    }
  }

  void OutTree::add_double_parameter(const std::string& name, double val) { 
    TParameter<double> par(name.c_str(), val); 
    m_file->WriteTObject(&par); 
  }

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
      cnn_tight.set_branches(tree, prefix + "cnn_tight_", flags); 
      cnn_medium.set_branches(tree, prefix + "cnn_medium_", flags); 
      cnn_loose.set_branches(tree, prefix + "cnn_loose_", flags); 

      jfc_tight.set_branches(tree,  prefix + "jfc_tight_", flags); 
      jfc_medium.set_branches(tree, prefix + "jfc_medium_", flags); 
      jfc_loose.set_branches(tree,  prefix + "jfc_loose_", flags); 
    }

    tree->Branch((prefix + "cnn_b").c_str(), &cnn_b); 
    tree->Branch((prefix + "cnn_c").c_str(), &cnn_c); 
    tree->Branch((prefix + "cnn_u").c_str(), &cnn_u); 
    tree->Branch((prefix + "jfc_b").c_str(), &jfc_b); 
    tree->Branch((prefix + "jfc_c").c_str(), &jfc_c); 
    tree->Branch((prefix + "jfc_u").c_str(), &jfc_u); 

    tree->Branch((prefix + "bits").c_str(), &jet_bits); 

    if (flags & cutflag::save_ratios) { 
      tree->Branch((prefix + "cnn_log_cb").c_str(), &cnn_log_cb); 
      tree->Branch((prefix + "cnn_log_cu").c_str(), &cnn_log_cu); 
    }
  }

  void Jet::clear() 
  {
    pt = -1; 
    eta = -10; 
    phi = -10; 
    flavor_truth_label = -1; 
    cnn_b = -1; 
    cnn_c = -1; 
    cnn_u = -1; 
    jfc_b = -1; 
    jfc_c = -1; 
    jfc_u = -1; 
    cnn_log_cu = -1000; 
    cnn_log_cb = -1000; 
    jet_bits = 0; 

    cnn_medium.clear(); 
    cnn_loose.clear(); 
  }

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
      m_cnn_u.push_back(jet->cnn_u); 
      m_cnn_c.push_back(jet->cnn_c); 
      m_cnn_b.push_back(jet->cnn_b); 
      m_jet_bits.push_back(jet->jet_bits); 
      m_cnn_tight.fill(jet->cnn_tight); 
      m_cnn_medium.fill(jet->cnn_medium); 
      m_cnn_loose.fill(jet->cnn_loose); 
    }
  }

}; 
