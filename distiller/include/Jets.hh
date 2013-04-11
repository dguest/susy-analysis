#ifndef JETS_H
#define JETS_H

#include "TLorentzVector.h"
#include <vector> 
#include "btag_defs.hh"
#include "JetBits.hh"

class EventJets; 
class SusyBuffer; 
class SUSYObjDef; 
struct RunInfo; 
class BtagCalibration; 


class SelectedJet: public TLorentzVector { 
public: 
  typedef std::pair<double, double> CalResult; 
  SelectedJet(const EventJets* container, int jet_index); 
  double combNN_btag() const; 
  int index() const;
  double jfitcomb_cu(const SusyBuffer& buffer, int jet_index) const;
  double jfitcomb_cb(const SusyBuffer& buffer, int jet_index) const;
  double jvf() const; 
  void set_bit(unsigned, bool set_to = true); 
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
  void unset_bit(unsigned); 
  void set_flavor_tag(btag::Flavor, btag::Tagger, const BtagCalibration*); 
  unsigned get_tagger_bit(btag::Flavor, btag::Tagger) const; 
  double m_cnn_b; 
  double m_cnn_c; 
  double m_cnn_u; 
  int m_jet_index;
  float m_jvf; //jet_jvtxf
  unsigned m_bits; 
  int m_flavor_truth_label; 
  std::vector<CalResult> m_scale_factor; 
}; 

typedef std::vector<SelectedJet*> JetContainer; 

class EventJets: public JetContainer
{ 
public: 
  EventJets(const SusyBuffer& buffer, SUSYObjDef& def, 
	    unsigned flags, const RunInfo& info);
  ~EventJets(); 
  void dump_debug(std::ostream&); 
private: 
  EventJets() {}; 
  EventJets(const EventJets&) {}; 
  void fill(const SusyBuffer& buffer, SUSYObjDef& def, 
	    unsigned flags, const RunInfo& info);
  const SusyBuffer* m_buffer; 
  friend class SelectedJet; 
  unsigned m_flags; 
  int m_jets_under_uncalibrated_min; 
}; 

class PreselectionJets: public JetContainer
{
public: 
  PreselectionJets() {}; 
  PreselectionJets(const JetContainer&); 
}; 

class SignalJets: public JetContainer
{
public: 
  SignalJets() {}; 
  SignalJets(const JetContainer&); 
}; 

namespace {
  bool has_min_pt(int iJet, const SusyBuffer& buffer, float pt); 
  bool fill_jet(int iJet, 
		const SusyBuffer& buffer, 
		SUSYObjDef& def, 
		const unsigned flags, 
		const RunInfo& info); 

  bool check_buffer(const SusyBuffer& buffer); 
  
}

#endif // JETS_H
