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
  SelectedJet(const SusyBuffer& buffer, SUSYObjDef& , int jet_index,
	      bool good);
  int index() const;
  double jvf() const;
  bool bad_tile(const TVector2& met) const;
  bool pass_chf() const;
  float timing() const;
  void set_bit(unsigned, bool set_to = true);
  unsigned bits() const;
  double flavor_weight(btag::Flavor, btag::Tagger) const;
  int flavor_truth_label() const;
  bool has_truth() const;
  void set_flavor_tag(const BtagCalibration*);
  CalResult scale_factor(btag::OperatingPoint) const;
  bool pass_anti_u(btag::OperatingPoint) const;
  bool pass_anti_b(btag::OperatingPoint) const;
  bool in_tag_eta() const;
private:
  void unset_bit(unsigned);
  void set_flavor_tag(btag::Flavor, btag::OperatingPoint,
		      const BtagCalibration*);
  unsigned get_tagger_bit(btag::Flavor, btag::OperatingPoint) const;
  double m_cnn_b;
  double m_cnn_c;
  double m_cnn_u;
  double m_jfc_b;
  double m_jfc_c;
  double m_jfc_u;
  float m_bch_corr;
  bool m_pass_chf;
  float m_timing;
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


#endif // JETS_H
