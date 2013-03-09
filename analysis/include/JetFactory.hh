#ifndef JET_FACTORY_H
#define JET_FACTORY_H

#include <string> 
#include <vector> 
#include "TLorentzVector.h"
#include "HforTypeDef.hh"
#include "systematic_defs.hh"
#include "typedefs.hh"
#include "BtagConfig.hh"

class Jet; 
class TTree;
class TFile; 
class TVector2;
class BtagScaler; 

namespace ioflag {
  const unsigned no_flavor = 1u << 0; 
  const unsigned no_truth  = 1u << 1; 
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
  int flavor_truth_label; 
  unsigned bits; 
  BtagScaler* btag_scaler; 
  JetBuffer(); 
  ~JetBuffer(); 
};

class JetFactory
{
public: 
  JetFactory(std::string root_file, int n_jets = 3); 
  ~JetFactory(); 
  void set_btagging(btag::EventConfig); 
  int entries() const; 
  void entry(int); 
  std::vector<Jet> jets() const; 
  TVector2 met() const; 
  ull_t bits() const; 
  double dphi() const; 
  int n_good() const; 
  int n_susy() const; 
  int leading_cjet_pos() const; 
  int subleading_cjet_pos() const; 
  hfor::JetType hfor_type() const; 
  double htx() const; 
  double event_weight(syst::Systematic = syst::NONE) const; 
private: 
  void set_btag(size_t jet_n, btag::JetTag); 
  Jet jet(int) const; 		// not fully supported
  void set_buffer(std::string base_name); 
  TTree* m_tree; 
  TFile* m_file; 
  std::vector<JetBuffer*> m_jet_buffers; 
  double m_met; 
  double m_met_phi; 
  ull_t m_bits; 
  double m_dphi; 
  int m_n_good; 
  int m_n_susy; 
  int m_hfor_type; 
  
  int m_leading_cjet_pos; 
  int m_subleading_cjet_pos; 

  double m_htx; 
  float m_mc_event_weight; 
  
  unsigned m_flags; 
}; 

class Jet: public TLorentzVector
{
public: 
  Jet(JetBuffer* basis, unsigned flags = 0); 
  void set_event_met(const TVector2& met); 
  void set_event_flags(ull_t); 
  double met_dphi() const; 
  double pb() const; 
  double pc() const; 
  double pu() const; 
  int flavor_truth_label() const; 
  bool has_flavor() const; 
  bool pass_tag(btag::JetTag) const; 
  double get_scalefactor(syst::Systematic = syst::NONE) const; 
private: 
  void req_flavor() const; 	// throws rumtime_error if no flavor
  double m_pb; 
  double m_pc; 
  double m_pu; 
  int m_truth_label; 
  double m_met_dphi; 

  unsigned m_config; 
  ull_t m_event_flags; 
  unsigned m_jet_flags; 
  BtagScaler* m_btag_scaler; 
}; 

#endif // JET_FACTORY_H
