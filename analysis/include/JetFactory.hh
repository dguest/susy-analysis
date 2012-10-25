#ifndef JET_FACTORY_H
#define JET_FACTORY_H

#include <string> 
#include <vector> 
#include "TLorentzVector.h"

class Jet; 
class TTree;
class TFile; 

namespace ioflag {
  const unsigned no_flavor = 1u << 0; 
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
};

class JetFactory
{
public: 
  JetFactory(std::string root_file, int n_jets = 3); 
  ~JetFactory(); 
  int entries() const; 
  void entry(int); 
  Jet jet(int) const; 
  std::vector<Jet> jets() const; 
  double met() const; 
  unsigned bits() const; 
  double dphi() const; 
  int n_good() const; 
  int n_susy() const; 
private: 
  void set_buffer(std::string base_name); 
  TTree* m_tree; 
  TFile* m_file; 
  std::vector<JetBuffer*> m_jet_buffers; 
  double m_met; 
  unsigned m_bits; 
  double m_dphi; 
  int m_n_good; 
  int m_n_susy; 
  
  unsigned m_flags; 
}; 

class Jet: public TLorentzVector
{
public: 
  Jet(JetBuffer* basis, unsigned flags = 0); 
  double pb() const; 
  double pc() const; 
  double pu() const; 
  int flavor_truth_label() const; 
  bool has_flavor() const; 
private: 
  void req_flavor() const; 	// throws rumtime_error if no flavor
  double m_pb; 
  double m_pc; 
  double m_pu; 
  int m_truth_label; 

  unsigned m_flags; 
}; 

#endif // JET_FACTORY_H
