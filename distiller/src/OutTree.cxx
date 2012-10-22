#include "OutTree.hh"
#include "CutBits.hh"

#include "TFile.h"
#include "TTree.h"

#include <string> 

//_______________________________________________________________
// OutTree

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

void OutTree::Jet::set_branches(TTree* tree, std::string prefix, 
				unsigned flags) 
{
  tree->Branch((prefix + "pt").c_str(), &pt); 
  tree->Branch((prefix + "eta").c_str(), &eta); 
  tree->Branch((prefix + "phi").c_str(), &phi); 
  tree->Branch((prefix + "flavor_truth_label").c_str(), 
		 &flavor_truth_label); 
  if (flags & cutflag::save_flavor_wt) { 
    tree->Branch((prefix + "cnn_b").c_str(), &cnn_b); 
    tree->Branch((prefix + "cnn_c").c_str(), &cnn_c); 
    tree->Branch((prefix + "cnn_u").c_str(), &cnn_u); 
  }
  if (flags & cutflag::save_ratios) { 
    tree->Branch((prefix + "cnn_log_cb").c_str(), &cnn_log_cb); 
    tree->Branch((prefix + "cnn_log_cu").c_str(), &cnn_log_cu); 
  }
}

void OutTree::Jet::clear() 
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
}

void OutTree::init(const unsigned flags) 
{ 
  m_tree->Branch("pass_bits", &pass_bits ); 
  m_tree->Branch("met", &met); 
  m_tree->Branch("min_jetmet_dphi" , &min_jetmet_dphi); 
  m_tree->Branch("n_susy_jets", &n_susy_jets); 
  m_tree->Branch("n_good_jets", &n_good_jets); 

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
  min_jetmet_dphi = -1; 
  sum_jetmet_dphi = -1; 
  n_susy_jets = -1; 
  n_good_jets = -1; 
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
