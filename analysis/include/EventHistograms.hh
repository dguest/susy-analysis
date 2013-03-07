#ifndef EVENT_HISTOGRAMS_HH
#define EVENT_HISTOGRAMS_HH

#include <boost/noncopyable.hpp>
#include <vector> 

class JetFactory; 
class Jet1DHists; 
class TruthJetHists; 
class HistConfig; 
class Histogram; 

namespace H5 { 
  class CommonFG; 
}

class EventHistograms: public boost::noncopyable
{
public: 
  EventHistograms(const HistConfig& config, const unsigned flags = 0); 
  ~EventHistograms(); 
  void fill(const JetFactory* jet_factory, double weight);
  void write_to(H5::CommonFG&) const; 
private: 
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
