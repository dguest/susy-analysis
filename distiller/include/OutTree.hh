#ifndef OUT_TREE_H
#define OUT_TREE_H

#include <string> 

class TFile; 
class TTree; 

class OutTree
{
public: 
  OutTree(std::string file, std::string tree = "evt_tree", 
	  const unsigned flags = 0); 
  ~OutTree(); 
  void fill(); 
  void clear_buffer(); 
  
  unsigned pass_bits; 
  double met; 
  double met_phi; 
  double min_jetmet_dphi; 
  double sum_jetmet_dphi; 
  int n_susy_jets; 
  int n_good_jets; 

  int hfor_type; 
  unsigned event_number; 

  int leading_cjet_pos; 
  int subleading_cjet_pos; 
  int n_cjet; 
  float mc_event_weight; 
  int spart1_pdgid; 
  int spart2_pdgid; 

  double htx; 
  
  class Jet 
  {
  public: 
    double pt; 
    double eta; 
    double phi; 
    int flavor_truth_label; 
    double cnn_b; 
    double cnn_c; 
    double cnn_u; 
    double cnn_log_cu; 
    double cnn_log_cb; 
  private: 
    friend class OutTree; 
    void set_branches(TTree*, std::string prefix, unsigned flags); 
    void clear(); 
  }; 

  Jet leading_jet_uncensored; 

  Jet leading_jet; 
  Jet subleading_jet; 
  Jet isr_jet; 

private:
  void init(const unsigned flags = 0); 
  TFile* m_file; 
  TTree* m_tree; 
}; 

#endif // OUT_TREE_H
