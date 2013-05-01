#ifndef JET_FACTORY_H
#define JET_FACTORY_H

#include <string> 
#include <vector> 
#include <map>
#include "HforTypeDef.hh"
#include "systematic_defs.hh"
#include "typedefs.hh"
#include "BtagConfig.hh"

class Jet; 
class TTree;
class TFile; 
class TVector2;
class BtagScaler; 
class BtagBuffer; 

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
  std::map<std::string, BtagBuffer*> btag_buffers; 
  // TODO: the btag scalers  shoudldn't be owned by the buffer. 
  std::vector<BtagScaler*> btag_scalers; 
  JetBuffer(); 
  ~JetBuffer(); 
};

class ObjectFactory
{
public: 
  ObjectFactory(std::string root_file, int n_jets); 
  ~ObjectFactory(); 
  void set_btagging(const std::vector<btag::JetTag>&); 
  int entries() const; 
  void entry(int); 
  std::vector<Jet> jets() const; 
  TVector2 met() const; 
  TVector2 mu_met() const; 
  ull_t bits() const; 
  double dphi() const; 
  int n_good() const; 
  int n_susy() const; 
  int leading_cjet_pos() const; 
  int subleading_cjet_pos() const; 
  hfor::JetType hfor_type() const; 
  double htx() const; 
  double event_weight() const; 
private: 
  void set_btag(size_t jet_n, btag::JetTag); 
  Jet jet(int) const; 		// not fully supported
  void set_buffer(std::string base_name); 
  TTree* m_tree; 
  TFile* m_file; 
  std::vector<JetBuffer*> m_jet_buffers; 
  double m_met; 
  double m_met_phi; 
  double m_mu_met; 
  double m_mu_met_phi; 
  ull_t m_bits; 
  double m_dphi; 
  int m_n_good; 
  int m_n_susy; 
  int m_hfor_type; 
  
  int m_leading_cjet_pos; 
  int m_subleading_cjet_pos; 

  double m_htx; 
  float m_mc_event_weight; 
  
  unsigned m_ioflags; 
}; 


#endif // JET_FACTORY_H
