#ifndef EVENT_HISTOGRAMS_HH
#define EVENT_HISTOGRAMS_HH

#include <boost/noncopyable.hpp>
#include <vector> 

#include "typedefs.hh"
#include "BtagConfig.hh"

class JetFactory; 
class Jet1DHists; 
class TruthJetHists; 
class RegionConfig; 
class Histogram; 

namespace H5 { 
  class CommonFG; 
}

class RegionHistograms: public boost::noncopyable
{
public: 
  RegionHistograms(const RegionConfig& config, const unsigned flags = 0); 
  ~RegionHistograms(); 
  void fill(const JetFactory* jet_factory, ull_t evt_mask, double weight);
  void write_to(H5::CommonFG&) const; 
private: 
  const ull_t m_required; 
  const ull_t m_veto; 
  std::vector<btag::JetTag> m_jet_tag_requirements; 
  Jet1DHists* m_jet1_hists; 
  Jet1DHists* m_jet2_hists; 
  Jet1DHists* m_jet3_hists; 

  Histogram* m_met; 
  Histogram* m_min_dphi; 
  Histogram* m_mttop; 
  Histogram* m_n_good_jets; 

  Histogram* m_htx; 

  Histogram* m_leading_cjet_rank; 
  Histogram* m_subleading_cjet_rank; 

  TruthJetHists* m_jet1_truth; 
  TruthJetHists* m_jet2_truth; 
  TruthJetHists* m_jet3_truth; 
}; 

#endif 
