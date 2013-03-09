#ifndef OUT_TREE_H
#define OUT_TREE_H

#include <string> 
#include <vector> 
#include "typedefs.hh"
#include <boost/noncopyable.hpp>

class TFile; 
class TTree; 

namespace outtree { 

  class ScaleFactor : public boost::noncopyable
  {
  public:
    double scale_factor; 
    double scale_factor_err; 
  private: 
    friend class Jet; 
    void set_branches(TTree*, std::string prefix, unsigned flags); 
    void clear(); 
  }; 

  class Jet : public boost::noncopyable
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
    unsigned jet_bits; 

    ScaleFactor cnn_tight; 
    ScaleFactor cnn_medium; 
    ScaleFactor cnn_loose; 

  private: 
    friend class OutTree; 
    void set_branches(TTree*, std::string prefix, unsigned flags); 
    void clear(); 
  }; 


  class OutTree : public boost::noncopyable
  {
  public: 
    OutTree(std::string file, std::string tree = "evt_tree", 
	    const unsigned flags = 0, int n_jets = 3); 
    ~OutTree(); 
    void fill(); 
    void clear_buffer(); 
  
    ull_t pass_bits; 
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

    outtree::Jet leading_jet_uncensored; 

    std::vector<outtree::Jet*> jets; 

  private:
    void init(const unsigned flags = 0, int n_jets = 3); 
    TFile* m_file; 
    TTree* m_tree; 
  }; 

}; 

#endif // OUT_TREE_H
