#ifndef OUT_TREE_H
#define OUT_TREE_H

#include <string>
#include <vector>
#include "typedefs.hh"

class TFile;
class TTree;
class TVector2;

namespace outtree {

  class EvtBools
  {
  public:
    EvtBools(unsigned);
    void set_branches(TTree*);
    void set_from_bits(ull_t);
  private:
    // met triggers
    bool EF_xe80_tclcw_loose;
    bool EF_xe80T_tclcw_loose;
    bool EF_xe80_tclcw_tight;
    // dilep triggers
    bool EF_mu18_tight_mu8_EFFS;
    bool EF_2e12Tvh_loose1;
    // single lepton triggers
    bool EF_mu24i_tight;
    bool EF_e24vhi_medium1;
    bool EF_mu36_tight;
    bool EF_e60_medium1;
    // compound
    bool met_trigger;
    bool two_el_trigger;
    bool two_mu_trigger;
    bool single_el_trigger;
    bool single_mu_trigger;

    // computed in the distiller
    bool event_quality;
    bool ossf;
    bool osdf;

    // save all triggers
    bool m_dump_triggers;
  };

  // Reconstructed event parameters, truth level and SF parameters
  // are defined in the main OutTree structure.
  struct EvtParameters
  {
    void set_branches(TTree*);
    void clear();
    int n_preselected_jets;
    int n_signal_jets;
    int n_baseline_electrons;
    int n_baseline_muons;
    int n_signal_electrons;
    int n_signal_muons;

    float min_jetmet_dphi;
    float lepmet_dphi;
    float met_eff;
    float mct;
    float mct_uncorr;
    float mcc;
    float mt;
    float mll;
    float htx;

    // float first_lepton_pt;
    // float second_lepton_pt;
    // int first_lepton_pdgid;
    // int second_lepton_pdgid;
  };

  struct SFBox
  {
    SFBox(unsigned flags);
    float nominal;
    float up;
    float down;
    void set_branches(TTree*, std::string prefix);
    void clear();
  private:
    bool m_variations;
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
    double jfc_b;
    double jfc_c;
    double jfc_u;
    unsigned jet_bits;

    ScaleFactor jfc_medium;
    ScaleFactor jfc_loose;

  private:
    friend class OutTree;
    void set_branches(TTree*, std::string prefix, unsigned flags);
    void clear();
  };

  class Lepton
  {
  public:
    Lepton();
    Lepton(Lepton&) = delete;
    Lepton& operator=(Lepton&) = delete;
    float pt;
    float eta;
    float phi;
    int pdgid;
  private:
    friend class OutTree;
    void set_branches(TTree*, std::string prefix);
    void clear();
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
    void add_double_parameter(const std::string& name, double val);
    void add_ll_parameter(const std::string& name, long long val);
    bool is_open() const;

    ull_t pass_bits;
    EvtParameters par;

    MetBlock met;

    int hfor_type;
    unsigned event_number;
    unsigned run_number;

    int truth_leading_cjet_pos;
    int truth_subleading_cjet_pos;
    int truth_n_cjet;
    float mc_event_weight;
    int spart1_pdgid;
    int spart2_pdgid;

    SFBox pileup_sf;
    SFBox el_sf;
    SFBox mu_sf;
    SFBox lepton_trig_sf;

    float boson_child_pt;
    float boson_child_phi;
    float truth_boson_pt_weight;
    float truth_boson_pt;

    std::vector<outtree::Jet*> jets;
    Lepton first_lepton;
    Lepton second_lepton;
  private:
    void init(const unsigned flags = 0, int n_jets = 3);
    EvtBools m_evt_bools;
    TFile* m_file;
    TTree* m_tree;
  };

};

#endif // OUT_TREE_H
