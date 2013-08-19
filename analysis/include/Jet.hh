#ifndef JET_H
#define JET_H

#include "btag_enums.hh"
#include "systematic_defs.hh"
#include "TLorentzVector.h"
#include "typedefs.hh"
#include "Flavor.hh"

struct JetBuffer; 
class TVector2; 

class Jet: public TLorentzVector
{
public: 
  Jet(const JetBuffer* basis, unsigned flags = 0); 
  void set_event_met(const TVector2& met); 
  void set_mu_met(const TVector2& mu_met); 
  double met_dphi() const; 
  double mu_met_dphi() const; 
  void set_event_flags(ull_t); 
  double flavor_weight(Flavor, btag::Tagger) const; 
  Flavor flavor_truth_label() const; 
  bool has_flavor() const; 
  bool pass_tag(btag::OperatingPoint) const; 
  double get_scalefactor(btag::OperatingPoint, syst::Systematic = syst::NONE) const; 
  bool is_electron_jet() const; 
private: 
  void req_flavor() const; 	// throws rumtime_error if no flavor
  double m_pb; 
  double m_pc; 
  double m_pu; 
  double m_jfc_pb; 
  double m_jfc_pc; 
  double m_jfc_pu; 
  Flavor m_truth_label; 
  double m_met_dphi; 
  double m_mu_met_dphi; 

  unsigned m_ioflags; 
  const JetBuffer* m_buffer; 
  bool m_el_jet; 
}; 

#endif
