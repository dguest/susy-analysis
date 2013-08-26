#ifndef UNORDERED_JET_TAG_FILTER_HH
#define UNORDERED_JET_TAG_FILTER_HH

#include <vector> 
#include "btag_enums.hh"
#include "systematic_defs.hh"
#include "IJetTagFilter.hh"
class Jet; 
class UnorderedJetTagFilter : public IJetTagFilter
{
public: 
  UnorderedJetTagFilter(const std::vector<btag::OperatingPoint>&, 
			syst::Systematic); 
  UnorderedJetTagFilter(const UnorderedJetTagFilter&) = delete; 
  UnorderedJetTagFilter& operator=(const UnorderedJetTagFilter&) = delete; 
  ~UnorderedJetTagFilter(); 
  virtual bool pass(const std::vector<Jet>& jets) const; 
  virtual double jet_scalefactor(const std::vector<Jet>& jets) const; 
private: 
  syst::Systematic m_systematic; 
  int m_n_jets_skipped; 
  int m_n_total_jets_required; 
  std::vector<btag::OperatingPoint> m_ordered_tags; 
};

#endif 
