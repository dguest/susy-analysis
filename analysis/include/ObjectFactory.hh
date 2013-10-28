#ifndef JET_FACTORY_H
#define JET_FACTORY_H

#include <string> 
#include <vector> 
#include <map>
#include <unordered_map>
#include "HforTypeDef.hh"
#include "systematic_defs.hh"
#include "typedefs.hh"
#include "btag_enums.hh"
#include "EventScaleEnums.hh"

class Jet; 
class TTree;
class TFile; 
class TVector2;
class BtagScaler; 
class BtagBuffer; 
class TLorentzVector; 
class SFBox; 
class EventScalefactors; 

namespace ioflag {
  const unsigned no_flavor            = 1u << 0; 
  const unsigned no_truth             = 1u << 1; 
  const unsigned has_boson_pt_weight  = 1u << 2; 
}

struct JetBuffer
{ 
  double pt; 
  double eta; 
  double phi; 
  double E; 
  double cnn_b; 
  double cnn_c; 
  double cnn_u; 
  double jfc_b; 
  double jfc_c; 
  double jfc_u; 
  int flavor_truth_label; 
  unsigned bits; 
  std::map<std::string, BtagBuffer*> btag_buffers; 
  // TODO: the btag scalers  shoudldn't be owned by the buffer. 
  std::vector<BtagScaler*> btag_scalers; 
  bool is_electron_jet; 
  JetBuffer(); 
  ~JetBuffer(); 
};

struct MetBuffer 
{
  double met; 
  double met_phi; 
  MetBuffer(TTree* tree, const std::string& prefix); 
  MetBuffer(const MetBuffer&) = delete; 
  MetBuffer& operator=(const MetBuffer&) = delete; 
}; 

class ObjectFactory
{
public: 
  ObjectFactory(std::string root_file, int n_jets); 
  ~ObjectFactory(); 
  void use_electron_jet(bool = true); 
  void set_btagging(const std::vector<btag::OperatingPoint>&); 
  int entries() const; 
  void entry(int); 
  std::vector<Jet> jets() const; 
  TVector2 met(syst::Systematic) const; 
  TVector2 mu_met(syst::Systematic) const; 
  ull_t bits() const; 
  double dphi() const; 
  int n_good() const; 
  int n_susy() const; 
  int leading_cjet_pos() const; 
  int subleading_cjet_pos() const; 
  hfor::JetType hfor_type() const; 
  double htx() const; 
  double event_weight() const; 
  EventScalefactors* event_scalefactors() const; 
private: 
  void set_btag_n(size_t jet_n, btag::OperatingPoint); 
  void set_btag(JetBuffer*, btag::OperatingPoint, std::string branch_name); 
  void set_buffer(JetBuffer* buffer, std::string base_name); 
  TTree* m_tree; 
  TFile* m_file; 
  std::vector<JetBuffer*> m_jet_buffers; 
  JetBuffer* m_electron_jet_buffer; 

  std::unordered_map<int, MetBuffer*> m_met; 
  std::unordered_map<int, MetBuffer*> m_mu_met; 

  ull_t m_bits; 
  double m_dphi; 
  int m_n_good; 
  int m_n_susy; 
  int m_hfor_type; 
  
  int m_leading_cjet_pos; 
  int m_subleading_cjet_pos; 

  double m_htx; 
  float m_mc_event_weight; 
  float m_pileup_weight; 
  float m_boson_pt_weight; 

  unsigned m_ioflags; 
  EventScalefactors* m_evt_sf; 
  
}; 

bool has_higher_pt(const Jet&, const Jet&); 

#endif // JET_FACTORY_H
