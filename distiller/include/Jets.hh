#ifndef JETS_H
#define JETS_H

#include "TLorentzVector.h"
#include <vector> 
#include "btag_defs.hh"

class EventJets; 
class SusyBuffer; 
class SUSYObjDef; 
class RunInfo; 
class BtagCalibration; 

namespace jetbit { 
  const unsigned susy            = 1u << 0; 
  const unsigned low_pt          = 1u << 1; 
  // const unsigned el_overlap      = 1u << 2; 
  const unsigned good            = 1u << 3; 
  const unsigned leading         = 1u << 4; 
  const unsigned high_eta        = 1u << 5; 
  const unsigned taggable_eta    = 1u << 6; 
  const unsigned signal_pt       = 1u << 7; 
}


class SelectedJet: public TLorentzVector { 
public: 
  typedef std::pair<double, double> CalResult; 
  SelectedJet(const EventJets* container, int jet_index); 
  double combNN_btag() const; 
  int index() const;
  double jfitcomb_cu(const SusyBuffer& buffer, int jet_index) const;
  double jfitcomb_cb(const SusyBuffer& buffer, int jet_index) const;
  double jvf() const; 
  void set_bit(unsigned); 
  void unset_bit(unsigned); 
  unsigned bits() const; 
  double pb() const; 
  double pu() const; 
  double pc() const; 
  int flavor_truth_label() const; 
  bool has_truth() const; 
  void set_flavor_tag(const BtagCalibration*); 
  CalResult scale_factor(btag::Tagger) const; 
  bool pass_anti_u(btag::Tagger) const; 
  bool pass_anti_b(btag::Tagger) const; 
private: 
  void set_flavor_tag(btag::Flavor, btag::Tagger, const BtagCalibration*); 
  double m_cnn_b; 
  double m_cnn_c; 
  double m_cnn_u; 
  int m_jet_index;
  float m_jvf; //jet_jvtxf
  unsigned m_bits; 
  int m_flavor_truth_label; 
  std::vector<CalResult> m_scale_factor; 
  // std::vector<CalResult> m_fail_factor;  
  std::vector<bool> m_pass_anti_b_tag; 
  std::vector<bool> m_pass_anti_u_tag; 
}; 

class EventJets: public std::vector<SelectedJet*> 
{ 
public: 
  EventJets(const SusyBuffer& buffer, SUSYObjDef& def, 
	    unsigned flags, const RunInfo& info);
  ~EventJets(); 
private: 
  EventJets() {}; 
  EventJets(const EventJets&) {}; 
  void fill(const SusyBuffer& buffer, SUSYObjDef& def, 
	    unsigned flags, const RunInfo& info);
  const SusyBuffer* m_buffer; 
  friend class SelectedJet; 
  unsigned m_flags; 
}; 

namespace { 
  bool check_if_jet(int iJet, 
		    const SusyBuffer& buffer, 
		    SUSYObjDef& def, 
		    const unsigned flags, 
		    const RunInfo& info); 

  bool check_buffer(const SusyBuffer& buffer); 
  
}

#endif // JETS_H
