#ifndef ORDERED_JET_TAG_FILTER_HH
#define ORDERED_JET_TAG_FILTER_HH

#include <vector> 
#include "btag_enums.hh"
#include "IJetTagFilter.hh"
class Jet; 
class OrderedJetTagFilter : public IJetTagFilter
{
public: 
  OrderedJetTagFilter(const std::vector<btag::OperatingPoint>&); 
  OrderedJetTagFilter(const OrderedJetTagFilter&) = delete; 
  OrderedJetTagFilter& operator=(const OrderedJetTagFilter&) = delete; 
  ~OrderedJetTagFilter(); 
  virtual bool pass(const std::vector<Jet>& jets); 
  virtual double jet_scalefactor(const std::vector<Jet>& jets); 
private: 
  int m_n_jets_skipped; 
  int m_n_total_jets_required; 
  std::vector<btag::OperatingPoint> m_ordered_tags; 
};

#endif 
