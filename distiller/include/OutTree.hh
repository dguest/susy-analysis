#ifndef OUT_TREE_H
#define OUT_TREE_H

#include <string> 
#include <vector> 
#include "typedefs.hh"

class TFile; 
class TTree; 
class TVector2; 

namespace outtree { 

  struct SFBox 
  {
    float nominal; 
    float up; 
    float down; 
    void set_branches(TTree*, std::string prefix); 
    void clear(); 
  }; 

  class MetBlock
  {
  public: 
    MetBlock() = default; 
    MetBlock(const MetBlock&) = delete; 
    MetBlock& operator=(const MetBlock&) = delete; 
    void set_branches(TTree* out_tree, const std::string& prefix); 
    void set_met(const TVector2& met); 
    void clear(); 
  private: 
    double m_met; 
    double m_met_phi;
  }; 

  class ScaleFactor 
  {
  public:
    ScaleFactor(); 
    ScaleFactor(ScaleFactor&) = delete; 
    ScaleFactor& operator=(ScaleFactor&) = delete; 
    double scale_factor; 
    double scale_factor_err; 
  private: 
    friend class Jet; 
    void set_branches(TTree*, std::string prefix, unsigned flags); 
    void clear(); 
  }; 

  class SFVector
  {
  public: 
    SFVector(); 
    SFVector(SFVector&) = delete; 
    SFVector& operator=(SFVector&) = delete; 
    void set_branches(TTree*, std::string prefix, unsigned flags); 
    void fill(const ScaleFactor& ); 
    void clear(); 
  private: 
    std::vector<double> m_scale_factor; 
    std::vector<double> m_scale_factor_err; 
  }; 

  class Jet 
  {
  public: 
    Jet(); 
    Jet(Jet&) = delete; 
    Jet& operator=(Jet&) = delete; 
    double pt; 
    double eta; 
    double phi; 
    int flavor_truth_label; 
    double cnn_b; 
    double cnn_c; 
    double cnn_u; 
    double jfc_b; 
    double jfc_c; 
    double jfc_u; 
    double cnn_log_cu; 
    double cnn_log_cb; 
    unsigned jet_bits; 

    ScaleFactor cnn_tight; 
    ScaleFactor cnn_medium; 
    ScaleFactor cnn_loose; 
    ScaleFactor jfc_tight; 
    ScaleFactor jfc_medium; 
    ScaleFactor jfc_loose; 

  private: 
    friend class OutTree; 
    void set_branches(TTree*, std::string prefix, unsigned flags); 
    void clear(); 
  }; 

  class JetVector
  {
  public: 
    JetVector(); 
    JetVector(JetVector&) = delete; 
    JetVector& operator=(JetVector&) = delete; 
    void set_branches(TTree*, std::string prefix, unsigned flags); 
    void fill(const std::vector<outtree::Jet*>&); 
  private: 
    std::vector<double> m_pt; 
    std::vector<double> m_eta; 
    std::vector<double> m_phi; 
    std::vector<int> m_flavor_truth_label; 
    std::vector<double> m_cnn_b; 
    std::vector<double> m_cnn_c; 
    std::vector<double> m_cnn_u; 
    std::vector<unsigned> m_jet_bits; 
    SFVector m_cnn_tight; 
    SFVector m_cnn_medium; 
    SFVector m_cnn_loose; 
  }; 


  class OutTree 
  {
  public: 
    OutTree(std::string file, std::string tree = "evt_tree", 
	    const unsigned flags = 0, int n_jets = 3); 
    ~OutTree(); 
    OutTree(OutTree&) = delete; 
    OutTree operator=(OutTree&) = delete; 
    void fill(); 
    void clear_buffer(); 
  
    ull_t pass_bits; 

    MetBlock met_nom; 
    MetBlock met_mu; 
    MetBlock met_nom_up; 
    MetBlock met_mu_up; 
    MetBlock met_nom_down; 
    MetBlock met_mu_down; 
    MetBlock met_nom_res; 
    MetBlock met_mu_res; 

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
    float pileup_weight; 
    float boson_pt_weight; 
    float boson_pt; 
    SFBox el_sf; 
    SFBox mu_sf; 

    outtree::Jet leading_jet_uncensored; 
    outtree::Jet electron_jet; 

    float boson_child_pt; 
    float boson_child_phi; 

    std::vector<outtree::Jet*> jets; 
  private:
    void init(const unsigned flags = 0, int n_jets = 3); 
    TFile* m_file; 
    TTree* m_tree; 
    outtree::JetVector m_jet_vector; 
  }; 

}; 

#endif // OUT_TREE_H
