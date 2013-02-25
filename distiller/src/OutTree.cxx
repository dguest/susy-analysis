#include "OutTree.hh"
#include "RunBits.hh"

#include "TFile.h"
#include "TTree.h"

#include <string> 

//_______________________________________________________________
// OutTree

namespace outtree { 

  OutTree::OutTree(std::string file, std::string tree, const unsigned flags): 
    m_file(0), 
    m_tree(0)
  { 
    if (file.size() > 0) { 
      m_file = new TFile(file.c_str(), "recreate"); 
      m_tree = new TTree(tree.c_str(), tree.c_str()); 
      init(flags); 
    }
  }


  void OutTree::init(const unsigned flags) 
  { 
    m_tree->Branch("pass_bits", &pass_bits ); 
    m_tree->Branch("met", &met); 
    m_tree->Branch("met_phi", &met_phi); 
    m_tree->Branch("min_jetmet_dphi" , &min_jetmet_dphi); 
    m_tree->Branch("n_susy_jets", &n_susy_jets); 
    m_tree->Branch("n_good_jets", &n_good_jets); 

    m_tree->Branch("event_number", &event_number); 

    m_tree->Branch("htx", &htx); 

    if ( flags & cutflag::truth) { 
      m_tree->Branch("hfor_type", &hfor_type); 
      m_tree->Branch("leading_cjet_pos", &leading_cjet_pos); 
      m_tree->Branch("subleading_cjet_pos", &subleading_cjet_pos); 
      m_tree->Branch("n_cjet", &n_cjet); 
      m_tree->Branch("mc_event_weight", &mc_event_weight); 
      if (flags & cutflag::spartid) { 
	m_tree->Branch("spart1_pdgid", &spart1_pdgid); 
	m_tree->Branch("spart2_pdgid", &spart2_pdgid); 
      }
    }

    leading_jet.set_branches(m_tree, "jet2_", flags); 
    subleading_jet.set_branches(m_tree, "jet3_", flags); 
    isr_jet.set_branches(m_tree, "jet1_", flags); 
    if (flags & cutflag::raw_evt_info) {
      leading_jet_uncensored.set_branches(m_tree, "leading_jet_uncensored_", 
					  flags); 
    }

  }

  void OutTree::clear_buffer() { 
    pass_bits = 0; 
    met = -1; 
    met_phi = -10; 
    min_jetmet_dphi = -1; 
    sum_jetmet_dphi = -1; 
    n_susy_jets = -1; 
    n_good_jets = -1; 

    hfor_type = -2; 
    event_number = 0; 

    htx = 0; 

    leading_cjet_pos = -1; 
    subleading_cjet_pos = -1; 
    n_cjet = 0; 
    mc_event_weight = 0; 
    spart1_pdgid = 0; 
    spart2_pdgid = 0; 

    leading_jet.clear(); 
    subleading_jet.clear(); 
    isr_jet.clear(); 
    leading_jet_uncensored.clear(); 
  }


  OutTree::~OutTree() 
  {
    if (m_file){ 
      m_file->WriteTObject(m_tree); 
      delete m_file; 
    }
    m_file = 0; 
  }

  void OutTree::fill() 
  {
    if (m_tree) { 
      m_tree->Fill(); 
    }
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
    }

    tree->Branch((prefix + "cnn_b").c_str(), &cnn_b); 
    tree->Branch((prefix + "cnn_c").c_str(), &cnn_c); 
    tree->Branch((prefix + "cnn_u").c_str(), &cnn_u); 

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
    cnn_log_cu = -1000; 
    cnn_log_cb = -1000; 

    cnn_medium.clear(); 
    cnn_loose.clear(); 
  }

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

}; 
